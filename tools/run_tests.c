#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* POSIX interfaces used here are available on both Linux and macOS. */
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} Buffer;

typedef struct {
    Buffer line;
    Buffer pending;
    int in_case;
    int failed;
} Reporter;

static int append(Buffer *buffer, const char *data, size_t length) {
    /* Bound output from buggy implementations instead of exhausting memory. */
    const size_t limit = 1024 * 1024;
    if (length > limit - buffer->length) return 0;
    size_t needed = buffer->length + length;
    if (needed > buffer->capacity) {
        size_t capacity = buffer->capacity ? buffer->capacity : 256;
        while (capacity < needed) capacity *= 2;
        char *storage = realloc(buffer->data, capacity);
        if (!storage) return 0;
        buffer->data = storage;
        buffer->capacity = capacity;
    }
    if (length) memcpy(buffer->data + buffer->length, data, length);
    buffer->length = needed;
    return 1;
}

static int starts_with(const Buffer *line, const char *prefix) {
    size_t length = strlen(prefix);
    return line->length >= length && memcmp(line->data, prefix, length) == 0;
}

static void print_buffer(const Buffer *buffer) {
    if (buffer->length) fwrite(buffer->data, 1, buffer->length, stdout);
}

static int case_header(const Buffer *line) {
    if (!starts_with(line, "  Case ")) return 0;
    size_t i = 7;
    while (i < line->length && line->data[i] >= '0' && line->data[i] <= '9') ++i;
    const char separator[] = " | input:";
    return i > 7 && line->length - i >= sizeof separator - 1 &&
           memcmp(line->data + i, separator, sizeof separator - 1) == 0;
}

static int report_line(Reporter *reporter) {
    Buffer *line = &reporter->line;
    if (case_header(line)) {
        reporter->pending.length = 0;
        reporter->in_case = 1;
        reporter->failed = 0;
        return append(&reporter->pending, line->data, line->length);
    }
    if (starts_with(line, "PASS:") || starts_with(line, "FAIL:")) {
        reporter->pending.length = 0;
        reporter->in_case = 0;
    }
    if (!reporter->in_case) {
        print_buffer(line);
    } else {
        if (!reporter->failed && starts_with(line, "    FAIL line ")) {
            print_buffer(&reporter->pending);
            reporter->pending.length = 0;
            reporter->failed = 1;
        }
        if (reporter->failed) print_buffer(line);
        else return append(&reporter->pending, line->data, line->length);
    }
    fflush(stdout);
    return 1;
}

static int report_bytes(Reporter *reporter, const char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (!append(&reporter->line, data + i, 1)) return 0;
        if (data[i] == '\n') {
            if (!report_line(reporter)) return 0;
            reporter->line.length = 0;
        }
    }
    return 1;
}

static int monotonic_time(double *seconds) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return 0;
    *seconds = (double)now.tv_sec + (double)now.tv_nsec / 1000000000.0;
    return 1;
}

static void stop_child(pid_t child, int *status) {
    kill(child, SIGKILL);
    while (waitpid(child, status, 0) < 0 && errno == EINTR) {}
}

static const char *signal_name(int number) {
    switch (number) {
    case SIGSEGV: return "SIGSEGV";
    case SIGABRT: return "SIGABRT";
    case SIGFPE: return "SIGFPE";
    case SIGILL: return "SIGILL";
    case SIGBUS: return "SIGBUS";
    case SIGTERM: return "SIGTERM";
    case SIGKILL: return "SIGKILL";
    case SIGINT: return "SIGINT";
    case SIGPIPE: return "SIGPIPE";
    default: return strsignal(number);
    }
}

static int run_tests(const char *executable, double timeout) {
    double started;
    if (!monotonic_time(&started)) {
        perror("Cannot read monotonic clock");
        return 2;
    }
    int output[2];
    if (pipe(output) != 0) {
        perror("Cannot create test output pipe");
        return 2;
    }
    if (fcntl(output[0], F_SETFL, O_NONBLOCK) < 0) {
        perror("Cannot configure test output pipe");
        close(output[0]);
        close(output[1]);
        return 2;
    }
    pid_t child = fork();
    if (child < 0) {
        perror("Cannot start tests");
        close(output[0]);
        close(output[1]);
        return 2;
    }
    if (child == 0) {
        close(output[0]);
        if (dup2(output[1], STDOUT_FILENO) < 0) {
            perror("Cannot capture test output");
            _exit(2);
        }
        close(output[1]);
        execl(executable, executable, (char *)NULL);
        perror("Cannot run tests");
        _exit(2);
    }
    close(output[1]);

    Reporter reporter = {0};
    int status = 0, timed_out = 0, error = 0, done = 0, pipe_closed = 0;
    while (!done) {
        double now;
        if (!monotonic_time(&now)) { error = 1; break; }
        double remaining = timeout - (now - started);
        if (remaining <= 0) { timed_out = 1; break; }
        int milliseconds = remaining >= 0.05 ? 50 : (int)(remaining * 1000) + 1;
        struct pollfd descriptor = {output[0], POLLIN, 0};
        int ready = poll(&descriptor, pipe_closed ? 0 : 1, milliseconds);
        if (ready < 0 && errno != EINTR) { error = 1; break; }
        if (ready > 0) {
            char chunk[4096];
            ssize_t length = read(output[0], chunk, sizeof chunk);
            if (length > 0 && !report_bytes(&reporter, chunk, (size_t)length)) {
                error = 1;
                break;
            }
            if (length == 0) pipe_closed = 1;
            if (length < 0 && errno != EAGAIN && errno != EINTR) { error = 1; break; }
        }
        pid_t result = waitpid(child, &status, WNOHANG);
        if (result == child) done = 1;
        else if (result < 0 && errno != EINTR) { error = 1; break; }
    }
    if (!done) stop_child(child, &status);

    /* Read anything written just before exit, including the active input. */
    if (!error) {
        char chunk[4096];
        ssize_t length;
        while ((length = read(output[0], chunk, sizeof chunk)) > 0) {
            if (!report_bytes(&reporter, chunk, (size_t)length)) { error = 1; break; }
        }
        if (reporter.line.length && !report_line(&reporter)) error = 1;
    }
    close(output[0]);
    int successful = !error && !timed_out && WIFEXITED(status) && WEXITSTATUS(status) == 0;
    if (!successful && reporter.in_case && !reporter.failed) print_buffer(&reporter.pending);
    free(reporter.line.data);
    free(reporter.pending.data);
    fflush(stdout);

    if (error) {
        fputs("Cannot capture test output (I/O error or case output exceeds 1 MiB).\n", stderr);
        return 2;
    }
    if (timed_out) {
        printf("FAIL: test timed out after %gs (possible infinite loop).\n", timeout);
        return 1;
    }
    if (WIFSIGNALED(status)) {
        printf("FAIL: test process crashed: %s.\n", signal_name(WTERMSIG(status)));
        puts("The last input printed above identifies the active check.");
        puts("Try make test N=... SANITIZE=1 for memory diagnostics.");
        return 1;
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 2;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fputs("Usage: test-runner EXECUTABLE\n", stderr);
        return 2;
    }
    const char *setting = getenv("TEST_TIMEOUT");
    double timeout = 5;
    if (setting) {
        char *end;
        errno = 0;
        timeout = strtod(setting, &end);
        if (end == setting || *end || errno == ERANGE || !isfinite(timeout) || timeout <= 0) {
            fputs("TEST_TIMEOUT must be a positive finite number of seconds.\n", stderr);
            return 2;
        }
    }
    struct stat info;
    if (stat(argv[1], &info) != 0 || !S_ISREG(info.st_mode) || access(argv[1], X_OK) != 0) {
        fprintf(stderr, "Cannot run tests: %s is not an executable file.\n", argv[1]);
        return 2;
    }
    return run_tests(argv[1], timeout);
}

#include "support.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#if EXERCISE == 41
void run_tests(void)
{
    const unsigned values[] = {0xA5u, 7, 0, 1, UINT_MAX, UINT_MAX ^ (UINT_MAX >> 1)};
    for (size_t i = 0; i < 6; ++i) {
        unsigned expected = 0, v = values[i];
        while (v) { expected += v % 2; v /= 2; }
        char label[100]; snprintf(label, sizeof label, "value=%u (0x%X)", values[i], values[i]); CASE(label);
        UINT(expected, bit_count(values[i]));
    }
}
#elif EXERCISE == 42
void run_tests(void)
{
    const unsigned char bytes[][6] = {{0x55, 0x78, 0x56, 0x34, 0x12, 0x55}, {0x55, 0, 0, 0, 0, 0x55}, {0x55, 0xFF, 0xFF, 0xFF, 0xFF, 0x55}, {0x55, 0, 0, 0, 0x80, 0x55}};
    const uint32_t expected[] = {UINT32_C(0x12345678), 0, UINT32_MAX, UINT32_C(0x80000000)};
    for (size_t i = 0; i < 4; ++i) {
        char label[100]; snprintf(label, sizeof label, "bytes=%02X %02X %02X %02X at buffer+1", bytes[i][1], bytes[i][2], bytes[i][3], bytes[i][4]); CASE(label);
        UINT(expected[i], read_u32_le(bytes[i] + 1));
    }
}
#elif EXERCISE == 43
void run_tests(void)
{
    const uint32_t values[] = {UINT32_C(0x12345678), 0, UINT32_MAX, UINT32_C(0x80000000)};
    const unsigned char expected[][6] = {{0x55, 0x78, 0x56, 0x34, 0x12, 0x55}, {0x55, 0, 0, 0, 0, 0x55}, {0x55, 0xFF, 0xFF, 0xFF, 0xFF, 0x55}, {0x55, 0, 0, 0, 0x80, 0x55}};
    for (size_t i = 0; i < 4; ++i) {
        unsigned char actual[6]; memset(actual, 0x55, 6);
        char label[100]; snprintf(label, sizeof label, "value=0x%08lx, destination=buffer+1", (unsigned long)values[i]); CASE(label);
        write_u32_le(actual + 1, values[i]); BYTES(expected[i], actual, 6);
    }
}
#elif EXERCISE == 44
void run_tests(void)
{
    int values[][6] = {{3, 1, 2, 99, 99, 99}, {5, 4, 3, 2, 1, 0}, {2, 2, -1, 2, -1, 0}, {INT_MAX, 0, INT_MIN, -1, 1, 1}};
    const int expected[][6] = {{1, 2, 3, 99, 99, 99}, {0, 1, 2, 3, 4, 5}, {-1, -1, 0, 2, 2, 2}, {INT_MIN, -1, 0, 1, 1, INT_MAX}};
    for (size_t i = 0; i < 4; ++i) {
        CASE(i == 0 ? "[3,1,2,99,99,99], count=3" : i == 1 ? "[5,4,3,2,1,0]" : i == 2 ? "[2,2,-1,2,-1,0]" : "[INT_MAX,0,INT_MIN,-1,1,1]");
        array_sort(values[i], i == 0 ? 3 : 6); ARRAY(expected[i], values[i], 6);
    }
    CASE("NULL, count=0"); array_sort(NULL, 0);
    int one = 42; CASE("[42], count=1"); array_sort(&one, 1); INT(42, one);
}
#elif EXERCISE == 45
static size_t comparisons;
static int compare_int(const void *a, const void *b)
{
    ++comparisons; int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
typedef struct { int key; char payload[12]; } Record;
static int compare_record(const void *key, const void *record)
{
    ++comparisons; int a = *(const int *)key, b = ((const Record *)record)->key;
    return (a > b) - (a < b);
}
void run_tests(void)
{
    int values[1024]; for (int i = 0; i < 1024; ++i) values[i] = i * 2;
    const int keys[] = {0, 1024, 2046, -1, 2048, 3};
    for (size_t i = 0; i < 6; ++i) {
        comparisons = 0; char label[100]; snprintf(label, sizeof label, "1024 sorted even integers, key=%d", keys[i]); CASE(label);
        void *expected = keys[i] >= 0 && keys[i] <= 2046 && keys[i] % 2 == 0 ? values + keys[i] / 2 : NULL;
        POINTER(values, sizeof values, expected, my_bsearch(&keys[i], values, 1024, sizeof *values, compare_int));
        TRUE(comparisons <= 11);
    }
    const Record records[] = {{1, "one"}, {3, "three"}, {5, "five"}}; int key = 3;
    CASE("records with 16-byte-like stride, key=3");
    POINTER(records, sizeof records, records + 1, my_bsearch(&key, records, 3, sizeof *records, compare_record));
    int duplicates[] = {2, 2, 2}; key = 2; CASE("[2,2,2], key=2; any match accepted");
    void *p = my_bsearch(&key, duplicates, 3, sizeof *duplicates, compare_int);
    TRUE(p == duplicates || p == duplicates + 1 || p == duplicates + 2);
    CASE("NULL base, count=0"); POINTER(NULL, 0, NULL, my_bsearch(&key, NULL, 0, sizeof key, compare_int));
}
#elif EXERCISE == 46
void run_tests(void)
{
    IntVector vector = {0}; int expected[128];
    for (int i = 0; i < 128; ++i) {
        char label[100]; snprintf(label, sizeof label, "push %d into vector of length %zu", i - 64, vector.length); CASE(label);
        int *previous = vector.data; size_t capacity = vector.capacity, length = vector.length;
        expected[i] = i - 64;
        INT(0, vector_push(&vector, expected[i])); UINT((size_t)i + 1, vector.length);
        TRUE(vector.capacity >= vector.length); ARRAY(expected, vector.data, (size_t)i + 1);
        if (length < capacity) { TRUE(vector.data == previous); UINT(capacity, vector.capacity); }
        if (!vector.data || vector.length != (size_t)i + 1 || vector.capacity < vector.length) break;
    }
    free(vector.data);
}
#elif EXERCISE == 47
void run_tests(void)
{
    ByteBuffer b = {0}; const unsigned char block[] = {0x41, 0, 0xFF};
    unsigned char expected[192];
    CASE("empty buffer, data=NULL, count=0"); INT(0, buffer_append(&b, NULL, 0));
    UINT(0, b.length); UINT(0, b.capacity); POINTER(NULL, 0, NULL, b.data);
    for (size_t i = 0; i < 64; ++i) {
        memcpy(expected + i * 3, block, 3);
        char label[100]; snprintf(label, sizeof label, "append 41 00 FF to buffer of length %zu", b.length); CASE(label);
        unsigned char *previous = b.data; size_t capacity = b.capacity, length = b.length;
        INT(0, buffer_append(&b, block, 3)); UINT((i + 1) * 3, b.length); TRUE(b.capacity >= b.length);
        BYTES(expected, b.data, (i + 1) * 3);
        if (capacity - length >= 3) { TRUE(previous == b.data); UINT(capacity, b.capacity); }
        if (!b.data || b.length != (i + 1) * 3 || b.capacity < b.length) break;
    }
    if (b.data && b.length) {
        unsigned char *previous = b.data; size_t length = b.length, capacity = b.capacity;
        CASE("append count=SIZE_MAX; reject addition overflow before reading source");
        INT(-1, buffer_append(&b, NULL, SIZE_MAX)); TRUE(previous == b.data); UINT(length, b.length); UINT(capacity, b.capacity);
        BYTES(expected, b.data, length);
        CASE("append zero bytes to existing buffer"); INT(0, buffer_append(&b, NULL, 0)); TRUE(previous == b.data); UINT(length, b.length); UINT(capacity, b.capacity);
    }
    free(b.data);
}
#elif EXERCISE == 48
void run_tests(void)
{
    IntNode *head = NULL, *saved[4] = {NULL};
    const int values[] = {3, 1, 4, -2};
    for (size_t i = 0; i < 4; ++i) {
        char label[80]; snprintf(label, sizeof label, "append %d to list of length %zu", values[i], i); CASE(label);
        INT(0, list_push_back(&head, values[i]));
        IntNode *p = head;
        for (size_t j = 0; j <= i; ++j) {
            TRUE(p != NULL); if (!p) return;
            INT(values[j], p->value);
            if (j < i) TRUE(saved[j] == p); else saved[j] = p;
            p = p->next;
        }
        POINTER(NULL, 0, NULL, p);
    }
    CASE("free four-node list and NULL"); list_free(head); list_free(NULL);
}
#elif EXERCISE == 49
static size_t predicate_calls;
static int is_negative(int value) { ++predicate_calls; return value < 0; }
static int remove_all(int value) { (void)value; ++predicate_calls; return 1; }
void run_tests(void)
{
    const int values[] = {-1, -2, 3, -4, 5, -6}; IntNode *nodes[6];
    for (size_t i = 0; i < 6; ++i) {
        nodes[i] = malloc(sizeof *nodes[i]);
        if (!nodes[i]) { perror("test setup malloc"); exit(2); }
        nodes[i]->value = values[i]; nodes[i]->next = NULL;
        if (i) nodes[i - 1]->next = nodes[i];
    }
    IntNode *head = nodes[0]; CASE("[-1,-2,3,-4,5,-6], predicate=value<0");
    UINT(4, list_remove_if(&head, is_negative)); UINT(6, predicate_calls); TRUE(head == nodes[2]);
    if (head != nodes[2]) return;
    TRUE(head->next == nodes[4]); if (head->next != nodes[4]) return;
    POINTER(NULL, 0, NULL, head->next->next); INT(3, head->value); INT(5, head->next->value);
    CASE("[3,5], predicate=value<0"); predicate_calls = 0;
    UINT(0, list_remove_if(&head, is_negative)); UINT(2, predicate_calls);
    CASE("[3,5], predicate=always true"); predicate_calls = 0;
    UINT(2, list_remove_if(&head, remove_all)); UINT(2, predicate_calls); POINTER(NULL, 0, NULL, head);
    CASE("empty list"); predicate_calls = 0; UINT(0, list_remove_if(&head, is_negative)); UINT(0, predicate_calls);
}
#elif EXERCISE == 50
static void expect_line(FILE *stream, const void *expected, size_t count, int status)
{
    char *line = NULL; size_t length = 999;
    INT(status, read_line(stream, &line, &length)); UINT(count, length);
    if (status == 1) {
        BYTES(expected, line, count); TRUE(line != NULL);
        if (line && length == count) INT(0, (unsigned char)line[count]);
    } else POINTER(NULL, 0, NULL, line);
    free(line);
}
void run_tests(void)
{
    FILE *stream = tmpfile(); if (!stream) { perror("test setup tmpfile"); exit(2); }
    const unsigned char input[] = {'h', 'i', '\n', '\n', 'a', 0, 'b', '\n', 'x', '\r', '\n', 'e', 'n', 'd'};
    if (fwrite(input, 1, sizeof input, stream) != sizeof input) { perror("test setup fwrite"); exit(2); }
    rewind(stream);
    CASE("stream=hi\\n\\na\\0b\\nx\\r\\nend; first line"); expect_line(stream, "hi", 2, 1);
    CASE("next line: empty"); expect_line(stream, "", 0, 1);
    const unsigned char binary[] = {'a', 0, 'b'};
    CASE("next line: a\\0b"); expect_line(stream, binary, 3, 1);
    CASE("next line: x\\r"); expect_line(stream, "x\r", 2, 1);
    CASE("next line: end, no trailing newline"); expect_line(stream, "end", 3, 1);
    CASE("EOF twice"); expect_line(stream, NULL, 0, 0); expect_line(stream, NULL, 0, 0);
    fclose(stream);
    stream = tmpfile(); if (!stream) { perror("test setup tmpfile"); exit(2); }
    CASE("empty stream"); expect_line(stream, NULL, 0, 0);
    char long_line[8192]; memset(long_line, 'x', sizeof long_line);
    if (fwrite(long_line, 1, sizeof long_line, stream) != sizeof long_line || fputc('\n', stream) == EOF) { perror("test setup write"); exit(2); }
    rewind(stream); CASE("8192 x bytes followed by newline"); expect_line(stream, long_line, sizeof long_line, 1);
    CASE("EOF after long line"); expect_line(stream, NULL, 0, 0); fclose(stream);
}
#endif

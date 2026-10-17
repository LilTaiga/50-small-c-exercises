#!/bin/sh
# Maintainer regression checks, independent of student implementations.
set -eu
ulimit -c 0
cd "$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
mkdir -p build
directory=$(mktemp -d build/runner-checks.XXXXXX)
trap 'rm -f "$directory/test-runner" "$directory/fixture" "$directory/stdout" "$directory/stderr" "$directory/compiler" "$directory/editor.c"; rmdir "$directory"' 0

compiler=${CC:-cc}
flags=${CFLAGS:--std=c11 -Wall -Wextra -Wpedantic -Werror}
# Word splitting permits CC="ccache cc" and normal space-separated CFLAGS.
$compiler $flags -Iinclude -Itests -fsyntax-only tools/run_tests.c tests/support.c tests/framework_fixture.c
$compiler $flags tools/run_tests.c -o "$directory/test-runner"
$compiler $flags -Iinclude -Itests tests/support.c tests/framework_fixture.c -o "$directory/fixture"

fail() {
    printf 'Regression failed: %s\n' "$1" >&2
    cat "$directory/stdout" "$directory/stderr" >&2
    exit 1
}
contains() {
    grep -F -q -- "$1" "$directory/${2:-stdout}" || fail "missing $1"
}
absent() {
    if grep -F -q -- "$1" "$directory/${2:-stdout}"; then fail "unexpected $1"; fi
}
run_case() {
    expected=$1
    status=0
    FRAMEWORK_MODE=$2 TEST_TIMEOUT=${3-2} "$directory/test-runner" "$directory/fixture" \
        >"$directory/stdout" 2>"$directory/stderr" || status=$?
    [ "$status" -eq "$expected" ] || fail "$2 returned $status, expected $expected"
}

run_case 0 pass
contains 'PASS: 11/11 checks'
absent 'Case '
run_case 0 bool-pass
contains 'PASS: 4/4 checks'
run_case 1 bool-fail
contains 'expected: true'
contains 'actual:   false'
contains 'expected: false'
contains 'actual:   true'
for marker in byte 0x 0b; do absent "$marker"; done

run_case 1 mixed
contains 'input: failing middle'
contains 'FAIL: 2/4 checks passed across 4 cases.'
for marker in 'passing before' 'passing after' 'framework fixture'; do absent "$marker"; done
[ "$(grep -c '^  Case ' "$directory/stdout")" -eq 1 ] || fail 'duplicate case header'
[ "$(grep -c '^    FAIL line ' "$directory/stdout")" -eq 2 ] || fail 'missing failure detail'

run_case 1 escaped-input
contains 'input: "a\nb\t\"\\"'
contains 'a="\xFF", b="\x7F"'
contains 'a="a\nb", b="a\tb", count=2'
run_case 1 fail
for marker in 'expected:' 'actual:' "'A'" '0x41' '0b10100101' '42 0A 7F' '|B..|' \
    '[1, 9, 3]' 'buffer + 1' 'buffer + 2' 'NULL' 'PARSE_OK (0)' 'PARSE_OVERFLOW (2)' \
    'FAIL: 0/12 checks' 'first differing byte: 79 (expected 0x00, actual 0xFF)' \
    'first differing index: 39 (expected 0, actual 42)'; do contains "$marker"; done

run_case 1 crash
contains SIGSEGV
contains 'input: framework fixture'
run_case 1 hang 0.1
contains 'timed out'
contains 'input: framework fixture'
for mode in mixed-crash mixed-hang; do
    run_case 1 "$mode" 0.1
    contains 'input: failing middle'
    contains 'input: active input before interruption'
    absent 'input: passing before'
    absent 'input: framework fixture'
done
run_case 7 exit
run_case 1 partial
contains 'input: framework fixture'
contains 'output without a final newline'
run_case 1 closed-output 0.1
contains 'timed out'
contains 'input: framework fixture'
run_case 2 flood
contains 'input: framework fixture'
contains 'case output exceeds 1 MiB' stderr
run_case 0 stderr
contains 'diagnostic on stderr' stderr
absent 'Case '
for value in 0 -1 nan inf nope 1e999 ''; do
    run_case 2 pass "$value"
    contains 'positive finite' stderr
done

status=0
"$directory/test-runner" "$directory/missing" >"$directory/stdout" 2>"$directory/stderr" || status=$?
[ "$status" -eq 2 ] || fail 'missing executable accepted'
contains 'Cannot run tests:' stderr
status=0
"$directory/test-runner" >"$directory/stdout" 2>"$directory/stderr" || status=$?
[ "$status" -eq 2 ] || fail 'missing argument accepted'
contains 'Usage:' stderr

for value in '' 0 51 01 abc '1 2' '%' '1%'; do
    status=0
    make --no-print-directory -n test "N=$value" >"$directory/stdout" 2>"$directory/stderr" || status=$?
    [ "$status" -ne 0 ] || fail "invalid exercise accepted: $value"
    contains 'N must be' stderr
done
for target in selftest clean; do
    if make --no-print-directory -n "$target" >"$directory/stdout" 2>"$directory/stderr"; then
        fail "removed target still exists: $target"
    fi
done

cat >"$directory/compiler" <<'SH'
#!/bin/sh
printf 'compiler diagnostic\n' >&2
exit "${MOCK_COMPILER_STATUS:-0}"
SH
chmod +x "$directory/compiler"
for target in "$directory/01-run" "$directory/01-test" "$directory/test-runner" check; do
    for verbose in 0 1; do
        make --no-print-directory "$target" "BUILD=$directory" "CC=$directory/compiler" "V=$verbose" \
            >"$directory/stdout" 2>"$directory/stderr" || fail "mock compilation failed: $target"
        contains 'compiler diagnostic' stderr
        if [ "$verbose" -eq 1 ]; then contains '-Iinclude'; else absent '-Iinclude'; fi
    done
done
status=0
MOCK_COMPILER_STATUS=1 make --no-print-directory "$directory/01-run" "BUILD=$directory" \
    "CC=$directory/compiler" V=0 >"$directory/stdout" 2>"$directory/stderr" || status=$?
[ "$status" -ne 0 ] || fail 'compiler error accepted'
contains 'compiler diagnostic' stderr
absent '-Iinclude'

cat >"$directory/editor.c" <<'C'
#include "exercises.h"
#include "lib.h"
#include "support.h"
uint32_t editor_value = UINT32_C(42);
C
$compiler $(cat compile_flags.txt) -Werror -fsyntax-only "$directory/editor.c"
printf '%s\n' 'All runner and Make regression checks passed.'

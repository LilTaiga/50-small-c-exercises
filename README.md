# C, one function at a time

50 small exercises for learning C, pointers, memory, and reuse. Implement the
functions yourself; **the supplied stubs are supposed to fail their tests**.
No solutions are included.

## Start here

You need a C11 compiler (`cc`, GCC, or Clang), GNU Make, and Python 3. No packages
or network access are needed.

```sh
make run N=1
make test N=1
```

Read `exercises/01.md`, edit `exercises/01.c`, and use its `main` as your
playground. Tests use their own `main`; leave your playground inside the
`#ifndef EXERCISE_TEST` guard. Keep trying until the tests pass, then move on.
Select exercises with unpadded numbers: `N=1` through `N=50`.

Tests print the active input before calling your code. Failures show expected
and actual values: escaped strings, byte dumps with ASCII, integer arrays,
binary/hexadecimal integers, and buffer-relative pointer offsets where useful.
Comparisons follow the function contract (for example, string comparison checks
the sign, not an implementation-specific return value).

```sh
make test N=23 SANITIZE=1  # AddressSanitizer + undefined-behavior checks
make check               # Syntax/type-check every playground and test suite
make build               # Build all 100 executables; does not run student tests
make selftest            # Check test infrastructure, not student solutions
```

Tests time out after 5 seconds; override with `TEST_TIMEOUT=10 make test N=50`.
A crashing exercise is reported as a failure, not as a successful test run.
Sanitizer support depends on your compiler/platform. Build output lives under
`build/`; `make clean` removes only that generated directory. Override the
compiler with `make CC=clang ...`.

## Reuse your work

`lib.c` is linked into **every** playground and test executable. Declare reusable
helpers in `lib.h`, and include that header wherever needed. Earlier helpers are
allowed in every exercise unless its Markdown says otherwise. Do not call the
standard-library equivalent of the function you are implementing.

To reuse a completed exercise, move its implementation (not its `main`) into
`lib.c` and leave its declaration in `include/exercises.h`. Remove the definition
from the original exercise file and include `lib.h` there. Both the old exercise
and later ones will then link against the shared implementation. Never leave
two definitions of the same function. The tests continue to check old exercises.

## Contracts and learning progression

- Exercises 1–10 explicitly explain boundary cases.
- Exercises 11–15 still give boundary examples.
- Exercises 16–30 mention that additional cases exist, without listing them.
- Exercises 31–50 no longer mention them; discover them through test feedback.

Requirements such as ownership, valid input ranges, and error handling remain
explicit throughout. Tests never demand a particular result for undefined
behavior. Standard-like functions follow their documented subset; character
classification is deliberately ASCII-only. `my_strdup` is a familiar extension
(and part of C23), not an ISO C11 function. `next_token` is a custom interface.

Allocation exercises may use real `malloc`, `realloc`, and `free` as documented.
Writing a heap allocator is outside this course. The goal is correctness first,
not matching the performance of an optimized system libc.

## Exercise map

See the [full numbered index](exercises/README.md) for links to every task.

| Stage | Exercises | Focus |
|---|---|---|
| 1 | 01–10 | Logic, ASCII, integers, first pointers |
| 2 | 11–20 | Strings and pointer traversal |
| 3 | 21–30 | Raw memory, searching, tokenization |
| 4 | 31–40 | Parsing, allocation, ownership |
| 5 | 41–50 | Bits, byte order, callbacks, containers, I/O |

Each numbered Markdown file gives the signature, requirements, library
restrictions, and examples. Each C file is an independently buildable playground.

## About the tests

Tests check behavior, not your source-code choices: follow the stated algorithm,
reuse, and library restrictions yourself. Passing the provided checks is useful
feedback, not a proof of correctness. Sanitizers can catch memory errors that
ordinary output comparisons cannot. Allocation failure and I/O failure handling
are required by the contracts, but real resource failures are not forced by the
default suites; size-overflow rejection is checked where practical.

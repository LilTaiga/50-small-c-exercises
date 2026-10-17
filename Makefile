.DEFAULT_GOAL := help
CC ?= cc
V ?= 0
Q := $(if $(filter 1,$(V)),,@)
CPPFLAGS += -Iinclude -I. -Itests
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g -O0
LDLIBS ?=
NUMBERS := 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50
IDS := $(shell printf '%02d\n' $(NUMBERS))
BUILD := build/default
ifeq ($(SANITIZE),1)
BUILD := build/sanitized
override CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
override LDFLAGS += -fsanitize=address,undefined
endif

ifneq ($(filter run test,$(MAKECMDGOALS)),)
ifneq ($(filter $(N),$(NUMBERS)),$(N))
$(error Choose an exercise: make run N=1 or make test N=1 (N must be 1 through 50))
endif
ifneq ($(words $(N)),1)
$(error N must be a single number from 1 through 50)
endif
ID := $(shell printf '%02d' $(N))
endif

TEST_SOURCES := tests/support.c tests/main.c $(wildcard tests/stage*.c)
HEADERS := include/exercises.h lib.h tests/support.h
.PHONY: help run test check build FORCE
help:
	@printf '%s\n' 'make run N=1       Run your playground main' 'make test N=1      Test your implementation' 'make check         Syntax/type-check all exercises and tests' 'make build         Build all playgrounds and test executables' 'Add V=1 to show compilation commands.' 'Add SANITIZE=1 to enable AddressSanitizer and UBSan.'

run: $(BUILD)/$(ID)-run
	@./$<

test: $(BUILD)/$(ID)-test $(BUILD)/test-runner
	@$(BUILD)/test-runner $<

$(BUILD)/test-runner: tools/run_tests.c FORCE
	@mkdir -p $(BUILD)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD)/%-run: exercises/%.c lib.c $(HEADERS) FORCE
	@mkdir -p $(BUILD)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) $< lib.c $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD)/%-test: exercises/%.c lib.c $(HEADERS) $(TEST_SOURCES) FORCE
	@mkdir -p $(BUILD)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -DEXERCISE_TEST -DEXERCISE=$(shell expr $* + 0) $< lib.c $(TEST_SOURCES) $(LDFLAGS) $(LDLIBS) -o $@

check:
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only tools/run_tests.c
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only tests/framework_fixture.c tests/support.c
	$(Q)set -e; for id in $(IDS); do \
	  n=$$(expr $$id + 0); \
	  $(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only exercises/$$id.c lib.c; \
	  $(CC) $(CPPFLAGS) $(CFLAGS) -DEXERCISE_TEST -DEXERCISE=$$n -fsyntax-only exercises/$$id.c lib.c $(TEST_SOURCES); \
	done
	@printf '%s\n' 'All 50 exercises and test suites passed syntax/type checking.'

build: $(BUILD)/test-runner $(foreach id,$(IDS),$(BUILD)/$(id)-run $(BUILD)/$(id)-test)

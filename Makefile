# GNU Make: supports native Windows Make and MSYS/POSIX Make.
ifeq ($(origin CC),default)
CC = gcc
endif
CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
LDLIBS ?= -lm

ifeq ($(OS),Windows_NT)
EXE := .exe
endif

# Preserve a POSIX shell selected by MSYS Make. Forcing cmd.exe here makes
# older MSYS Make launch an interactive prompt instead of executing recipes.
ifeq ($(findstring sh,$(SHELL)),)
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /C
WINDOWS_CMD := 1
endif
endif

BUILD_DIR := build/make
TESTBENCH := $(BUILD_DIR)/lpf_testbench$(EXE)
ifeq ($(WINDOWS_CMD),1)
TEST_COMMAND := $(subst /,\,$(TESTBENCH))
else
TEST_COMMAND := ./$(TESTBENCH)
endif

.PHONY: all test clean
all: $(TESTBENCH)

$(BUILD_DIR):
ifeq ($(WINDOWS_CMD),1)
	if not exist "$(subst /,\,$(BUILD_DIR))" mkdir "$(subst /,\,$(BUILD_DIR))"
else
	mkdir -p "$(BUILD_DIR)"
endif

$(TESTBENCH): src/low_pass_filter.c testbench/low_pass_filter_testbench.c include/low_pass_filter.h Makefile | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -Iinclude src/low_pass_filter.c testbench/low_pass_filter_testbench.c $(LDFLAGS) $(LDLIBS) -o "$@"

test: $(TESTBENCH)
	$(TEST_COMMAND)

# Only remove the generated Make testbench executable.
clean:
ifeq ($(WINDOWS_CMD),1)
	if exist "$(subst /,\,$(TESTBENCH))" del /Q "$(subst /,\,$(TESTBENCH))"
else
	rm -f -- "$(TESTBENCH)"
endif


# Automatically build all C source files in the project.
# Stores generated object/dependency files in a separate build folder.

CC ?= cc
INCLUDE_DIRS := $(shell find . -name "*.h" -exec dirname {} \; | sort | uniq)
CFLAGS ?= -Wall -Wextra -pedantic -std=c99 $(addprefix -I, $(INCLUDE_DIRS))

BUILD_DIR := build
SRC := $(shell find . -name "*.c" -not -path "./$(BUILD_DIR)/*" -not -path "./test/*")
OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))
DEPS := $(OBJ:.o=.d)

TEST_SRC := $(shell find test -name "*.c")
TEST_OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(TEST_SRC))

.PHONY: all test clean

all: main

test: test_runner
	./test_runner

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

test_runner: build/data/rope.o build/test/test_rope.o build/test/unity.o
	$(CC) $(CFLAGS) -o $@ $^

-include $(DEPS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) main test_runner

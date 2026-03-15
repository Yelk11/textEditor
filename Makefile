# Automatically build all C source files in the project.
# Stores generated object/dependency files in a separate build folder.

CC ?= cc
CFLAGS ?= -Wall -Wextra -pedantic -std=c99 -Imodel

BUILD_DIR := build
SRC := $(wildcard *.c model/*.c)
OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))
DEPS := $(OBJ:.o=.d)

.PHONY: all clean

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

-include $(DEPS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) main

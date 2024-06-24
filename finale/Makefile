CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =

SRC_DIR = .
BUILD_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

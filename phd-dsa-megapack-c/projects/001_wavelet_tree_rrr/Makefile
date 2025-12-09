CC?=gcc
CFLAGS:=-std=c11 -O2 -Wall -Wextra -Wpedantic
INCLUDES:=-Iinclude
SRC:=src/wavelet_tree_rrr_core.c src/main.c
TEST_SRC:=tests/test_wavelet_tree_rrr.c src/wavelet_tree_rrr_core.c

BIN_DIR:=bin

all: $(BIN_DIR)/demo

$(BIN_DIR):
	@mkdir -p $@

$(BIN_DIR)/demo: $(SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(BIN_DIR)/test: $(TEST_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

.PHONY: test clean

test: $(BIN_DIR)/test
	./$(BIN_DIR)/test

clean:
	rm -rf $(BIN_DIR)

CC = gcc
FLAGS = -Wall -Wextra -std=c11 -Isrc -MMD -MP
TARGET = parser

LIB_SRC = src/gettoken.c src/parser.c src/reader.c
VAL = src/validator.c

OBJS = $(LIB_SRC:.c=.o)
VAL_OBJ = $(VAL:.c=.o)

TEST_PARSER = run_parser
TEST_VALIDATOR = run_validator

.PHONY: all clean run test1 test2

all: $(TARGET)

$(TARGET): $(OBJS) $(VAL_OBJ) src/main.o
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

$(TEST_PARSER): $(OBJS) src/test_parser.o
	$(CC) $(FLAGS) $^ -o $@

$(TEST_VALIDATOR): $(VAL_OBJ) src/test_validator.o
	$(CC) $(FLAGS) $^ -o $@

test1: $(TEST_PARSER)
	./$(TEST_PARSER)

test2: $(TEST_VALIDATOR)
	./$(TEST_VALIDATOR)

clean:
	rm -f $(OBJS) $(VAL_OBJ) src/main.o tests/*.o $(TARGET) $(TEST_PARSER) $(TEST_VALIDATOR) *.d

-include $(OBJS:.o=.d) $(VAL_OBJ:.o=.d)
-include src/main.d
-include tests/test_parser.d tests/test_validator.d
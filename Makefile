CC = gcc
FLAGS = -Wall -Wextra -std=c11 -Isrc -MMD -MP
TARGET = parser

LIB_SRC = src/gettoken.c src/parser.c src/reader.c
VAL_SRC = src/validator.c

OBJS = $(LIB_SRC:.c=.o)
VAL_OBJ = $(VAL_SRC:.c=.o)
MAIN_OBJ = src/main.o

TEST_PARSER_OBJ = tests/test_parser.o
TEST_VALIDATOR_OBJ = tests/test_validator.o

TEST_PARSER = run_parser
TEST_VALIDATOR = run_validator

.PHONY: all clean run test1 test2

all: $(TARGET)

$(TARGET): $(OBJS) $(VAL_OBJ) $(MAIN_OBJ)
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

$(TEST_PARSER): $(OBJS) $(VAL_OBJ) $(TEST_PARSER_OBJ)
	$(CC) $(FLAGS) $^ -o $@

$(TEST_VALIDATOR): $(VAL_OBJ) $(TEST_VALIDATOR_OBJ)
	$(CC) $(FLAGS) $^ -o $@

test1: $(TEST_PARSER)
	./$(TEST_PARSER)

test2: $(TEST_VALIDATOR)
	./$(TEST_VALIDATOR)

clean:
	rm -f $(OBJS) $(VAL_OBJ) $(MAIN_OBJ)
	rm -f $(TEST_PARSER_OBJ) $(TEST_VALIDATOR_OBJ)
	rm -f $(OBJS:.o=.d) $(VAL_OBJ:.o=.d) $(MAIN_OBJ:.o=.d)
	rm -f $(TEST_PARSER_OBJ:.o=.d) $(TEST_VALIDATOR_OBJ:.o=.d)
	rm -f $(TARGET) $(TEST_PARSER) $(TEST_VALIDATOR)

-include $(OBJS:.o=.d) $(VAL_OBJ:.o=.d) $(MAIN_OBJ:.o=.d)
-include $(TEST_PARSER_OBJ:.o=.d) $(TEST_VALIDATOR_OBJ:.o=.d)
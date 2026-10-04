CC = gcc
FLAGS = -Wall -Wextra -std=c11
TARGET = parser
LIB_SRC = src/gettoken.c src/parser.c src/reader.c
VAL = src/validator.c
OBJS = $(LIB_SRC:.c=.o)
VAL_OBJ = $(VAL:.c=.o)
HEADER1 = parser.h
HEADER2 = validator.h

.PHONY: all clean run test1 test2 cleantests

all: $(TARGET)

$(TARGET): $(OBJS) $(VAL_OBJ) src/main.c
	$(CC) $(FLAGS) $^ -o parser

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(VAL_OBJ) $(TARGET)

run_parser: $(LIB_SRC) tests/test_parser.c
	$(CC) $(FLAGS) $^ -o run_parser

run_validator: $(VAL) tests/test_validator.c
	$(CC) $(FLAGS) $^ -o run_validator

test1: run_parser
	./run_parser

test2: run_validator
	./run_validator

cleantests:
	rm -f run_parser
	rm -f run_validator
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I.
OBJ = main.o parsing.o execution.o
TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJ) -lreadline

main.o: main.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c

parsing.o: parsing.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c parsing.c

execution.o: execution.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c execution.c

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

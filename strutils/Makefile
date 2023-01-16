CC = gcc
BIN = bin
SRC = /

FLAGS = -Wall -Wextra -g
VPATH = .

SOURCES = $(wildcard ./*.c)
OBJECTS = $(patsubst ./%.c, $(BIN)/%.o, $(SOURCES))
TARGET = main.exe

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@

$(BIN)/%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ -I"$(SRC)"

clean:
	rm $(BIN)/*.o
	rm $(TARGET)

.PHONY: clean

CC := gcc
CFLAGS := -Wall -Werror -Werror=vla -g -std=c11 -lm -fsanitize=address

TARGET := minesweeper

all: $(TARGET)

clean: 
	rm -f $(TARGET)
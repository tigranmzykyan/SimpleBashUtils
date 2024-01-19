CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11

all: clean s21_grep

s21_grep : s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o : s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf  *.o *.a *.so s21_grep

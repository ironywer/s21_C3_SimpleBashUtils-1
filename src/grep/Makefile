CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SANITIZE = -fsanitize=address -fsanitize=address -fsanitize=undefined

all : s21_grep

s21_grep : 
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean :
	rm s21_grep

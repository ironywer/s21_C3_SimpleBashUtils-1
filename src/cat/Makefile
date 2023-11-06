CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SANITIZE = -fsanitize=address -fsanitize=address -fsanitize=undefined

all: s21_cat

s21_cat : 
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean :
	rm s21_cat

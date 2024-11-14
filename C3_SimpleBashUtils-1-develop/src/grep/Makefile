SHELL := /bin/bash
CC = gcc
FLAGS = -Wall -Werror -Wextra 
FILE = test1.txt test2.txt
PATTERNS = paterns.reg

all: s21_grep

s21_grep: grep.o
	$(CC) $(FLAGS) grep.o -o s21_grep

grep.o: grep.c
	$(CC) $(FLAGS) -c grep.c -o grep.o

test: s21_grep
	bash test.sh

clean:
	rm -rf *.o
	rm s21_grep

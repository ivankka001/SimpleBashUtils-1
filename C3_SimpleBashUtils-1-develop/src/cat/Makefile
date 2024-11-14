SHELL := /bin/bash
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
FILE = test.txt

all: s21_cat

s21_cat: cat.o
	$(CC) $(FLAGS) cat.o -o s21_cat

cat.o: cat.c
	$(CC) $(FLAGS) -c cat.c -o cat.o

test:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat --number-nonblank $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -etsb $(FILE)) <(cat -etsb $(FILE))

 
clean:
	rm -rf *.o
	rm s21_cat

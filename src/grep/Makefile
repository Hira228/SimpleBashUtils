CC = gcc
FLAGS = -Wall -Wextra -Werror
FILE_GREP = s21_grep.c
NAME_GREP = s21_grep

all: s21_grep
	
rebuild: clean all

clean:
	rm -rf *.o *.out *.exe s21_grep

s21_grep: 
	$(CC) $(FLAGS) $(FILE_GREP) -o $(NAME_GREP)

CC = gcc
FLAGS = -Wall -Wextra -Werror
FILE_CAT = s21_cat.c
NAME_CAT = s21_cat

all: s21_cat
	
rebuild: clean all

clean:
	rm -rf *.o *.out *.exe s21_cat

s21_cat: 
	$(CC) $(FLAGS) $(FILE_CAT) -o $(NAME_CAT)
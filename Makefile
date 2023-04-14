CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -ansi -pedantic

search: search.c
	$(CC) $(CFLAGS) -o search search.c

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -ansi -pedantic

CinexSearch: CinexSearch.c
	$(CC) $(CFLAGS) -o CinexSearch CinexSearch.c

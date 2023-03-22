CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb 
eqtn-matrix : eqtn-matrix.c
	$(CC) $(CFLAGS) -o eqtn-matrix eqtn-matrix.c

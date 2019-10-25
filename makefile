################################################################################
# Make file for banking application
################################################################################
CC = gcc
CFLAGS = -Wall -Werror -ansi -Irle.h

Banking: $(OBJ)
	$(CC) -o $@ $@.c $(CFLAGS)

all: Banking

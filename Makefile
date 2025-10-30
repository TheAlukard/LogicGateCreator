CC=gcc
CFLAGS= -O3 -Wall -Wextra -Werror -pedantic
SRC=$(shell ls ./src/*.c)
EXEC=./build/lgc

lgc: $(SRC)
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)

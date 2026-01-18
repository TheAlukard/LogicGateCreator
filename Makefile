CC=gcc
CFLAGS=-O3 -Werror -pedantic -lm
SRC=$(shell ls ./src/*.c)
EXEC=./build/lgc

all: lgc

build: $(SRC)
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)

lgc:
	make -B build

run: lgc
	./build/lgc.exe

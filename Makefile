# Makefile for the Plugable EDitor #

CC=gcc
FLAGS=-Wall -pedantic -ldl
EXE=bin/ped

all: install
	$(CC) src/tty.c -o tty.o -c
	$(CC) src/utils.c -o utils.o -c
	$(CC) src/commands.c -o commands.o -c
	$(CC) $(FLAGS) -o $(EXE) src/ped.c tty.o utils.o commands.o

test: clean all
	./bin/ped test
	rm -rf test

install:
	mkdir -p obj
	mkdir -p bin

clean:
	rm -rf test
	rm -rf bin
	rm -rf obj

# Makefile for the Plugable EDitor #

CC=gcc
FLAGS=-Wall -pedantic
EXE=bin/ped

all: install
	$(CC) $(FLAGS) -o $(EXE) src/ped.c

install:
	mkdir -p obj
	mkdir -p bin

clean:
	rm -rf bin
	rm -rf obj

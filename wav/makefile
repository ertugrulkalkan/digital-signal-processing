CC = gcc
CFLAGS = -I ./include
BUILD = ./build

all: main

main: wavefile.o
	$(CC) $(CFLAGS) ./main.c $(BUILD)/* -o ./main.bin

wavefile.o: directory
	$(CC) $(CFLAGS) -c ./src/wavefile.c -o $(BUILD)/wavefile.o

directory:
	mkdir -p $(BUILD)

.PHONY: clean
clean:
	rm -rf $(BUILD)
	rm -rf ./out/*
	rm -f main.bin

CC=gcc
CFLAGS=-Wall -Wextra -g -Og
CFILES=main.c
DEPS=
OBJECTS=main.o
SRC=.
SOURCE=$(wildcard $(SRC)/*.c)

all: main.o run

%:%.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPS) 

%.o:%.c $(DEPS)
	@printf "\n"
	@printf "##################\n"
	@printf "Building %s\n" $@
	@printf "##################\n"
	@printf "\n"
	$(CC) $(CFLAGS) -o $@ $^

%.s: %.c
	$(CC) -S $^

run: main.o
	@printf "\n"
	@printf "###############\n"
	@printf "#   Running   #\n"
	@printf "###############\n"
	@printf "\n"
	@mv main.o minesweeper-c
	./minesweeper-c

help:
	@echo "src: $(SOURCE)"
	@echo "obj: $(OBJECTS)"

# cleaning everything that can be automatically recreated with "make".
clean:
	@printf "Cleaning\n"
	rm $(OBJECTS)

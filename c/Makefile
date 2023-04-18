CC=gcc
CFLAGS=-Wall -Wextra -g -Og
CFILES=main.c
DEPS=
OBJECTS=main.o
SRC=.
SOURCE=$(wildcard $(SRC)/*.c)

all: minesweeper-c run

%:%.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPS) 

%.o:%.c $(DEPS)
	@printf "\n"
	@printf "##################\n"
	@printf "Building %s\n" $@
	@printf "##################\n"
	@printf "\n"
	$(CC) $(CFLAGS) -o $@ $^

minesweeper-c: main.o
	mv main.o minesweeper-c

%.s: %.c
	$(CC) -S $^

run: minesweeper-c
	@printf "\n"
	@printf "###############\n"
	@printf "#   Running   #\n"
	@printf "###############\n"
	@printf "\n"
	./minesweeper-c

help:
	@echo "src: $(SOURCE)"
	@echo "obj: $(OBJECTS)"

# cleaning everything that can be automatically recreated with "make".
clean:
	@printf "Cleaning\n"
	rm $(OBJECTS)

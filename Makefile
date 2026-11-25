CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = towerdefense

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXEC)
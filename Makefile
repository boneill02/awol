.POSIX:

include config.mk

SRC = src/main.c src/game.c src/ecs.c src/graphics.c
OBJ = $(SRC:.c=.o)
LIBS = -lSDL2 -lSDL2_image -luuid

all: game

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

game: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

clean:
	rm -f game $(OBJ) game-$(VERSION).tar.gz

dist: clean

install: game

uninstall:

.PHONY: all clean dist install uninstall

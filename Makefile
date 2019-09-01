.POSIX:

include config.mk

SRC = src/main.c src/game.c src/ecs.c src/graphics.c
OBJ = $(SRC:.c=.o)
LIBS = -lSDL2 -lSDL2_image -luuid

all: awol

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

awol: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

clean:
	rm -f awol $(OBJ) awol-$(VERSION).tar.gz

dist: clean
	mkdir awol
	cp -rf assets/ doc/ src/ awol.6 config.mk Makefile README.md LICENSE awol/
	tar -cf awol.tar awol
	gzip awol.tar
	rm -rf awol/

install: awol
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f awol $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/awol
	mkdir -p $(DESTDIR)$(MANPREFIX)/man6
	sed "s/VERSION/$(VERSION)/g" < awol.6 > $(DESTDIR)$(MANPREFIX)/man6/awol.6

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/awol
	rm -f $(DESTDIR)$(MANPREFIX)/man6/awol.6

.PHONY: all clean dist install uninstall

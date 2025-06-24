VERSION = dev

PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man

CC = gcc
CFLAGS = -Werror -std=gnu99 -pedantic -O2
LDFLAGS = -s

VERSION = dev

PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man

CC ?= gcc
CFLAGS ?= -Wall -std=c99 -pedantic -O2
LDFLAGS ?= -s

EXECUTABLE=shitlisp

CC=gcc
INCLUDES=-I. -Ilib
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -g
LDFLAGS=

SOURCES=$(wildcard *.c)
LIBSOURCES=$(wildcard lib/*.c)

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

debug: CFLAGS += -DDEBUG
debug: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(LIBSOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $^ -o $@

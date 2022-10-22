EXECUTABLE=shitlisp

CC=gcc
INCLUDES=-I. -Ilib
CFLAGS=
LDFLAGS=

SOURCES=$(wildcard src/*.c)
LIBSOURCES=$(wildcard lib/*.c)

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(LIBSOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $^ -o $@

.PHONY: all run debug clean

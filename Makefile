EXECUTABLE=shitlisp

CC=gcc
INCLUDES=-I. -Ilib
CFLAGS=
LDFLAGS=

SOURCES=$(wildcard src/*.c)
LIBSOURCES=$(wildcard lib/*.c)
LIBOBJECTS=$(LIBSOURCES:.c=.o)

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(LIBOBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $^ -o $@

lib/%.o: lib/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all run debug clean

#include "sport.h"

#include "sgc.h"

static struct port* create_port(struct state* state, unsigned char flags) {
    struct port* port = gc_create_object(state, OBJECT_TYPE_PORT, sizeof(*port));
    port->flags = flags;
    return port;
}

static void port_flats_to_file_mode(unsigned char flags, char* mode) {
    mode[0] = (flags & PORT_FLAG_INPUT) ? 'r' : 'w';
    mode[1] = (flags & PORT_FLAG_TYPE) ? 'b' : '\0';
    mode[2] = '\0';
}

struct port* open_file_port(struct state* state, const char* filename, unsigned char flags) {
    char mode[3];
    FILE* file;

    port_flats_to_file_mode(flags, mode);
    file = fopen(filename, mode);

    if (!file) {
        // todo: signal error
    }

    struct port* port = create_port(state, flags);
    port->file.file = file;

    return port;
}

struct port* open_input_file(struct state* state, const char *filename, bool binary) {
    return open_file_port(
        state,
        filename,
        PORT_FLAG_INPUT | (PORT_FLAG_TYPE & !binary) | PORT_FLAG_INPUT_OPEN | PORT_FLAG_TYPE_FILE
    );
}

struct port* open_output_file(struct state* state, const char *filename, bool binary) {
    return open_file_port(
        state,
        filename,
        PORT_FLAG_OUTPUT | (PORT_FLAG_TYPE & !binary) | PORT_FLAG_OUTPUT_OPEN | PORT_FLAG_TYPE_FILE
    );
}

struct port* open_input_string(struct state* state, const char *data, size_t length) {
    return NULL;
}

struct port* open_output_string(struct state* state) {
    return NULL;
}

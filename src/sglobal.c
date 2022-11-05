#include <stdlib.h>

#include "sglobal.h"

void init_symbol_table(struct symbol_table* table) {
    table->symbols = NULL;
    table->count = 0;
    table->capacity = 0;
}

status_t global_init(struct global* global) {
    status_t result;

    global->seed = 0; // TODO: seed
    init_symbol_table(&global->symbol_table);

    if ((result = gc_init(&global->gc)) != STATUS_OK) {
        return result;
    }
    return STATUS_OK;
}

struct global* global_new() {
    struct global* g = malloc(sizeof(*g));
    if (g == NULL) return NULL;

    if (global_init(g) != STATUS_OK) {
        free(g);
        return NULL;
    }

    return g;
}

void global_free(struct global* g) {
    gc_terminate(&g->gc);
    free(g);
}

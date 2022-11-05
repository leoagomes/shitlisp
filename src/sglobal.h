#ifndef __SGLOBAL_H__
#define __SGLOBAL_H__

#include "stypes.h"
#include "sgc.h"

struct symbol_table {
    struct symbol** symbols;
    size_t count, capacity;
};

struct global {
    struct gc gc;
    struct state* main_state;
    struct symbol* symbols;
    uint seed;
    struct symbol_table symbol_table;
};

struct global* global_new();
void global_free(struct global* g);

#endif

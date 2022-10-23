#ifndef __SGLOBAL_H__
#define __SGLOBAL_H__

#include "stypes.h"
#include "sgc.h"

struct global {
    struct gc gc;
    struct state* main_state;
    struct symbol* symbols;
};

struct global* global_new();
void global_free(struct global* g);

#endif

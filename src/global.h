#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "types.h"
#include "object.h"
#include "gc.h"

struct global {
    struct gc gc;
    struct state* main_state;
};

struct global* global_new();
void global_free(struct global* g);

#endif

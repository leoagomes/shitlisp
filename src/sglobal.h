#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "stypes.h"
#include "sobject.h"
#include "sgc.h"

struct global {
    struct gc gc;
    struct state* main_state;
};

struct global* global_new();
void global_free(struct global* g);

#endif

#ifndef __SCONS_H__
#define __SCONS_H__

#include "stypes.h"

struct cons* cons_new(
    struct state* state,
    struct value* car,
    struct value* cdr
);

#endif

#ifndef __SAUX_H__
#define __SAUX_H__

#include <stdio.h>

#include "stypes.h"

struct state* bootstrap_state();
void destroy_state(struct state* state);

void print_value(struct value* v, FILE* f);
void print_object(struct object* obj, FILE* f);

#endif

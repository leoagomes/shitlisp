#ifndef __SPRINTER_H__
#define __SPRINTER_H__

#include <stdio.h>

#include "stypes.h"

void print_value(struct state* state, struct value* value, FILE* stream);
void print_object(struct state* state, struct object* value, FILE* stream);

#endif

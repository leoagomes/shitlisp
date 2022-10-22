#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "types.h"

#include <stdio.h>

void print_value(struct value* v, FILE* f);
void print_object(struct object* obj, FILE* f);

#endif

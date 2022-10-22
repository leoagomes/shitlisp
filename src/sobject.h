#ifndef __SOBJECT_H__
#define __SOBJECT_H__

#include "stypes.h"

#include <stdio.h>

void print_value(struct value* v, FILE* f);
void print_object(struct object* obj, FILE* f);

#endif

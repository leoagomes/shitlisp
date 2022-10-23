#ifndef __SSYMBOL_H__
#define __SSYMBOL_H__

#include "stypes.h"

void symbolv_from_c_string(struct state* state, const char* text, struct value* dst);
struct symbol* symbol_from_c_string(struct state* state, const char* text);

#endif

#ifndef __SSYMBOL_H__
#define __SSYMBOL_H__

#include "stypes.h"

#define INITIAL_SYMBOL_TABLE_CAPACITY 128

void symbolv_from_c_string(struct state* state, const char* text, size_t length, struct value* dst);
struct symbol* symbol_from_c_string(struct state* state, const char* text, size_t length);

unsigned int hash_as_symbol(const char* text, size_t length, unsigned int seed);
void init_symbol_table(struct state* state);

#endif

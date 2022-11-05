#include <string.h>
#include <limits.h>
#include "utf8.h"

#include "sglobal.h"
#include "ssymbol.h"
#include "sgc.h"

unsigned int hash_as_symbol(const char* text, size_t length, unsigned int seed) {
    unsigned int hash = seed ^ (unsigned int)length;
    for (; length > 0; length--) 
        hash ^= (hash << 5) + (hash >> 2) + (unsigned int)text[length - 1];
    return hash;
}

struct symbol* symbol_new(struct state* state, const char* text, size_t length) {
    struct symbol* symbol = (struct symbol*)alloc_object(
        state,
        OBJECT_TYPE_SYMBOL,
        sizeof(struct symbol) + length + 1
    );
    strncpy(symbol->text, text, length);
    symbol->text[length] = '\0';
    symbol->length = length;
    symbol->hash = hash_as_symbol(text, length, state->global->seed);
    return symbol;
}

void symbolv_from_c_string(
    struct state* state,
    const char* text,
    size_t length,
    struct value* dst
) {
    struct symbol* symbol = symbol_from_c_string(state, text, length);
    if (symbol == NULL) {
        dst->type = VALUE_TYPE_NIL;
        return;
    }

    dst->value.object_as.symbol = symbol;
    dst->type = VALUE_TYPE_OBJECT;
}

void grow_symbol_table(struct state* state, struct symbol_table* table) {
    if (table->count == INT_MAX) {
        // TODO: perform full gc
        if (table->count == INT_MAX)
            fail(state, STATUS_SYMBOL_TABLE_LIMIT);
    }

}

struct symbol* symbol_from_c_string(
    struct state* state,
    const char* text,
    size_t length
) {
    // look up the symbol in the symbol table
    unsigned int hash = hash_as_symbol(text, length, state->global->seed);
    struct symbol_table* table = &state->global->symbol_table;
    struct symbol** symbol_list = &table->symbols[hash % table->capacity];

    for (
        struct symbol* symbol = *symbol_list;
        symbol != NULL;
        symbol = symbol->table_next
    ) {
        if (
            symbol->hash == hash &&
            symbol->length == length &&
            strncmp(symbol->text, text, length) == 0
        ) // TODO: GC shennanigans
            return symbol;
    }

    if (table->count >= table->capacity) {
        grow_symbol_table(state, table);
        symbol_list = &table->symbols[hash % table->capacity];
    }
}

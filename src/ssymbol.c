#include <string.h>
#include <limits.h>
#include "utf8.h"

#include "sglobal.h"
#include "ssymbol.h"
#include "sgc.h"

struct symbol* create_new_symbol(
    struct state* state,
    const char* text,
    size_t length,
    unsigned int hash
);
void grow_symbol_table(struct state* state, struct symbol_table* table);

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
    struct symbol* symbol = create_new_symbol(state, text, length, hash);
    symbol->hash = hash;
    symbol->table_next = *symbol_list;
    *symbol_list = symbol;
    table->count++;
    return symbol;
}

struct symbol* create_new_symbol(
    struct state* state,
    const char* text,
    size_t length,
    unsigned int hash
) {
    struct symbol* symbol = (struct symbol*)gc_create_object(
        state,
        OBJECT_TYPE_SYMBOL,
        sizeof(struct symbol) + length + 1
    );
    strncpy(symbol->text, text, length);
    symbol->text[length] = '\0';
    symbol->length = length;
    symbol->hash = hash;
    return symbol;
}

unsigned int hash_as_symbol(const char* text, size_t length, unsigned int seed) {
    unsigned int hash = seed ^ (unsigned int)length;
    for (; length > 0; length--) 
        hash ^= (hash << 5) + (hash >> 2) + (unsigned int)text[length - 1];
    return hash;
}


/* symbol table */

void rehash_table(struct symbol** symbols, size_t new_size, size_t old_size) {
    int i;
    for (i = old_size; i < new_size; i++)
        symbols[i] = NULL;

    for (i = 0; i < old_size; i++) {
        struct symbol* symbol = symbols[i];
        symbols[i] = NULL;

        while (symbol != NULL) {
            struct symbol* next = symbol->table_next;
            unsigned int new_index = symbol->hash % new_size;
            symbol->table_next = symbols[new_index];
            symbols[new_index] = symbol;
            symbol = next;
        }
    }
}

void init_symbol_table(struct state* state) {
    struct symbol_table* table = &state->global->symbol_table;
    table->count = 0;
    table->capacity = INITIAL_SYMBOL_TABLE_CAPACITY;
    table->symbols = protected_alloc(state, sizeof(struct symbol*) * table->capacity);
    rehash_table(table->symbols, table->capacity, 0);
    // TODO: pre-create memory exhausted message
}

void resize_symbol_table(struct state* state, int new_capacity) {
    struct symbol_table* table = &state->global->symbol_table;
    int old_capacity = table->capacity;
    struct symbol** new_symbols = protected_realloc(
        state,
        table->symbols,
        sizeof(struct symbol*) * new_capacity
    );
    if (new_symbols == NULL) {
        if (new_capacity < old_capacity)
            rehash_table(table->symbols, new_capacity, old_capacity);
        return;
    }

    table->symbols = new_symbols;
    table->capacity = new_capacity;
    if (new_capacity > old_capacity)
        rehash_table(table->symbols, new_capacity, old_capacity);
}

void grow_symbol_table(struct state* state, struct symbol_table* table) {
    if (table->count == INT_MAX) {
        // TODO: perform full gc
        if (table->count == INT_MAX)
            fail(state, STATUS_SYMBOL_TABLE_LIMIT);
    }
    resize_symbol_table(state, table->capacity * 2);
}

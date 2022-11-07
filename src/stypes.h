#ifndef __STYPES_H__
#define __STYPES_H__

#include <ctype.h>
#include <stddef.h>
#include <setjmp.h>

typedef enum {
    OBJECT_TYPE_NIL,
    OBJECT_TYPE_SYMBOL,
    OBJECT_TYPE_CONS,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_STATE,
    OBJECT_TYPE_MAP,
    OBJECT_TYPE_COUNT
} object_type_t;

typedef enum {
    VALUE_TYPE_NIL,
    VALUE_TYPE_INT,
    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_POINTER,
    VALUE_TYPE_OBJECT,
    VALUE_TYPE_TRUE,
    VALUE_TYPE_FALSE,
    VALUE_TYPE_COUNT
} value_type_t;

typedef enum {
    STATUS_OK,
    STATUS_INVALID,
    STATUS_OUT_OF_MEMORY,
    STATUS_INVALID_ARGUMENT,
    STATUS_SYMBOL_TABLE_LIMIT,
} status_t;

struct state;
typedef int (*generic_callback)(struct state*, void*);

struct object;
struct string;
struct symbol;
struct cons;

struct value {
    value_type_t type;
    union {
        int integer;
        double number;
        void *pointer;
        struct object *object;
        union {
            struct symbol* symbol;
            struct cons* cons;
        } object_as;
    } value;
};

#define OBJECT_HEADER \
    struct object *_next, *_prev; \
    object_type_t _type

struct object {
    OBJECT_HEADER;
};

struct string {
    OBJECT_HEADER;
    char *data;
    size_t length;
};

struct symbol {
    OBJECT_HEADER;
    struct symbol* table_next;
    size_t length;
    unsigned int hash;
    char text[0];
};

struct cons {
    OBJECT_HEADER;
    struct value car, cdr;
};

struct vector {
    OBJECT_HEADER;
    struct value *data;
    size_t length;
};

struct map_node {
    struct value value, key;
    int next;
};

struct map {
    OBJECT_HEADER;
    unsigned char logsize;
    struct map_node *nodes, *free;
    struct map* meta;
    int count;
};

struct frame {
    OBJECT_HEADER;
};

struct state {
    OBJECT_HEADER;
    struct jumplist {
        struct jumplist *next;
        jmp_buf buf;
    }* jumplist;
    struct global* global;
};

struct environment {
    OBJECT_HEADER;
};

struct function {
    OBJECT_HEADER;
};

#define NIL_VALUE ((struct value){VALUE_TYPE_NIL, {0}})
#define TRUE_VALUE ((struct value){VALUE_TYPE_TRUE, {0}})
#define FALSE_VALUE ((struct value){VALUE_TYPE_FALSE, {0}})
static struct value _nil = NIL_VALUE;
static struct value _t = TRUE_VALUE;
static struct value _f = FALSE_VALUE;

#define value_is_object(v) ((v)->type == VALUE_TYPE_OBJECT)
#define value_is_nil(v) ((v)->type == VALUE_TYPE_NIL)
#define value_is_cons(val) (value_is_object(val) && object_is_cons((val)->value.object))
#define value_is_list(val) (value_is_nil(val) || value_is_cons(val))
#define value_is_symbol(val) (value_is_object(val) && object_is_symbol((val)->value.object))

#define object_is_type(obj, type) ((obj)->_type == (type)) // TODO: apply mask
#define object_is_cons(obj) object_is_type(obj, OBJECT_TYPE_CONS)
#define object_is_symbol(obj) object_is_type(obj, OBJECT_TYPE_SYMBOL)

#define value_copy(dst, src) \
    do { \
        (dst)->type = (src)->type; \
        (dst)->value = (src)->value; \
    } while (0)


#endif

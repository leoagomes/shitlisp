#ifndef __STYPES_H__
#define __STYPES_H__

#include <ctype.h>
#include <stddef.h>
#include <setjmp.h>

typedef enum {
    OBJECT_TYPE_NIL,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_SYMBOL,
    OBJECT_TYPE_VALUE,
    OBJECT_TYPE_LIST,
    OBJECT_TYPE_POINTER,
    OBJECT_TYPE_COUNT
} object_type_t;

typedef enum {
    VALUE_TYPE_NIL,
    VALUE_TYPE_INT,
    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_POINTER,
    VALUE_TYPE_OBJECT,
    VALUE_TYPE_COUNT
} value_type_t;

typedef enum {
    STATUS_OK,
    STATUS_OUT_OF_MEMORY,
} status_t;

struct state;
typedef int (*generic_callback)(struct state*, void*);

struct object;
struct string;
struct symbol;
struct list;

struct value {
    value_type_t type;
    union {
        int i;
        double d;
        void *p;
        union {
            struct object* obj;
            struct string* str;
            struct symbol* sym;
            struct list* list;
            struct vector* vec;
            struct frame* frame;
            struct state* state;
            struct environment* env;
            struct function* fn;
        } object;
    } v;
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
    size_t length;
    char name[0];
};

struct list {
    OBJECT_HEADER;
    struct value car, cdr;
};

struct vector {
    OBJECT_HEADER;
    struct value *data;
    size_t length;
};

struct map {
    OBJECT_HEADER;
    struct value *data;
    size_t length;
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

#define value_is_object(v) ((v)->type == VALUE_TYPE_OBJECT)
#define value_is_nil(v) ((v)->type == VALUE_TYPE_NIL)
#define value_is_list(val) (value_is_object(val) && ((val)->v.object.obj->_type == OBJECT_TYPE_LIST))

#endif

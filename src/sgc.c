#include <stdlib.h>

#include "sgc.h"

status_t gc_init(struct gc* gc) {
    objl_close(&gc->object_list);
    objl_close(&gc->root_list);
    return STATUS_OK;
}

status_t gc_terminate(struct gc* gc) {
    return STATUS_OK;
}

gc_status_t collect_garbage(struct state* state, int full) {
    // TODO:
    return GC_STATUS_OK;
}


/* --- object lifecycle --- */

struct object* alloc_object(struct state* state, object_type_t type, size_t size) {
    struct object* obj = palloc(state, size);
    obj->_type = type;
    objl_close(obj);

    return obj;
}

void* palloc(struct state* state, size_t size) {
    void* obj = malloc(size);
    if (obj == NULL) {
        collect_garbage(state, 1);
        obj = malloc(size);
        if (obj == NULL) {
            fail(state, STATUS_OUT_OF_MEMORY);
            return NULL; // unreachable
        }
    }
    return obj;
}

void* prealloc(struct state* state, void* ptr, size_t size) {
    // TODO: care about GC/memory
    return realloc(ptr, size);
}

void free_object(struct state* state, struct object* obj) {
    // free(obj);
}

int mark_value(struct state* state, struct value* value) {
    return 0;
}

int mark_object(struct state* state, struct object* object) {
    if (object == NULL) return 0;

    return 0;
}

/* --- object list --- */

void objl_close(struct object* obj) {
    obj->_next = obj->_prev = obj;
}

void objl_prepend(struct object* obj, struct object* list) {
    obj->_next = list;
    obj->_prev = list->_prev;
    list->_prev->_next = obj;
    list->_prev = obj;
}

void objl_append(struct object* obj, struct object* list) {
    obj->_next = list->_next;
    obj->_prev = list;
    list->_next->_prev = obj;
    list->_next = obj;
}

void objl_remove(struct object* obj) {
    obj->_prev->_next = obj->_next;
    obj->_next->_prev = obj->_prev;
    objl_close(obj);
}

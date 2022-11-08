#ifndef __SGC_H__
#define __SGC_H__

#include "sruntime.h"

typedef enum {
    GC_STATUS_OK,
} gc_status_t;

struct gc {
    struct object object_list, root_list;
};

status_t gc_init(struct gc* gc);
status_t gc_terminate(struct gc* gc);

gc_status_t gc_collect_garbage(struct state*, int full);

/* --- object lifecycle --- */
struct object* gc_create_object(struct state*, object_type_t type, size_t size);
void gc_free_object(struct state*, struct object*);

void* protected_alloc(struct state*, size_t size);
void* protected_realloc(struct state*, void* ptr, size_t size);
// actually just free, but should be wrapped in case of GC
void protected_free(struct state*, void* ptr);

int mark_value(struct state*, struct value*);
int mark_object(struct state*, struct object*);

/* --- object list --- */
void objl_close(struct object* obj);
void objl_prepend(struct object* obj, struct object* list);
void objl_append(struct object* obj, struct object* list);
void objl_remove(struct object* obj);

#endif

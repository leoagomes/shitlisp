#ifndef __GC_H__
#define __GC_H__

#include "sobject.h"
#include "sruntime.h"

typedef enum {
    GC_STATUS_OK,
} gc_status_t;

struct gc {
    struct object object_list, root_list;
};

status_t gc_init(struct gc* gc);
status_t gc_terminate(struct gc* gc);

gc_status_t collect_garbage(struct state*, int full);

/* --- object lifecycle --- */
struct object* alloc_object(struct state*, object_type_t type, size_t size);

/* --- object list --- */
void objl_close(struct object* obj);
void objl_prepend(struct object* obj, struct object* list);
void objl_append(struct object* obj, struct object* list);
void objl_remove(struct object* obj);

#endif

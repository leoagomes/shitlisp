#include "scons.h"

#include "sgc.h"

status_t cons_init(struct cons* cons) {
    cons->car.type = VALUE_TYPE_NIL;
    cons->cdr.type = VALUE_TYPE_NIL;
    return STATUS_OK;
}

struct cons* cons_new(
    struct state* state,
    struct value* car,
    struct value* cdr
) {
    struct cons* cons = (struct cons*)alloc_object(state, OBJECT_TYPE_CONS, sizeof(struct cons));
    value_copy(&cons->car, car ? car : &_nil);
    value_copy(&cons->cdr, cdr ? cdr : &_nil);
    return cons;
}

#include "sstate.h"

status_t state_init(struct state* state) {
    state->jumplist = NULL;
    return STATUS_OK;
}

status_t state_push_object(struct state* state, struct object* object);

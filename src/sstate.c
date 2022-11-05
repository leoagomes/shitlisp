#include "sstate.h"

status_t state_init(struct state* state) {
    state->_type = OBJECT_TYPE_STATE;
    state->jumplist = NULL;
    return STATUS_OK;
}

status_t state_push_object(struct state* state, struct object* object);

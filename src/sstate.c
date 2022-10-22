#include "state.h"

status_t state_init(struct state* state) {
    state->jumplist = NULL;
    return STATUS_OK;
}


#ifndef __SSTATE_H__
#define __SSTATE_H__

#include "stypes.h"

#define is_main_state(state) ((state)->global->main_state == (state))

status_t state_init(struct state* state);

status_t state_push_object(struct state* state, struct object* object);

#endif

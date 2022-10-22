#ifndef __SSTATE_H__
#define __SSTATE_H__

#include "sobject.h"

#define is_main_state(state) ((state)->global->main_state == (state))

status_t state_init(struct state* state);

#endif

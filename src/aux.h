#ifndef __AUX_H__
#define __AUX_H__

#include "object.h"

struct state* bootstrap_state();
void destroy_state(struct state* state);

#endif

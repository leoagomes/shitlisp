#ifndef __SAUX_H__
#define __SAUX_H__

#include <stdio.h>

#include "stypes.h"

struct state* bootstrap_state();
void destroy_state(struct state* state);

#endif

#ifndef __SRUNTIME_H__
#define __SRUNTIME_H__

#include "stypes.h"

status_t run_protected_callback(struct state* state, generic_callback callback, void* ud);
status_t fail(struct state* state, status_t status);

#endif

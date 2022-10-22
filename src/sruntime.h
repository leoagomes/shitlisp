#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include "types.h"
#include "object.h"

status_t run_protected_callback(struct state* state, generic_callback callback, void* ud);
status_t fail(struct state* state, status_t status);

#endif

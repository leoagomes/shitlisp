#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

#include "runtime.h"

status_t run_protected_callback(
    struct state* state,
    generic_callback callback,
    void* ud
) {
    struct jumplist j;

    j.next = state->jumplist;
    state->jumplist = &j;

    int result = setjmp(j.buf);
    if (result == 0) {
        result = callback(state, ud);
    }
    state->jumplist = j.next;
    return result;
}

status_t fail(struct state* state, status_t status) {
    if (!state->jumplist) {
        fprintf(stderr, "panic! unhandled error %d\n", status);
        exit(status);
    }
    longjmp(state->jumplist->buf, status);
    return status;
}

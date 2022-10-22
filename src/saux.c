#include <stdlib.h>

#include "aux.h"
#include "gc.h"
#include "global.h"
#include "state.h"

struct state* bootstrap_state() {
    struct global* global = global_new();
    if (global == NULL) return NULL;

    struct state* state = malloc(sizeof(*state));
    if (state == NULL) {
        global_free(global);
        return NULL;
    }
    if (state_init(state) != STATUS_OK) {
        free(state);
        global_free(global);
        return NULL;
    }

    objl_close(&global->gc.object_list);
    state->global = global;
    global->main_state = state;
    objl_append((struct object*)state, &global->gc.root_list);

    return state;
}

void destroy_state(struct state* state) {
    // TODO:
}

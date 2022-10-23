#include <stdlib.h>

#include "saux.h"
#include "sgc.h"
#include "sglobal.h"
#include "sstate.h"

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

void print_value(struct value* v, FILE* f) {
    switch (v->type) {
    case VALUE_TYPE_INT:
        fprintf(f, "%d", v->value.integer);
        break;
    case VALUE_TYPE_DOUBLE:
        fprintf(f, "%lf", v->value.number);
        break;
    case VALUE_TYPE_NIL:
        fprintf(f, "null");
        break;
    case VALUE_TYPE_OBJECT:
        print_object(v->value.object, f);
        break;
    default:
        fprintf(f, "#E<unknown value type>");
        break;
    }
}

void print_object(struct object* obj, FILE* f) {
    if (obj == NULL) {
        fprintf(f, "#E<null object>");
        return;
    }

    switch (obj->_type) {
    case OBJECT_TYPE_STRING:
        fprintf(f, "\"%s\"", ((struct string*)obj)->data);
        break;
    case OBJECT_TYPE_SYMBOL:
        fprintf(f, ":%s", ((struct symbol*)obj)->text);
        break;
    case OBJECT_TYPE_LIST:
        {
            struct list* node = (struct list*)obj;

            fprintf(f, "(");
            while (node != NULL) {
                print_value(&node->car, f);

                if (value_is_nil(&node->cdr))
                    break;

                if (!value_is_list(&node->cdr)) {
                    fprintf(f, " . ");
                    print_value(&node->cdr, f);
                    fprintf(f, ")");
                    break;
                }

                fprintf(f, " ");
                node = node->cdr.value.object_as.list;
            }
            fprintf(f, ")");
        }
        break;
    default:
        fprintf(f, "#E<unknown object type>");
        break;
    }
}
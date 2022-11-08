#include "sprinter.h"

void print_value(struct state* _state, struct value* v, FILE* f) {
    switch (v->type) {
    case VALUE_TYPE_INT:
        fprintf(f, "%d", v->value.integer);
        break;
    case VALUE_TYPE_DOUBLE:
        fprintf(f, "%lf", v->value.number);
        break;
    case VALUE_TYPE_NIL:
        fprintf(f, "'()");
        break;
    case VALUE_TYPE_OBJECT:
        print_object(_state, v->value.object, f);
        break;
    case VALUE_TYPE_TRUE:
        fprintf(f, "#t");
        break;
    case VALUE_TYPE_FALSE:
        fprintf(f, "#f");
        break;
    default:
        fprintf(f, "#E<unknown value type>");
        break;
    }
}

void print_object(struct state* _state, struct object* obj, FILE* f) {
    if (obj == NULL) {
        fprintf(f, "#E<null object>");
        return;
    }

    switch (obj->_type) {
    case OBJECT_TYPE_STRING:
        fprintf(f, "\"%s\"", ((struct string*)obj)->data);
        break;
    case OBJECT_TYPE_SYMBOL:
        fprintf(f, "%s", ((struct symbol*)obj)->text);
        break;
    case OBJECT_TYPE_CONS:
        {
            struct cons* node = (struct cons*)obj;

            fprintf(f, "(");
            while (node != NULL) {
                print_value(_state, &node->car, f);

                if (value_is_nil(&node->cdr))
                    break;

                if (!value_is_list(&node->cdr)) {
                    fprintf(f, " . ");
                    print_value(_state, &node->cdr, f);
                    break;
                }

                fprintf(f, " ");
                node = node->cdr.value.object_as.cons;
            }
            fprintf(f, ")");
        }
        break;
    default:
        fprintf(f, "#E<unknown object type>");
        break;
    }
}

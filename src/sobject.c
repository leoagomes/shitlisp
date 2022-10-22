#include "sobject.h"

void print_value(struct value* v, FILE* f) {
    switch (v->type) {
    case VALUE_TYPE_INT:
        fprintf(f, "%d", v->v.i);
        break;
    case VALUE_TYPE_DOUBLE:
        fprintf(f, "%lf", v->v.d);
        break;
    case VALUE_TYPE_NIL:
        fprintf(f, "null");
        break;
    case VALUE_TYPE_OBJECT:
        print_object(v->v.object.obj, f);
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
        fprintf(f, ":%s", ((struct symbol*)obj)->name);
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
                node = node->cdr.v.object.list;
            }
            fprintf(f, ")");
        }
        break;
    default:
        fprintf(f, "#E<unknown object type>");
        break;
    }
}

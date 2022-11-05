// // I told you this was shit.
// //
// // for original cheap copy of lua's implementation, see:
// //   https://github.com/leoagomes/europa/blob/master/src/table.c
// #include "smap.h"

// #include "sgc.h"
// #include <stdlib.h>
// #include <math.h>
// #include <limits.h>

// #define twoto(x) (1 << (x))

// static struct map_node _dummy = {
//     .key = NIL_VALUE,
//     .value = NIL_VALUE,
//     .next = -1,
// };

// int ceil_log2(unsigned int length)
// {
//     static const unsigned char log_2[256] = {/* log_2[i] = ceil(log2(i - 1)) */
//         0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//         6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
//         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
//         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
//         8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
//         8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
//         8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
//         8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
//     };
//     int l = 0;

//     length--;
//     while (length >= 256)
//     {
//         l += 8;
//         length >>= 8;
//     }
//     return l + log_2[length];
// }

// void set_nodes_length(struct state *state, struct map *map, size_t length)
// {
//     if (length == 0) {
//         if ((map->nodes != &_dummy) && map->free != NULL)
//             free_object(state, map->nodes);

//         map->free = NULL;
//         map->nodes = &_dummy;
//         map->logsize = 0;
//         return;
//     }

//     int logsize = ceil_log2(length);

//     // TODO: check for overflows

//     map->logsize = (unsigned char)logsize;
//     size_t size = twoto(map->logsize);

//     map->nodes = palloc(state, sizeof(struct map_node) * size);
//     map->free = &(map->nodes[size]);
//     for (struct map_node* node = map->nodes; node != map->free; node++) {
//         node->key = NIL_VALUE;
//         node->value = NIL_VALUE;
//         node->next = node - map->nodes +  1; // TODO: CHECK
//     }
// }

// struct map *new_map(struct state *state, size_t count)
// {
//     struct map *map = (struct map *)alloc_object(
//         state,
//         OBJECT_TYPE_MAP,
//         sizeof(struct map));
//     map->free = NULL;
//     map->nodes = &_dummy;
//     set_nodes_length(state, map, count);
//     map->meta = NULL;
//     map->count = 0;
//     return map;
// }

// int mark_map(struct state* state, struct map* map) {
//     int i;
//     size_t len;
//     struct value* v;
//     struct map_node *node;

//     len = twotto(map->logsize);
//     for (i = 0; i < len; i++) {
//         node = &(map->nodes[i]);

//         if (node->key.type != VALUE_TYPE_NIL) {
//             mark_value(state, &node->key);
//             mark_value(state, &node->value);
//         }
//     }

//     mark_object(state, (struct object*)map->meta);
//     return 0;
// }

// void free_map(struct state* state, struct map* map) {
//     if (map->nodes != &_dummy) {
//         free_object(state, map->nodes);
//     }
// }

// struct value* map_get_symbol(
//     struct state* state,
//     struct map* map,
//     struct symbol* symbol
// ) {
//     //
// }

// #define get_node(map, index) (&((map)->nodes[(index)]))
// #define node_count(map) (twoto((map)->logsize))
// #define hash_mod(map, node) (get_node(map, ((node) % ((node_count(map) - 1) | 1))))
// #define hash_pointer(map, pointer) (hash_mod(map, (unsigned int)((size_t)(pointer) & UINT_MAX)))

// static struct map_node* hash_int(struct map* map, int key) {
//     unsigned int ui = (unsigned int)key;
//     if (ui <= (unsigned int)INT_MAX)
//         return hash_mod(map, (int)ui);

//     return hash_mod(map, ui);
// }

// static int hash_float(double number) {
//     int i;
//     int ni;

//     number = frexp(number, &i) * -(double)INT_MIN;
//     if (number > (double)INT_MAX || number < (double)INT_MIN)
//         return 0;

//     ni = (int)number;
//     unsigned int u = (unsigned int)i + (unsigned int)ni;
//     return  (int)(u <=(unsigned int)INT_MAX ? u : ~u);
// }

// static struct map_node* node_main_position(struct map* map, struct value* key) {
//     switch (key->type) {
//         case VALUE_TYPE_INT: {
//             int i = key->value.integer;
//             return hash_int(map, i);
//         }
//         case VALUE_TYPE_DOUBLE: {
//             double number = key->value.number;
//             return hash_mod(map, hash_float(number));
//         }
//         case VALUE_TYPE_POINTER: {
//             return hash_pointer(map, key->value.pointer);
//         }
//         case VALUE_TYPE_OBJECT: {
//             switch (key->value.object->_type) {
//                 case OBJECT_TYPE_SYMBOL: {
//                     return hash_pointer(map, key->value.object);
//                 }
//                 default: {
//                     return hash_pointer(map, key->value.object);
//                 }
//             }
//         }
//     }
// }

// struct value* get_generic(
//     struct state* state,
//     struct map* map,
//     struct value* key
// ) {
//     //
// }

// struct value* map_get(
//     struct state* state,
//     struct map* map,
//     struct value* key
// ) {
//     if (!state || !map || !key)
//         return NULL; // TODO: fail?

//     if (value_is_symbol(key)) 
//         return map_get_symbol(state, map, key->value.object_as.symbol);

//     // getgeneric
// }

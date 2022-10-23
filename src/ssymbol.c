#include <string.h>
#include "utf8.h"

#include "sglobal.h"
#include "ssymbol.h"
#include "sobject.h"
#include "sgc.h"

#define RED 'r'
#define BLACK 'b'

#define stree_root(state) ((state)->global->symbols)

/* I'm doing this just because I think it looks cool. */
struct symbol* stree_search(struct symbol* root, const char* text);
struct symbol* stree_insert(struct state* state, struct symbol* root, const char* text, size_t length);
void stree_remove(struct symbol* root, struct symbol* node);
void stree_fixup(struct symbol** root, struct symbol* node);

struct symbol* symbol_new(struct state* state, const char* text, size_t length) {
    struct symbol* symbol = (struct symbol*)alloc_object(
        state,
        OBJECT_TYPE_SYMBOL,
        sizeof(struct symbol) + length + 1
    );
    strncpy(symbol->text, text, length);
    symbol->text[length] = '\0';
    symbol->length = length;
    symbol->parent = symbol->left = symbol->right = NULL;
    symbol->color = 'b';
    return symbol;
}

void symbolv_from_c_string(
    struct state* state,
    const char* text,
    struct value* dst
) {
    dst->value.object_as.symbol = symbol_from_c_string(state, text);
    dst->type = VALUE_TYPE_OBJECT;
}

struct symbol* symbol_from_c_string(struct state* state, const char* text) {
    size_t length = strlen(text);
    return stree_insert(state, stree_root(state), text, length);
}

/* --- symbol tree --- */

struct symbol* stree_search(struct symbol* root, const char* text) {
    if (root == NULL) return NULL;

    // TODO: make this a loop
    int cmp = utf8cmp(text, root->text);
    if (cmp < 0)
        return stree_search(root->left, text);
    else if (cmp > 0)
        return stree_search(root->right, text);

    return root;
}

struct symbol* bst_insert(
    struct state* state,
    struct symbol* node,
    const char* text,
    size_t length
) {
    if (node == NULL) return symbol_new(state, text, length);

    int cmp = utf8cmp(text, node->text);
    if (cmp < 0) {
        node->left = bst_insert(state, node->left, text, length);
        node->left->parent = node;
    } else if (cmp > 0) {
        node->right = bst_insert(state, node->right, text, length);
        node->right->parent = node;
    } else {
        return node;
    }
}

struct symbol* stree_insert(
    struct state* state,
    struct symbol* node,
    const char* text,
    size_t length
) {
    struct symbol* symbol = bst_insert(state, node, text, length);
    // check for existing symbol
    if (
        symbol->parent != NULL ||
        symbol == stree_root(state)
    ) return symbol;

    if (stree_root(state) == NULL) {
        stree_root(state) = symbol;
        return symbol;
    }

    stree_fixup(&stree_root(state), symbol);
    return symbol;
}

void rotate_right(struct symbol** root, struct symbol* node);
void rotate_left(struct symbol** root, struct symbol* node);

void stree_fixup(struct symbol** root, struct symbol* node) {
    while(node->color == RED) {
        if (node->parent == node->parent->parent->left) {
            struct symbol* y = node->parent->parent->right;

            if (y->color == RED) {
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotate_left(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_right(root, node->parent->parent);
            }
        } else {
            struct symbol* y = node->parent->parent->left;

            if (y->color == RED) {
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotate_right(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_left(root, node->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void rotate_right(struct symbol** root, struct symbol* node) {
    struct symbol* left = node->left;
    node->left = left->right;

    if (left->right != NULL)
        left->right->parent = node;
    left->parent = node->parent;
    if (node->parent == NULL)
        *root = left;
    else if (node == node->parent->right)
        node->parent->right = left;
    else
        node->parent->left = left;


    left->right = node;
    node->parent = left;
}

void rotate_left(struct symbol** root, struct symbol* node) {
    struct symbol* right = node->right;
    node->right = right->left;

    if (right->left != NULL)
        right->left->parent = node;
    right->parent = node->parent;
    if (node->parent == NULL)
        *root = right;
    else if (node == node->parent->left)
        node->parent->left = right;
    else
        node->parent->right = right;

    right->left = node;
    node->parent = right;
}

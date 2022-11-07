#include "utf8.h"

#include "sparser.h"
#include "ssymbol.h"
#include "scons.h"

struct parser {
    struct state* state;
    char* source;
    int position, line, column;
};

struct token {
    enum token_type {
        TOK_SYMBOL,
        TOK_PARENS,
    } type;
    const char* value;
    int length, line, column, postion;
};

status_t pread(struct parser* parser, struct value* dst);

// "API" implementation
status_t read_cstr(struct state* state, char* source, struct value* dst) {
    struct parser parser = {state, source, 0};
    return pread(&parser, dst);
}

// parser implementation

#define pchar(p) ((p)->source[(p)->position])
#define poffchar(p,o) ((p)->source[(p)->position + (o)])

#define is_whitespace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define is_newline(c) (c == '\n')
#define is_end(c) ((c) == '\0')
#define is_identifier_special_initial(c) ( \
    (c) == '!' || (c) == '$' || (c) == '%' || (c) == '&' || (c) == '*' || \
    (c) == '/' || (c) == ':' || (c) == '<' || (c) == '=' || (c) == '>' || \
    (c) == '?' || (c) == '^' || (c) == '_' || (c) == '~' \
)

#define is_identifier_initial(c) \
    (isalpha(c) || is_identifier_special_initial(c))
#define is_explicit_sign(c) (c == '+' || c == '-')
#define is_identifier_subsequent(c) ( \
    is_identifier_initial(c) || isdigit(c) || is_explicit_sign(c) || \
    (c) == '.' || (c) == '@' \
)
#define is_hash(c) ((c) == '#')
#define is_delimiter(c) ( \
    is_whitespace(c) || is_end(c) || is_newline(c) || \
    (c) == '(' || (c) == ')' || \
    (c) == '"' || \
    (c) == ';' \
)

static void padvance(struct parser* parser) {
    char c = pchar(parser);
    if (is_end(c)) return;

    parser->position++;
    if (is_newline(c)) {
        parser->line++;
        parser->column = 0;
    } else {
        parser->column++;
    }
}

static void skip_whitespace(struct parser* parser) {
    char *c = &pchar(parser);
    while (
        !is_end(*c) &&
        is_whitespace(*c)
    ) {
        if (is_newline(*c)) {
            parser->line++;
            parser->column = 0;
        }

        c++; // huh
        padvance(parser);
    }
}

status_t read_list(struct parser* parser, struct value* dst) {
    if (pchar(parser) != '(') return STATUS_INVALID;
    padvance(parser);

    skip_whitespace(parser);

    if (pchar(parser) == ')') {
        padvance(parser);
        dst->type = VALUE_TYPE_NIL;
        dst->value.object = NULL;
        return STATUS_OK;
    }

    struct value* next_cell = dst;
    do {
        next_cell->type = VALUE_TYPE_OBJECT;
        next_cell->value.object_as.cons = cons_new(parser->state, NULL, NULL);
        status_t result = pread(parser, &next_cell->value.object_as.cons->car);
        if (result != STATUS_OK) return result;

        skip_whitespace(parser);

        if (pchar(parser) == '.' && is_whitespace(poffchar(parser, 1))) {
            padvance(parser);
            skip_whitespace(parser);

            result = pread(parser, &next_cell->value.object_as.cons->cdr);
            if (result != STATUS_OK) return result;

            skip_whitespace(parser);

            if (pchar(parser) != ')') return STATUS_INVALID;
        }
        next_cell = &next_cell->value.object_as.cons->cdr;
    } while (!is_end(pchar(parser)) && pchar(parser) != ')');
    padvance(parser);

    return STATUS_OK;
}

status_t read_symbol(struct parser* parser, struct value* dst) {
    char* symbol_str = &pchar(parser);
    int symbol_len = 0;

    if (!is_identifier_initial(*symbol_str)) {
        return STATUS_INVALID;
    }
    symbol_len++;
    padvance(parser);

    while (
        !is_end(pchar(parser)) &&
        is_identifier_subsequent(pchar(parser))
    ) {
        padvance(parser);
        symbol_len++;
    }

    symbolv_from_c_string(parser->state, symbol_str, symbol_len, dst);
    return STATUS_OK;
}

status_t read_hash_expression(struct parser* parser, struct value* dst) {
    if (pchar(parser) != '#') return STATUS_INVALID;
    padvance(parser);

    if (pchar(parser) == 't' && is_delimiter(poffchar(parser, 1))) {
        padvance(parser);
        dst->type = VALUE_TYPE_TRUE;
        return STATUS_OK;
    } else if (pchar(parser) == 'f' && is_delimiter(poffchar(parser, 1))) {
        padvance(parser);
        dst->type = VALUE_TYPE_FALSE;
        return STATUS_OK;
    }
}

status_t pread(struct parser* parser, struct value* dst) {
    skip_whitespace(parser);

    char c = pchar(parser);

    if (is_end(c)) {
        *dst = (struct value) { .type = VALUE_TYPE_NIL };
        return STATUS_OK;
    }

    if (is_identifier_initial(c)) {
        return read_symbol(parser, dst);
    }

    if (c == '(') {
        return read_list(parser, dst);
    }

    if (is_hash(c)) {
        return read_hash_expression(parser, dst);
    }

    return STATUS_INVALID;
}

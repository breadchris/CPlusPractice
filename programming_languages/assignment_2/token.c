#include "token.h"

char *keywords[] = {
    "for", "while", "if", "else"
};
char *operators[] = {
"!", "%", "&", "|", "+", "-", "*", "/", "{",
"}", "[", "]", ";", ",", "<", ">", "=", "<=",
">=", "!=", ":=", "(", ")"
};

const uint8_t is_letter[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const uint8_t is_digit[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const uint8_t is_operator[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const uint8_t is_alpha[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};

char *read_lines(size_t *line_len)
{
    char *buf, *lines;
    int read_size = 1024, bytes_read, cur_len = 0;
    bool done = false;

    lines = (char *)scalloc(1, 64);
    buf = (char *)scalloc(1, read_size);

    do {
        memset(buf, '\0', read_size);
        bytes_read = read(STDIN, buf, read_size);
        if (bytes_read <= 0) {
            break;
        }
        lines = realloc(lines, cur_len + bytes_read);
        memcpy(lines + cur_len, buf, bytes_read);
        cur_len += bytes_read;
    } while (1);

    *line_len = cur_len;
    return lines;
}

void free_tokens(struct token **tokens, size_t token_count)
{
    size_t i;
    struct token *t;
    for (i = 0; i < token_count; ++i) {
        t = tokens[i];
        if (t->value != NULL) free(t->value);
        free(t);
    }
    free(tokens);
}

void print_token(struct token *t) {
    printf("TID: %zu, TYPE: %zu(%s), POS: %zu, LEN: %zu",
            t->id, t->numeric_type, t->english_type, t->pos,
            t->len);
    if (t->value != NULL) {
        printf(", VALUE: %s\n", t->value);
    } else {
        printf("\n");
    }
}

void print_tokens(struct token **tokens, size_t token_count, size_t line_size, int token_level)
{
    /* Example output:
     * TID: 1, TYPE: 3(INT), POS: 0, LEN: 1, VALUE: 1
     * TID: 2, TYPE: 6(NEWLINE), POS: 1, LEN: 1
     * TID: 3, TYPE: 1(ID), POS: 2, LEN: 1, VALUE: x
     * TID: 4, TYPE: 6(NEWLINE), POS: 3, LEN: 1
     * Totals: len = 4, tokens = 4, printed = 4
     * */
    size_t i, num_printed;
    struct token *t;
    bool output = true, is_cwen;
    if (-1 == token_level) {
        printf("TID: 1, TYPE: 97(ERR1), POS: 0, LEN: 0\n");
        printf("\nTotals: len = 0, tokens = 1, printed = 1\n");
        return;
    }
    for (num_printed = 0, i = 0; i < token_count; ++i, output = true) {
        t = tokens[i];
        is_cwen = N_COMMENT == t->numeric_type || \
                  N_WS      == t->numeric_type || \
                  N_ERR1    == t->numeric_type || \
                  N_ERR2    == t->numeric_type || \
                  N_ERR3    == t->numeric_type || \
                  N_ERR4    == t->numeric_type || \
                  N_NEWLINE == t->numeric_type;
        switch (token_level) {
            case 0:
                output = true; break;
            case 1:
                if (is_cwen) output = false; break;
            case 2:
                if (!is_cwen) output = false; break;
            default:
                log_err("Shouldn't be here");
        }
        if (!output) continue;

        print_token(t);

        ++num_printed;
    }
    printf("\nTotals: len=%zu, tokens=%zu, printed=%zu\n",
            line_size, token_count, num_printed);
}

void read_string(char *line, size_t len, size_t *i, struct token *t)
{
    size_t pos;
    char c;
    bool in_string = false;

    pos = *i;
    t->pos = pos;
    t->numeric_type = N_STR;
    t->english_type = S_STR;
    for (; pos < len; ++pos) {
        c = line[pos];
        if (!is_alpha[c]) {
            break;
        } else if ('"' == c) {
            if (!in_string) {
                in_string = true;
                continue;
            } else {
                in_string = false;
                ++pos;
                break;
            }
        }
    }
    t->len = pos - *i;
    if (pos == len || in_string) {
        //Reached end without finishing token
        //ERR2
        t->numeric_type = N_ERR2;
        t->english_type = S_ERR2;
        t->value = NULL;
    } else {
        t->value = sstrndup(line + t->pos + 1, t->len - 2);
    }
    *i = pos - 1;
}

void read_number(char *line, size_t len, size_t *i, struct token *t)
{
    bool is_float = false;
    size_t pos;
    char c;

    pos = *i;
    t->pos = pos;
    for (; pos < len; ++pos) {
        c = line[pos];
        if ('.' == c && !is_float) {
            is_float = true;
            continue;
        }
        if (!is_digit[c]) {
            break;
        }
    }
    t->len = pos - *i;

    t->value = sstrndup(line + t->pos, t->len);
    if (!is_float) {
        t->numeric_type = N_INT;
        t->english_type = S_INT;
    } else {
        t->numeric_type = N_FLOAT;
        t->english_type = S_FLOAT;
    }
    *i = pos - 1;
}

void read_comment(char *line, size_t len, size_t *i, struct token *t)
{
    size_t pos;
    char c;

    pos = *i;
    t->pos = pos;
    for (; pos < len; ++pos) {
        c = line[pos];
        if (!is_alpha[c] ) {
            break;
        }
    }
    t->len = pos - *i;

    t->value = sstrndup(line + t->pos, t->len);
    t->numeric_type = N_COMMENT;
    t->english_type = S_COMMENT;
    *i = pos - 1;
}

void read_identifier(char *line, size_t len, size_t *i, struct token *t)
{
    size_t pos;
    char c;

    pos = *i;
    t->pos = pos;
    for (; pos < len; ++pos) {
        c = line[pos];
        if (('_' != c && !is_letter[c] && !is_digit[c]) ||
                (is_digit[c] && (pos - *i == 0))) {
            break;
        }
    }

    t->len = pos - *i;

    t->value = sstrndup(line + t->pos, t->len);
    t->numeric_type = N_ID;
    t->english_type = S_ID;
    *i = pos - 1;
}

struct token **tokenizer(char *line, size_t line_len, size_t *tc)
{
    #define ALLOC_TOK(id_) do { \
        tmp = (struct token *)scalloc(1, sizeof(struct token)); \
        tmp->id = id_; \
    } while (0);
    size_t i, token_count, r;
    ssize_t j;
    char c;
    struct token **tokens;
    struct token *tmp;

    tokens = scalloc(line_len, sizeof(struct token *));

    for (token_count = 0, i = 0; i < line_len; ++i) {
        c = line[i];
        if ('\n' == c) {
            //End of line
            ALLOC_TOK(token_count + 1);
            tmp->numeric_type = N_NEWLINE;
            tmp->english_type = S_NEWLINE;
            tmp->pos = i;
            tmp->len = 1;
            tokens[token_count++] = tmp;
            continue;
        }
        if (' ' == c || '\t' == c) {
            //Whitespace
            ALLOC_TOK(token_count + 1);
            j = 0;
            tmp->pos = i;
            for (j = 0; i < line_len &&
                    (' ' == line[i] || '\t' == line[i]);
                    i += 1, j += 1);
            i -= 1;

            tmp->len = j;
            tmp->numeric_type = N_WS;
            tmp->english_type = S_WS;
            tokens[token_count++] = tmp;
            continue;
        }
        if ('#' == c) {
            //Comment
            ALLOC_TOK(token_count + 1);
            read_comment(line, line_len, &i, tmp);
            tokens[token_count++] = tmp;
            continue;
        }
        if (!is_alpha[c]) {
            //Not valid character
            //ERR3
            ALLOC_TOK(token_count + 1);
            j = 0;
            tmp->pos = i;
            while (i < line_len && !is_alpha[line[i]]) { i += 1; j += 1; }
            i -= 1;

            tmp->len = j;
            tmp->numeric_type = N_ERR3;
            tmp->english_type = S_ERR3;
            tokens[token_count++] = tmp;
            continue;
        }
        if ('_' == c || is_letter[c]) {
            bool found_match = false;

            for (j = 0; j < 4; ++j) {
                char *key = keywords[j];
                size_t key_len = strlen(key);
                if (key_len + i < line_len && \
                        0 == strncmp(line + i, key, key_len) && \
                        !is_letter[line[key_len + i]] && \
                        !is_digit[line[key_len + i]]) {
                    ALLOC_TOK(token_count + 1);
                    tmp->numeric_type = N_KEYWORD + j;
                    tmp->english_type = convert_to_upper(key);
                    tmp->pos = i;
                    tmp->len = key_len;
                    tmp->value = sstrndup(key, key_len);
                    tokens[token_count++] = tmp;
                    i += key_len - 1;
                    found_match = true;
                    break;
                }
            }
            if (!found_match) {
                ALLOC_TOK(token_count + 1);
                read_identifier(line, line_len, &i, tmp);
                tokens[token_count++] = tmp;
            }
            continue;
        }
        if ('.' == c || is_digit[c]) {
            ALLOC_TOK(token_count + 1);
            read_number(line, line_len, &i, tmp);
            tokens[token_count++] = tmp;
            continue;
        }
        if (is_operator[c]) {
            if (':' == c && i + 1 < line_len && '=' != line[i + 1]) {
                //Invalid character
                ALLOC_TOK(token_count + 1);
                tmp->pos = i;
                tmp->len = 1;
                tmp->numeric_type = N_ERR4;
                tmp->english_type = S_ERR4;
                tmp->value = NULL;
                tokens[token_count++] = tmp;
                continue;
            }

            for (j = 22; j >= 0; --j) {
                char *oper = operators[j];
                size_t oper_len = strlen(oper);
                if (oper_len + i < line_len && 0 == strncmp(line + i, oper, oper_len)) {
                    ALLOC_TOK(token_count + 1);
                    tmp->numeric_type = N_OP + j;
                    tmp->english_type = convert_to_upper(oper);
                    tmp->pos = i;
                    tmp->len = oper_len;
                    tmp->value = NULL;
                    tokens[token_count++] = tmp;
                    i += oper_len - 1;
                    break;
                }
            }
            continue;
        }
        if ('"' == c) {
            ALLOC_TOK(token_count + 1);
            read_string(line, line_len, &i, tmp);
            tokens[token_count++] = tmp;
            continue;
        }

        //Invalid character (in alphabet, not valid in context)
        ALLOC_TOK(token_count + 1);
        tmp->pos = i;
        tmp->len = 1;
        tmp->numeric_type = N_ERR4;
        tmp->english_type = S_ERR4;
        tmp->value = NULL;
        tokens[token_count++] = tmp;
    } //Done tokenizing loop

    ALLOC_TOK(token_count + 1);
    tmp->numeric_type = N_EMPTY_STR;
    tmp->english_type = "epsilon";
    tmp->value = NULL;
    tokens[token_count++] = tmp;

    #undef ALLOC_TOK
    *tc = token_count;
    return tokens;
}

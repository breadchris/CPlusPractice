#include "parse.h"

//#define log(a, M, ...) for (int i = a; i-- != 0;) fprintf(stdout, "\t"); fprintf(stdout, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log(a, M, ...)
#define depth_print(a, M, ...) for (depth_count = a; depth_count-- != 0;) fprintf(stdout, "\t"); fprintf(stdout, M, ##__VA_ARGS__)

struct token *parse_tree_next_token(struct token **tokens, size_t token_count,
    size_t *pos)
{
    bool is_cwn, is_error;
    struct token *t;

    do {
        if (token_count == ++*pos) {
            --*pos;
            return NULL;
        }
        t = tokens[*pos];

        is_cwn = N_COMMENT == t->numeric_type || \
                  N_WS      == t->numeric_type || \
                  N_NEWLINE == t->numeric_type;
    } while (is_cwn);
    return t;
}

void parse_tree_print_token(struct token *t, size_t depth) {
    if (t->numeric_type == N_EMPTY_STR) {
        depth_print(depth, "<epsilon></epsilon>\n");
    } else {
        if (t->value == NULL) {
            depth_print(depth, "<id>%zu</id><typenum>%zu</typenum><typename>%s</typename><position>%zu</position><length>%zu</length>\n",
                    t->id, t->numeric_type, t->english_type, t->pos,
                    t->len);
        } else {
            depth_print(depth, "<id>%zu</id><typenum>%zu</typenum><typename>%s</typename><position>%zu</position><length>%zu</length><value>%s</value>\n",
                    t->id, t->numeric_type, t->english_type, t->pos,
                    t->len, t->value);
        }
    }
}

void parse_tree_print(struct parse_node *parse_tree, size_t depth)
{
    size_t i;
    struct parse_node *child_node;

    if (parse_tree == NULL) return;

    depth_print(depth, "<node>");
    if (!parse_tree->is_terminal) {
        printf("<nt>%s</nt>", parse_tree->parse_token->english_type);
    } else {
        printf("\n");
        parse_tree_print_token(parse_tree->parse_token, depth + 1);
        depth_print(depth, "</node>\n");
        return;
    }
    printf("<children>\n");
    for (i = 0; i < parse_tree->children_count; i++) {
        parse_tree_print(parse_tree->children[i], depth + 1);
    }
    depth_print(depth + 1, "</children>\n");
    depth_print(depth, "</node>\n");
}

void parse_tree_clear(struct parse_node *parse_tree)
{
    size_t i;
    struct parse_node *child_node;
    for (i = 0; i < parse_tree->children_count; i++) {
        child_node = parse_tree->children[i];
        //log_info("Freeing: %zu:%s", parse_tree->children[i]->parse_token->numeric_type, parse_tree->children[i]->parse_token->english_type);
        if (!child_node->is_terminal) {
            parse_tree_clear(child_node);
            free(child_node->parse_token);
        }
        free(parse_tree->children[i]);
    }
    parse_tree->children_count = 0;
}

size_t parse_tree_match_stmt(struct token **tokens, size_t token_count,
    size_t pos, uint8_t **check_stmt, uint8_t check_stmt_num,
    uint8_t prev_check_stmt_num, struct parse_node *parse_tree, size_t depth)
{
    struct token *t;
    struct parse_node *cnode;
    size_t i, new_pos = 0, save_pos;
    uint8_t *cur_check, *save_cur_check, cur_stmt_check, **token_check;

    if (pos == token_count) {
        // We gucci, we have matched all input
        log_info("All done");
    }
    t = tokens[pos];

    save_pos = pos;

    for (; 0 != (cur_check = *(check_stmt)); check_stmt++) {
        save_cur_check = cur_check;
        for (; 0 != (cur_stmt_check = (*cur_check)); cur_check++) {
            log(depth, "%s:%d %s:%d", STMT_STRING_LOOKUP(cur_stmt_check), cur_stmt_check, STMT_STRING_LOOKUP(check_stmt_num), check_stmt_num);
            if (N_STMT != cur_stmt_check && cur_stmt_check == check_stmt_num && cur_stmt_check == prev_check_stmt_num) {
                // We aint about to fall into an infinite loop
                break;
            }

            token_check = STMT_LOOKUP(cur_stmt_check);
            log(depth, "--> Trying to match... %s [%zu] ", t->english_type, t->numeric_type);
            if (0 == token_check) {
                // We have a terminal
                if (pos != token_count - 1 && pos > 0 && cur_stmt_check == N_EMPTY_STR) {
                    --pos;
                    t = scalloc(1, sizeof(struct token));
                    t->numeric_type = N_EMPTY_STR;
                    t->english_type = S_EMPTY_STR;
                    t->value = NULL;
                }
                if (t->numeric_type == cur_stmt_check) {
                    log(depth, "<-- Got terminal match %s [%s]", t->value, t->english_type);
                    cnode = scalloc(1, sizeof(struct parse_node));
                    cnode->parse_token = t;
                    cnode->children_count = 0;
                    cnode->is_terminal = true;

                    parse_tree->children[parse_tree->children_count++] = cnode;

                    if (pos == token_count - 1) return pos;

                    // Our token is what is expected in stmt check
                    t = parse_tree_next_token(tokens, token_count, &pos);

                    log(depth, "!!! New search token: %s [%zu]", t->english_type, pos);
                } else {
                    // We do not have a token match
                    log(depth, "[%hhu] No terminal match found", cur_stmt_check);
                    break;
                }
            } else {
                // We need to descend parsing tree further
                log(depth, "<-- It is a %s [%hhu]", STMT_STRING_LOOKUP(cur_stmt_check), cur_stmt_check);
                cnode = scalloc(1, sizeof(struct parse_node));
                cnode->parse_token = scalloc(1, sizeof(struct token));
                cnode->parse_token->numeric_type = cur_stmt_check;
                cnode->parse_token->english_type = STMT_STRING_LOOKUP(cur_stmt_check);
                cnode->children = scalloc(16, sizeof(struct parse_node*));
                cnode->children_count = 0;
                cnode->is_terminal = false;

                new_pos = parse_tree_match_stmt(tokens, token_count, pos, token_check, \
                        cur_stmt_check, check_stmt_num, cnode, depth + 1);
                if (pos != 0 && new_pos == 0) break;

                log(depth, "%zu ? %zu", pos, new_pos);
                if (cnode->children_count == 0 \
                    || (cnode->children[cnode->children_count - 1]->parse_token->numeric_type != N_EMPTY_STR \
                    && new_pos == pos)) {
                    log(depth, "[%hhu] No match found %s", cur_stmt_check, STMT_STRING_LOOKUP(check_stmt_num));
                    break;
                } else {
                    log(depth, "[%hhu] Match found %s", cur_stmt_check, STMT_STRING_LOOKUP(check_stmt_num));

                    parse_tree->children[parse_tree->children_count++] = cnode;

                    pos = new_pos;
                    t = tokens[pos];
                }
            }
        }
        if (0 == cur_stmt_check) {
            // We got to end of check, this means we matched it
            log(depth, "[%hhu] Matched... %s", cur_stmt_check, STMT_STRING_LOOKUP(check_stmt_num));
            break;
        } else {
            // We did not get to end of check, something must have not matched
            log(depth, "[%hhu] Did not match... %s", cur_stmt_check, STMT_STRING_LOOKUP(check_stmt_num));
            log(depth, "%zu = %zu", pos, save_pos);

            parse_tree_clear(parse_tree);
            pos = save_pos;
            t = tokens[pos];
            continue;
        }
    }
    if (0 == cur_check) {
        // Did not match any possibilities in given stmt
        log(depth, "Did not match anything\n\n");
        return 0;
    }

    // Matched something in stmt
    log(depth, "Matched something\n\n");
    return pos;
}

struct parse_node *parse_tree_generate(struct token **tokens, size_t token_count)
{
    size_t parse_return;
    struct parse_node *parse_tree;

    parse_tree = scalloc(1, sizeof(struct parse_node));
    parse_tree->parse_token = scalloc(1, sizeof(struct token));
    parse_tree->parse_token->numeric_type = N_STMT;
    parse_tree->parse_token->english_type = S_STMT;
    parse_tree->children = scalloc(8, sizeof(struct parse_node*));
    parse_tree->children_count = 0;
    parse_tree->is_terminal = false;

    parse_return = parse_tree_match_stmt(tokens, token_count, 0, stmt, N_STMT, 255, parse_tree, 0);

    if (parse_return != token_count - 1) {
        printf("Parse error\n");
        return NULL;
    }
    return parse_tree;
}

bool parse_tree_check_tokens(struct token **tokens, size_t token_count)
{
    size_t i;
    bool is_error;
    struct token *t;

    for (i = 0; i < token_count; ++i) {
        t = tokens[i];
        is_error = N_ERR1    == t->numeric_type || \
                   N_ERR2    == t->numeric_type || \
                   N_ERR3    == t->numeric_type || \
                   N_ERR4    == t->numeric_type;

        if (is_error) {
            printf("Lex error\n");
            return false;
        }

        if (N_MOD == t->numeric_type || N_COMMA == t->numeric_type) {
            printf("Unimplemented error\n");
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv, char **envp)
{
    char *input;
    struct token **tokens;
    size_t line_len, token_count;

    input = read_lines(&line_len);
    tokens = tokenizer(input, line_len, &token_count);
    //print_tokens(tokens, token_count, line_len, 0);

    if (!parse_tree_check_tokens(tokens, token_count)) {
        exit(0);
    }

    struct parse_node *parse_tree = parse_tree_generate(tokens, token_count);
    parse_tree_print(parse_tree, 0);

    // Cleanup
    // free_tokens(tokens, token_count);
}

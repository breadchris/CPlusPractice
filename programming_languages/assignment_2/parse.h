#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"
#include "token.h"

struct parse_node {
    struct token *parse_token;
    struct parse_node **children;
    size_t children_count;
    bool is_terminal;
};

size_t depth_count;

#define S_STMT          "stmt"
#define S_WHILE_STMT    "while_stmt"
#define S_FOR_STMT      "for_stmt"
#define S_IF_STMT       "if_stmt"
#define S_ASSIGNMENT_STMT   "assign_stmt"
#define S_OUTPUT_STMT   "output_stmt"
#define S_EMPTY_STR     "empty_str"
#define S_COND_EXPR     "cond_expr"
#define S_OPT_ASSIGN    "opt_assign"
#define S_OPT_ELSE      "opt_else"
#define S_EXPR          "expr"
#define S_MULT_EXPR     "mult_expr"
#define S_UNARY_EXPR    "unary_expr"
#define S_PAREN_EXPR    "paren_expr"
#define S_BASIC_EXPR    "basic_expr"
#define S_AND_EXPR      "and_expr"
#define S_EQ_EXPR       "eq_expr"
#define S_CMP_EXPR      "cmp_expr"
#define S_OPT_COND_EXPR "opt_cond_expr"

#define N_STMT              101
#define N_WHILE_STMT        102
#define N_FOR_STMT          103
#define N_IF_STMT           104
#define N_ASSIGNMENT_STMT   105
#define N_OUTPUT_STMT       106
#define N_NOTHING           107
#define N_COND_EXPR         108
#define N_OPT_ASSIGN        109
#define N_OPT_ELSE          110
#define N_EXPR              111
#define N_MULT_EXPR         112
#define N_UNARY_EXPR        113
#define N_PAREN_EXPR        114
#define N_BASIC_EXPR        115
#define N_AND_EXPR          116
#define N_EQ_EXPR           117
#define N_CMP_EXPR          118
#define N_OPT_COND_EXPR     110

uint8_t empty_str[] = {N_EMPTY_STR, 0};
uint8_t *empty_str_stmt[] = {empty_str, 0};

/* Stmt specification
<stmt> ::= <while_stmt> <stmt>
 | <for_stmt> <stmt>
 | <if_stmt> <stmt>
 | <assignment_stmt> ';' <stmt>
 | <output_stmt> <stmt>
 | ε
*/
uint8_t stmt_while[] = {N_WHILE_STMT, N_STMT, 0};
uint8_t stmt_for[] = {N_FOR_STMT, N_STMT, 0};
uint8_t stmt_if[] = {N_IF_STMT, N_STMT, 0};
uint8_t stmt_assign[] = {N_ASSIGNMENT_STMT, N_SEMI, N_STMT, 0};
uint8_t stmt_output[] = {N_OUTPUT_STMT, N_STMT, 0};

uint8_t *stmt[] = {stmt_while, stmt_for, stmt_if, stmt_assign, stmt_output, \
                    empty_str, 0};

/*
<while_stmt> ::= 'while' '(' <cond_expr> ')' '{' <stmt> '}'
*/
uint8_t while_stmt_spec[] = {N_WHILE, N_O_PAREN, N_COND_EXPR, N_C_PAREN, \
                         N_O_BRK, N_STMT, N_C_BRK, 0};

uint8_t *while_stmt[] = {while_stmt_spec, 0};

/*
<for_stmt> ::= 'for' '(' <opt_assign> ';' <opt_cond_expr>
 ';' <opt_assign> ')' '{' <stmt> '}'
*/
uint8_t for_stmt_spec[] = {N_FOR, N_O_PAREN, N_OPT_ASSIGN, N_SEMI, N_OPT_COND_EXPR, \
                         N_C_PAREN, N_SEMI, N_OPT_ASSIGN, N_C_PAREN, N_O_BRK, N_STMT, N_C_BRK, 0};

uint8_t *for_stmt[] = {for_stmt_spec, 0};

/*
<if_stmt> ::= 'if' '(' <cond_expr> ')' '{' <stmt> '}' <opt_else>
*/
uint8_t if_stmt_spec[] = {N_IF, N_O_PAREN, N_COND_EXPR, N_C_PAREN, \
                         N_O_BRK, N_STMT, N_C_BRK, N_OPT_ELSE, 0};
uint8_t *if_stmt[] = {if_stmt_spec, 0};

/*
<opt_else> ::= 'else' '{' <stmt> '}'
 | ε
*/
 uint8_t opt_else_stmt_spec[] = {N_ELSE, N_O_BRK, N_STMT, N_C_BRK, 0};
 uint8_t *opt_else_stmt[] = {opt_else_stmt_spec, empty_str, 0};

/*
<assignment_stmt> ::= ID ':=' <expr>
*/
uint8_t assignment_stmt_spec[] = {N_ID, N_ASSIGN, N_EXPR, 0};
uint8_t *assignment_stmt[] = {assignment_stmt_spec, 0};

/*
<output_stmt> ::= '[' <expr> ']' ';'
*/
uint8_t output_stmt_spec[] = {N_O_SBRK, N_EXPR, N_C_SBRK, N_SEMI, 0};
uint8_t *output_stmt[] = {output_stmt_spec, 0};

/*
<expr> ::= <expr> '+' <mult_expr>
 | <mult_expr>
*/
uint8_t expr_spec[] = {N_EXPR, N_ADD, N_MULT_EXPR, 0};
uint8_t expr_base_spec[] = {N_MULT_EXPR, 0};
uint8_t *expr_stmt[] = {expr_spec, expr_base_spec, 0};

/*
<mult_expr> ::= <mult_expr> '*' <unary_expr>
 | <unary_expr>
*/
uint8_t mult_expr_spec[] = {N_MULT_EXPR, N_MULT, N_UNARY_EXPR, 0};
uint8_t mult_expr_base_spec[] = {N_UNARY_EXPR, 0};
uint8_t *mult_expr_stmt[] = {mult_expr_spec, mult_expr_base_spec, 0};

/*
<unary_expr> ::= '!' <expr>
 | '+' <expr>
 | '-' <expr>
 | <paren_expr>
*/
uint8_t unary_expr_not_spec[] = {N_NOT, N_EXPR, 0};
uint8_t unary_expr_add_spec[] = {N_ADD, N_EXPR, 0};
uint8_t unary_expr_sub_spec[] = {N_SUB, N_EXPR, 0};
uint8_t unary_expr_base_spec[] = {N_PAREN_EXPR, 0};
uint8_t *unary_expr_stmt[] = {unary_expr_not_spec, unary_expr_add_spec, \
                            unary_expr_sub_spec, unary_expr_base_spec, 0};

/*
<paren_expr> ::= '(' <expr> ')'
 | <basic_expr>
*/
uint8_t paren_expr_expr_spec[] = {N_O_PAREN, N_EXPR, N_C_PAREN, 0};
uint8_t paren_expr_base_spec[] = {N_BASIC_EXPR, 0};
uint8_t *paren_expr_stmt[] = {paren_expr_expr_spec, paren_expr_base_spec, 0};

/*
<basic_expr> ::= ID
 | STR
 | INT
 | FLOAT
*/
uint8_t basic_expr_ID[] = {N_ID, 0};
uint8_t basic_expr_STR[] = {N_STR, 0};
uint8_t basic_expr_INT[] = {N_INT, 0};
uint8_t basic_expr_FLOAT[] = {N_FLOAT, 0};
uint8_t *basic_expr_stmt[] = {basic_expr_ID, basic_expr_STR, basic_expr_INT, \
                                basic_expr_FLOAT, 0};

/*
<cond_expr> ::= <cond_expr> '|' <and_expr>
 | <and_expr>
*/
uint8_t cond_expr_OR_spec[] = {N_COND_EXPR, N_OR, N_AND_EXPR, 0};
uint8_t cond_expr_base_spec[] = {N_AND_EXPR, 0};
uint8_t *cond_expr_stmt[] = {cond_expr_OR_spec, cond_expr_base_spec, 0};

/*
<and_expr> ::= <and_expr> '&' <cmp_expr>
 | <eq_expr>
*/
uint8_t and_expr_AND_spec[] = {N_AND_EXPR, N_AND, N_CMP_EXPR, 0};
uint8_t and_expr_base_spec[] = {N_EQ_EXPR, 0};
uint8_t *and_expr_stmt[] = {and_expr_AND_spec, and_expr_base_spec, 0};

/*
<eq_expr> ::= <eq_expr> '=' <cmp_expr>
 | <eq_expr> '!=' <cmp_expr>
 | <cmp_expr>
*/
uint8_t eq_expr_equal_spec[] = {N_EQ_EXPR, N_EQUAL, N_CMP_EXPR, 0};
uint8_t eq_expr_not_equal_spec[] = {N_EQ_EXPR, N_NOT_EQUAL, N_CMP_EXPR, 0};
uint8_t eq_expr_base_spec[] = {N_CMP_EXPR, 0};
uint8_t *eq_expr_stmt[] = {eq_expr_equal_spec, eq_expr_not_equal_spec, \
                            eq_expr_base_spec, 0};

/*
<cmp_expr> ::= <cmp_expr> '<' <expr>
 | <cmp_expr> '>' <expr>
 | <cmp_expr> '<=' <expr>
 | <cmp_expr> '>=' <expr>
 | <expr>
*/
uint8_t cmp_expr_lthan_spec[] = {N_CMP_EXPR, N_LESS_THAN, N_EXPR, 0};
uint8_t cmp_expr_gthan_spec[] = {N_CMP_EXPR, N_GREAT_THAN, N_EXPR, 0};
uint8_t cmp_expr_lequal_spec[] = {N_CMP_EXPR, N_LESS_EQUAL, N_EXPR, 0};
uint8_t cmp_expr_gequal_spec[] = {N_CMP_EXPR, N_GREAT_EQUAL, N_EXPR, 0};
uint8_t cmp_expr_base_spec[] = {N_EXPR, 0};
uint8_t *cmp_expr_stmt[] = {cmp_expr_lthan_spec, cmp_expr_gthan_spec, \
                            cmp_expr_lequal_spec, cmp_expr_gequal_spec, \
                            cmp_expr_base_spec, 0};

/*
<opt_cond_expr> ::= <cond_expr>
 | ε
*/
uint8_t opt_cond_expr_cond_spec[] = {N_COND_EXPR, 0};
uint8_t opt_cond_expr_base_spec[] = {N_EMPTY_STR, 0};
uint8_t *opt_cond_expr_stmt[] = {opt_cond_expr_cond_spec, \
                                    opt_cond_expr_base_spec, 0};

/*
<opt_assign> ::= <assignment_stmt>
 | ε
*/
uint8_t opt_assign_assign_spec[] = {N_ASSIGNMENT_STMT, 0};
uint8_t opt_assign_base_spec[] = {N_EMPTY_STR, 0};
uint8_t *opt_assign_stmt[] = {opt_assign_assign_spec, opt_assign_base_spec, \
                                0};

uint8_t **stmt_lookup[] = {stmt, while_stmt, for_stmt, if_stmt, assignment_stmt, \
                            output_stmt, empty_str_stmt, cond_expr_stmt, opt_assign_stmt,\
                            opt_else_stmt, expr_stmt, mult_expr_stmt, unary_expr_stmt, \
                            paren_expr_stmt, basic_expr_stmt, and_expr_stmt, eq_expr_stmt, \
                            cmp_expr_stmt, opt_cond_expr_stmt};
char *stmt_string_lookup[] = {S_STMT, S_WHILE_STMT, S_FOR_STMT, S_IF_STMT, S_ASSIGNMENT_STMT, \
                                S_OUTPUT_STMT, S_EMPTY_STR, S_COND_EXPR, S_OPT_ASSIGN, \
                                S_OPT_ELSE, S_EXPR, S_MULT_EXPR, S_UNARY_EXPR, S_PAREN_EXPR, \
                                S_BASIC_EXPR, S_AND_EXPR, S_EQ_EXPR, S_CMP_EXPR, S_OPT_COND_EXPR};

#define STMT_LOOKUP(num) num >= 101 && num <= 118 ? stmt_lookup[num - N_STMT] : 0
#define STMT_STRING_LOOKUP(num) num >= 101 && num <= 118 ? stmt_string_lookup[num - N_STMT] : 0

uint8_t stmt_check_lookup(uint8_t **check_stmt) {
    if (check_stmt == stmt) return N_STMT;
    if (check_stmt == while_stmt) return N_WHILE_STMT;
    if (check_stmt == for_stmt) return N_FOR_STMT;
    if (check_stmt == if_stmt) return N_IF_STMT;
    if (check_stmt == assignment_stmt) return N_ASSIGNMENT_STMT;
    if (check_stmt == output_stmt) return N_OUTPUT_STMT;
    if (check_stmt == empty_str_stmt) return N_EMPTY_STR;
    if (check_stmt == cond_expr_stmt) return N_COND_EXPR;
    if (check_stmt == opt_assign_stmt) return N_OPT_ASSIGN;
    if (check_stmt == opt_else_stmt) return N_OPT_ELSE;
    if (check_stmt == expr_stmt) return N_EXPR;
    if (check_stmt == mult_expr_stmt) return N_MULT_EXPR;
    if (check_stmt == unary_expr_stmt) return N_UNARY_EXPR;
    if (check_stmt == paren_expr_stmt) return N_PAREN_EXPR;
    if (check_stmt == basic_expr_stmt) return N_BASIC_EXPR;
    if (check_stmt == and_expr_stmt) return N_AND_EXPR;
    if (check_stmt == eq_expr_stmt) return N_EQ_EXPR;
    if (check_stmt == cmp_expr_stmt) return N_CMP_EXPR;
    if (check_stmt == opt_cond_expr_stmt) return N_OPT_COND_EXPR;
    // TODO find out what the fuck is going on
    return 0;
}

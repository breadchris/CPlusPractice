#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"

struct token {
    size_t id;
    size_t numeric_type;
    char *english_type;
    size_t pos;
    size_t len;
    char *value;
};

// Tokens Numeric Type
#define N_ID            1
#define N_STR           2
#define N_INT           3
#define N_FLOAT         4
#define N_WS            5
#define N_NEWLINE       6
#define N_COMMENT       7
#define N_KEYWORD       8
#define N_FOR           8
#define N_WHILE         9
#define N_IF            10
#define N_ELSE          11
#define N_OP            12
#define N_NOT           12
#define N_MOD           13
#define N_AND           14
#define N_OR            15
#define N_ADD           16
#define N_SUB           17
#define N_MULT          18
#define N_DIV           19
#define N_O_BRK         20
#define N_C_BRK         21
#define N_O_SBRK        22
#define N_C_SBRK        23
#define N_SEMI          24
#define N_COMMA         25
#define N_LESS_THAN     26
#define N_GREAT_THAN    27
#define N_EQUAL         28
#define N_LESS_EQUAL    29
#define N_GREAT_EQUAL   30
#define N_NOT_EQUAL     31
#define N_ASSIGN        32
#define N_O_PAREN       33
#define N_C_PAREN       34
#define N_EMPTY_STR     35
#define N_ERR1          97
#define N_ERR2          98
#define N_ERR3          99
#define N_ERR4          100

// Tokens English Type
#define S_ID      "ID"
#define S_STR     "STR"
#define S_INT     "INT"
#define S_FLOAT   "FLOAT"
#define S_WS      "WS"
#define S_NEWLINE "NEWLINE"
#define S_COMMENT "COMMENT"

extern char *keywords[];
extern char *operators[];

#define S_ERR1 "ERR1"
#define S_ERR2 "ERR2"
#define S_ERR3 "ERR3"
#define S_ERR4 "ERR4"

extern const uint8_t is_letter[];
extern const uint8_t is_digit[];
extern const uint8_t is_operator[];
extern const uint8_t is_alpha[];

char *read_lines();
void print_token(struct token *);
void print_tokens(struct token **, size_t, size_t, int);
void free_tokens(struct token **, size_t);
void read_string(char *, size_t, size_t *, struct token *);
void read_number(char *, size_t, size_t *, struct token *);
void read_comment(char *, size_t, size_t *, struct token *);
void read_identifier(char *, size_t, size_t *, struct token *);
struct token **tokenizer(char *, size_t, size_t *);

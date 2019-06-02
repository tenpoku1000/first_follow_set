
// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#if ! defined(TP_FIRST_FOLLOW_SET_H_)
#define TP_FIRST_FOLLOW_SET_H_

#include <stdio.h>
#if defined(_WIN32)
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#if defined(_WIN32)
#include <crtdbg.h>
#endif
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include <string.h>
#include <errno.h>

#define TP_REALLOC(symbol_table, ptr, size) realloc((ptr), (size))

#define TP_CONFIG_OPTION_DISP_USAGE 'h'
#define TP_CONFIG_OPTION_DISABLE_TEST 'n'

typedef enum TP_SYMBOL_
{
    TP_SYMBOL_NULL = 0,
    TP_SYMBOL_PIPE,  // |
    TP_SYMBOL_EMPTY, // ε
    TP_SYMBOL_EOF,

    TP_SYMBOL_IDENTIFIER,
    TP_SYMBOL_PUNCTUATOR,

    TP_SYMBOL_KEYWORD,
    TP_SYMBOL_CONSTANT
}TP_SYMBOL;

typedef enum TP_PARSE_TREE_GRAMMER_
{
    TP_PARSE_TREE_GRAMMER_NULL = 0,

    TP_PARSE_TREE_GRAMMER_PROGRAM,
    TP_PARSE_TREE_GRAMMER_PROGRAM_TMP1,
    TP_PARSE_TREE_GRAMMER_STATEMENT,
    TP_PARSE_TREE_GRAMMER_STATEMENT_TMP1,
    TP_PARSE_TREE_GRAMMER_TYPE,
    TP_PARSE_TREE_GRAMMER_EXPRESSION,
    TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP1,
    TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP2,
    TP_PARSE_TREE_GRAMMER_TERM,
    TP_PARSE_TREE_GRAMMER_TERM_TMP1,
    TP_PARSE_TREE_GRAMMER_TERM_TMP2,
    TP_PARSE_TREE_GRAMMER_FACTOR,
    TP_PARSE_TREE_GRAMMER_FACTOR_TMP1,
    TP_PARSE_TREE_GRAMMER_FACTOR_TMP2,

    TP_PARSE_TREE_GRAMMER_E,
    TP_PARSE_TREE_GRAMMER_E_TMP,
    TP_PARSE_TREE_GRAMMER_T,
    TP_PARSE_TREE_GRAMMER_T_TMP,
    TP_PARSE_TREE_GRAMMER_F,
}TP_PARSE_TREE_GRAMMER;

typedef enum TP_GRAMMER_TYPE_
{
    TP_GRAMMER_TYPE_NULL = 0,
    TP_GRAMMER_TYPE_TERM,
    TP_GRAMMER_TYPE_NONTERM
}TP_GRAMMER_TYPE;

typedef int8_t TP_CHAR8_T;

typedef struct tp_grammer_term_{
    TP_SYMBOL member_symbol;
    TP_CHAR8_T* member_string;
}TP_GRAMMER_TERM;

typedef union tp_grammer_element_union_{
    TP_GRAMMER_TERM member_term;
    TP_PARSE_TREE_GRAMMER member_nonterm;
}TP_GRAMMER_ELEMENT_UNION;

typedef struct tp_grammer_element_{
    TP_GRAMMER_TYPE member_type;
    TP_GRAMMER_ELEMENT_UNION member_body;
    bool member_is_empty;
}TP_GRAMMER_ELEMENT;

typedef struct tp_grammer_{
    TP_PARSE_TREE_GRAMMER member_grammer;

    bool member_is_visit;
    bool member_is_empty;

    TP_GRAMMER_TERM* member_first_set;
    rsize_t member_first_set_num;

    TP_GRAMMER_TERM* member_follow_set;
    rsize_t member_follow_set_num;

    rsize_t member_element_num;
    TP_GRAMMER_ELEMENT* member_element;
}TP_GRAMMER;

#define TP_TERM_ELEMENT(symbol, string) (TP_GRAMMER_TERM){ \
    .member_symbol = (symbol), \
    .member_string = (string) \
}
#define TP_TERM_SYMBOL(symbol, string) (TP_GRAMMER_ELEMENT){ \
    .member_type = TP_GRAMMER_TYPE_TERM, \
    .member_body.member_term = TP_TERM_ELEMENT((symbol), (string)), \
    .member_is_empty = false \
}
// ε
#define TP_TERM_SYMBOL_EMPTY (TP_GRAMMER_ELEMENT){ \
    .member_type = TP_GRAMMER_TYPE_TERM, \
    .member_body.member_term = TP_TERM_ELEMENT(TP_SYMBOL_EMPTY, ""), \
    .member_is_empty = true \
}
// |
#define TP_TERM_PIPE (TP_GRAMMER_ELEMENT){ \
    .member_type = TP_GRAMMER_TYPE_TERM, \
    .member_body.member_term = TP_TERM_ELEMENT(TP_SYMBOL_PIPE, "|"), \
    .member_is_empty = false \
}
#define TP_TERM_NULL (TP_GRAMMER_ELEMENT){ \
    .member_type = TP_GRAMMER_TYPE_NULL, \
    .member_body.member_term = TP_TERM_ELEMENT(TP_SYMBOL_NULL, ""), \
    .member_is_empty = false \
}

#define TP_NONTERM_SYMBOL_REFERENCE(grammer) (TP_GRAMMER_ELEMENT){ \
    .member_type = TP_GRAMMER_TYPE_NONTERM, \
    .member_body.member_nonterm = (grammer), \
    .member_is_empty = false \
}

typedef struct tp_symbol_table_{
    FILE* member_write_log_file;
    bool member_is_need_test_first_follow_set;
}TP_SYMBOL_TABLE;

#define MAKE_GRAMMER(symbol_table, grammer_num, ...) \
  tp_make_grammer( \
    (symbol_table), (grammer_num), \
    (TP_GRAMMER*[]){ __VA_ARGS__ }, \
    sizeof((TP_GRAMMER*[]){ __VA_ARGS__ }) / sizeof(TP_GRAMMER*) \
  )

#define MAKE_GRAMMER_ELEMENT(symbol_table, grammer, ...) \
  tp_make_grammer_element( \
    (symbol_table), (grammer), \
    (TP_GRAMMER_ELEMENT[]){ __VA_ARGS__ }, \
    sizeof((TP_GRAMMER_ELEMENT[]){ __VA_ARGS__ }) / sizeof(TP_GRAMMER_ELEMENT) \
  )

bool tp_calc_first_follow_set(TP_SYMBOL_TABLE* symbol_table);

bool tp_calc_empty(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);
bool tp_calc_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);
bool tp_is_contained_empty_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer);
bool tp_calc_follow_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);

TP_GRAMMER** tp_make_grammer_1(TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num);
bool tp_test_grammer_1(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);

TP_GRAMMER** tp_make_grammer_2(TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num);
bool tp_test_grammer_2(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);

TP_GRAMMER** tp_make_grammer_3(TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num);
bool tp_test_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);

TP_GRAMMER** tp_make_grammer(
    TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num_out, TP_GRAMMER** grammer, rsize_t grammer_num
);
TP_GRAMMER* tp_make_grammer_element(
    TP_SYMBOL_TABLE* symbol_table,
    TP_PARSE_TREE_GRAMMER grammer, TP_GRAMMER_ELEMENT* grammer_element, rsize_t grammer_element_num
);
void tp_free_grammer(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER*** grammer, rsize_t grammer_element_num);
bool tp_print_grammer(TP_SYMBOL_TABLE* symbol_table, TP_PARSE_TREE_GRAMMER grammer);
bool tp_compare_first_or_follow_set(
    TP_GRAMMER_TERM* test_first_or_follow_set, rsize_t test_first_or_follow_set_num,
    TP_GRAMMER_TERM* first_or_follow_set
);

#endif


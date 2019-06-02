
// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#include "tp_first_follow_set.h"

static bool test_first_set_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer);
static bool test_follow_set_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer);

// Grammer:
//
// E -> TE' num
// E' -> TE' | ε
// T -> ε
//
// Example:
//
// 100
//
TP_GRAMMER** tp_make_grammer_3(TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num)
{
    // E -> TE' num
    TP_GRAMMER* E = MAKE_GRAMMER_ELEMENT(
        symbol_table, TP_PARSE_TREE_GRAMMER_E,
        TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_T),
        TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_E_TMP),
        TP_TERM_SYMBOL(TP_SYMBOL_CONSTANT, "num"),
        TP_TERM_NULL
    );

    // E' -> TE' | ε
    TP_GRAMMER* E_TMP = MAKE_GRAMMER_ELEMENT(
        symbol_table, TP_PARSE_TREE_GRAMMER_E_TMP,
        TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_T),
        TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_E_TMP),
        TP_TERM_PIPE,
        TP_TERM_SYMBOL_EMPTY,
        TP_TERM_NULL
    );

    // T -> ε
    TP_GRAMMER* T = MAKE_GRAMMER_ELEMENT(
        symbol_table, TP_PARSE_TREE_GRAMMER_T,
        TP_TERM_SYMBOL_EMPTY,
        TP_TERM_NULL
    );

    // NULL
    TP_GRAMMER* end = MAKE_GRAMMER_ELEMENT(symbol_table, TP_PARSE_TREE_GRAMMER_NULL, TP_TERM_NULL);

    return MAKE_GRAMMER(symbol_table, grammer_num, E, E_TMP, T, end);
}

bool tp_test_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num)
{
    fprintf(symbol_table->member_write_log_file, "\n=== begin test grammer_3: ===\n");

    for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[i]->member_grammer; ++i){

        if ( ! test_first_set_grammer_3(symbol_table, grammer_tbl[i])){

            fprintf(symbol_table->member_write_log_file, "\n=== first set test failed. ===\n");

            return false;
        }
    }

    fprintf(symbol_table->member_write_log_file, "\n=== first set test passed. ===\n");

    for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[i]->member_grammer; ++i){

        if ( ! test_follow_set_grammer_3(symbol_table, grammer_tbl[i])){

            fprintf(symbol_table->member_write_log_file, "\n=== follow set test failed. ===\n");

            return false;
        }
    }

    fprintf(symbol_table->member_write_log_file, "\n=== follow set test passed. ===\n");

    return true;
}

static bool test_first_set_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer)
{
    // E -> TE' num
    static TP_GRAMMER_TERM first_set_GRAMMER_E[] = {
        { .member_symbol = TP_SYMBOL_CONSTANT, .member_string = "num" },
    };

    // E' -> TE' | ε
    static TP_GRAMMER_TERM first_set_GRAMMER_E_TMP[] = {
        { .member_symbol = TP_SYMBOL_EMPTY, .member_string = "EMPTY" },
    };

    // T -> ε
    static TP_GRAMMER_TERM first_set_GRAMMER_T[] = {
        { .member_symbol = TP_SYMBOL_EMPTY, .member_string = "EMPTY" },
    };

    static TP_GRAMMER_TERM* first_set[] = {
        first_set_GRAMMER_E,
        first_set_GRAMMER_E_TMP,
        first_set_GRAMMER_T,
        NULL
    };

    static rsize_t first_set_num[] = {
        sizeof(first_set_GRAMMER_E) / sizeof(TP_GRAMMER_TERM),
        sizeof(first_set_GRAMMER_E_TMP) / sizeof(TP_GRAMMER_TERM),
        sizeof(first_set_GRAMMER_T) / sizeof(TP_GRAMMER_TERM),
        0
    };

    static TP_PARSE_TREE_GRAMMER grammer_index[] = {
        TP_PARSE_TREE_GRAMMER_E,
        TP_PARSE_TREE_GRAMMER_E_TMP,
        TP_PARSE_TREE_GRAMMER_T,
        TP_PARSE_TREE_GRAMMER_NULL
    };

    for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_index[i]; ++i){

        if (grammer_index[i] == grammer->member_grammer){

            if (first_set_num[i] != grammer->member_first_set_num){

                fprintf(
                    symbol_table->member_write_log_file, "\n=== first set num(index = %zd: %zd, %zd) ===\n",
                    i, first_set_num[i], grammer->member_first_set_num
                );

                return false;
            }

            if ( ! tp_compare_first_or_follow_set(first_set[i], first_set_num[i], grammer->member_first_set)){

                fprintf(
                    symbol_table->member_write_log_file,
                    "\n=== tp_compare_first_or_follow_set failed. ===\n"
                );

                return false;
            }

            return true;
        }
    }

    fprintf(
        symbol_table->member_write_log_file, "\n=== unmatch grammer(%d). ===\n",
        grammer->member_grammer
    );

    return false;
}

static bool test_follow_set_grammer_3(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer)
{
    // E -> TE' num
    static TP_GRAMMER_TERM follow_set_GRAMMER_E[] = {
        { .member_symbol = TP_SYMBOL_EOF, .member_string = "EOF" },
    };

    // E' -> TE' | ε
    static TP_GRAMMER_TERM follow_set_GRAMMER_E_TMP[] = {
        { .member_symbol = TP_SYMBOL_CONSTANT, .member_string = "num" },
    };

    // T -> ε
    static TP_GRAMMER_TERM follow_set_GRAMMER_T[] = {
        { .member_symbol = TP_SYMBOL_EOF, .member_string = "EOF" },
        { .member_symbol = TP_SYMBOL_CONSTANT, .member_string = "num" },
    };

    static TP_GRAMMER_TERM* follow_set[] = {
        follow_set_GRAMMER_E,
        follow_set_GRAMMER_E_TMP,
        follow_set_GRAMMER_T,
        NULL
    };

    static rsize_t follow_set_num[] = {
        sizeof(follow_set_GRAMMER_E) / sizeof(TP_GRAMMER_TERM),
        sizeof(follow_set_GRAMMER_E_TMP) / sizeof(TP_GRAMMER_TERM),
        sizeof(follow_set_GRAMMER_T) / sizeof(TP_GRAMMER_TERM),
        0
    };

    static TP_PARSE_TREE_GRAMMER grammer_index[] = {
        TP_PARSE_TREE_GRAMMER_E,
        TP_PARSE_TREE_GRAMMER_E_TMP,
        TP_PARSE_TREE_GRAMMER_T,
        TP_PARSE_TREE_GRAMMER_NULL
    };

    for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_index[i]; ++i){

        if (grammer_index[i] == grammer->member_grammer){

            if (follow_set_num[i] != grammer->member_follow_set_num){

                fprintf(
                    symbol_table->member_write_log_file, "\n=== follow set num(index = %zd: %zd, %zd) ===\n",
                    i, follow_set_num[i], grammer->member_follow_set_num
                );

                return false;
            }

            if ( ! tp_compare_first_or_follow_set(follow_set[i], follow_set_num[i], grammer->member_follow_set)){

                fprintf(
                    symbol_table->member_write_log_file,
                    "\n=== tp_compare_first_or_follow_set failed. ===\n"
                );

                return false;
            }

            return true;
        }
    }

    fprintf(
        symbol_table->member_write_log_file, "\n=== unmatch grammer(%d). ===\n",
        grammer->member_grammer
    );

    return false;
}


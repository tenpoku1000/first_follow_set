
// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#include "tp_first_follow_set.h"

static bool calc_first_follow(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);

bool tp_calc_first_follow_set(TP_SYMBOL_TABLE* symbol_table)
{
    typedef struct func_table_{
        rsize_t grammer_num;
        TP_GRAMMER** grammer_tbl;
        TP_GRAMMER** (*tp_make_grammer)(TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num);
        bool (*tp_test_grammer)(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num);
    }FUNC_TABLE;

    FUNC_TABLE func_table[] = {
        { 0, NULL, tp_make_grammer_1, tp_test_grammer_1 },
        { 0, NULL, tp_make_grammer_2, tp_test_grammer_2 },
        { 0, NULL, tp_make_grammer_3, tp_test_grammer_3 },
        { 0, NULL, NULL, NULL }
    };

    bool status = false;

    for (rsize_t i = 0; NULL != func_table[i].tp_make_grammer; ++i){

        func_table[i].grammer_tbl = func_table[i].tp_make_grammer(
            symbol_table, &(func_table[i].grammer_num)
        );

        if ( ! calc_first_follow(symbol_table, func_table[i].grammer_tbl, func_table[i].grammer_num)){

            goto out;
        }
    }

    if ( ! symbol_table->member_is_need_test_first_follow_set){

        fprintf(symbol_table->member_write_log_file, "\n=== test is disable. ===\n");

        status = true;

        goto out;
    }

    fprintf(symbol_table->member_write_log_file, "\n=== begin test: ===\n");

    bool is_test_failed = false;

    for (rsize_t i = 0; NULL != func_table[i].tp_make_grammer; ++i){

        if ( ! func_table[i].tp_test_grammer(
            symbol_table, func_table[i].grammer_tbl, func_table[i].grammer_num)){

            fprintf(symbol_table->member_write_log_file, "\n=== test failed. ===\n");

            is_test_failed = true;
        }
    }

    if (is_test_failed){

        goto out;
    }

    fprintf(symbol_table->member_write_log_file, "\n=== all test passed. ===\n");

    status = true;

out:

    for (rsize_t i = 0; NULL != func_table[i].tp_make_grammer; ++i){

        tp_free_grammer(symbol_table, &(func_table[i].grammer_tbl), func_table[i].grammer_num);
    }

    return status;
}

static bool calc_first_follow(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num)
{
    fprintf(symbol_table->member_write_log_file, "\n=== calc_empty ===\n");

    if ( ! tp_calc_empty(symbol_table, grammer_tbl, grammer_num)){

        tp_free_grammer(symbol_table, &grammer_tbl, grammer_num);

        return false;
    }

    fprintf(symbol_table->member_write_log_file, "\n=== calc_first_set ===\n");

    if ( ! tp_calc_first_set(symbol_table, grammer_tbl, grammer_num)){

        tp_free_grammer(symbol_table, &grammer_tbl, grammer_num);

        return false;
    }

    fprintf(symbol_table->member_write_log_file, "\n=== calc_follow_set ===\n");

    if ( ! tp_calc_follow_set(symbol_table, grammer_tbl, grammer_num)){

        tp_free_grammer(symbol_table, &grammer_tbl, grammer_num);

        return false;
    }

    return true;
}


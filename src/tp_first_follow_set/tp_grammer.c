
// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#include "tp_first_follow_set.h"

static int compare_grammer_term(const void* param1, const void* param2);

TP_GRAMMER** tp_make_grammer(
    TP_SYMBOL_TABLE* symbol_table, rsize_t* grammer_num_out, TP_GRAMMER** grammer, rsize_t grammer_num)
{
    TP_GRAMMER** grammer_buffer = (TP_GRAMMER**)calloc(grammer_num, sizeof(TP_GRAMMER*));

    if (NULL == grammer_buffer){

        *grammer_num_out = 0;

        return NULL;
    }

    memcpy(
        grammer_buffer, grammer,
        sizeof(TP_GRAMMER*) * grammer_num
    );
    *grammer_num_out = grammer_num;

    return grammer_buffer;
}

TP_GRAMMER* tp_make_grammer_element(
    TP_SYMBOL_TABLE* symbol_table,
    TP_PARSE_TREE_GRAMMER grammer, TP_GRAMMER_ELEMENT* grammer_element, rsize_t grammer_element_num)
{
    TP_GRAMMER* grammer_element_buffer = (TP_GRAMMER*)calloc(1, sizeof(TP_GRAMMER));

    if (NULL == grammer_element_buffer){

        return NULL;
    }

    TP_GRAMMER_ELEMENT* element_buffer = (TP_GRAMMER_ELEMENT*)calloc(
        grammer_element_num, sizeof(TP_GRAMMER_ELEMENT)
    );

    if (NULL == element_buffer){

        free(grammer_element_buffer);
        grammer_element_buffer = NULL;

        return NULL;
    }

    grammer_element_buffer->member_grammer = grammer;
    grammer_element_buffer->member_is_empty = false;
    grammer_element_buffer->member_element_num = grammer_element_num;
    grammer_element_buffer->member_element = element_buffer;
    memcpy(
        grammer_element_buffer->member_element, grammer_element,
        sizeof(TP_GRAMMER_ELEMENT) * grammer_element_num
    );

    return grammer_element_buffer;
}

void tp_free_grammer(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER*** grammer, rsize_t grammer_element_num)
{
    if (NULL == grammer){

        return;
    }

    TP_GRAMMER** p = *grammer;

    if (p){

        for (rsize_t i = 0; grammer_element_num > i; ++i){

            free(p[i]->member_first_set);
            p[i]->member_first_set = NULL;

            free(p[i]->member_follow_set);
            p[i]->member_follow_set = NULL;

            free(p[i]->member_element);
            p[i]->member_element = NULL;

            free(p[i]);
            p[i] = NULL;
        }

        free(*grammer);
        *grammer = NULL;
    }
}

bool tp_print_grammer(TP_SYMBOL_TABLE* symbol_table, TP_PARSE_TREE_GRAMMER grammer)
{
    switch (grammer){
    case TP_PARSE_TREE_GRAMMER_PROGRAM:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_PROGRAM:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_PROGRAM_TMP1:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_PROGRAM_TMP1:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_STATEMENT:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_STATEMENT:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_STATEMENT_TMP1:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_STATEMENT_TMP1:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_TYPE:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_TYPE:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_EXPRESSION:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_EXPRESSION:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP1:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP1:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP2:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_EXPRESSION_TMP2:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_TERM:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_TERM:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_TERM_TMP1:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_TERM_TMP1:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_TERM_TMP2:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_TERM_TMP2:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_FACTOR:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_FACTOR:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_FACTOR_TMP1:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_FACTOR_TMP1:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_FACTOR_TMP2:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_FACTOR_TMP2:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_E:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_E:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_E_TMP:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_E_TMP:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_T:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_T:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_T_TMP:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_T_TMP:\n");
        break;
    case TP_PARSE_TREE_GRAMMER_F:
        fprintf(symbol_table->member_write_log_file, "TP_PARSE_TREE_GRAMMER_F:\n");
        break;
    default:
        return false;
    }

    return true;
}

bool tp_compare_first_or_follow_set(
    TP_GRAMMER_TERM* test_first_or_follow_set, rsize_t test_first_or_follow_set_num,
    TP_GRAMMER_TERM* first_or_follow_set)
{
    qsort(test_first_or_follow_set, test_first_or_follow_set_num, sizeof(TP_GRAMMER_TERM), compare_grammer_term);

    qsort(first_or_follow_set, test_first_or_follow_set_num, sizeof(TP_GRAMMER_TERM), compare_grammer_term);

    for (rsize_t i = 0; test_first_or_follow_set_num > i; ++i){

        if ((test_first_or_follow_set[i].member_symbol != first_or_follow_set[i].member_symbol) ||
            (0 != strcmp(test_first_or_follow_set[i].member_string, first_or_follow_set[i].member_string))){

            return false;
        }
    }

    return true;
}

static int compare_grammer_term(const void* param1, const void* param2)
{
    TP_GRAMMER_TERM* first_or_follow_set1 = (TP_GRAMMER_TERM*)param1;

    TP_GRAMMER_TERM* first_or_follow_set2 = (TP_GRAMMER_TERM*)param2;

    if (first_or_follow_set1->member_symbol < first_or_follow_set2->member_symbol){

        return -1;
    }

    if (first_or_follow_set1->member_symbol > first_or_follow_set2->member_symbol){

        return 1;
    }

    return strcmp(first_or_follow_set1->member_string, first_or_follow_set2->member_string);
}



// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#include "tp_first_follow_set.h"

static bool calc_first_set_element(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, TP_GRAMMER* grammer, TP_GRAMMER* current_grammer,
    bool* is_current_first, bool* is_change
);
static bool print_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer);
static bool append_first_set(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer, TP_GRAMMER_TERM* grammer_term, bool* is_change
);
static bool append_first_set_from_first_set(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer_from, TP_GRAMMER* grammer_to, bool* is_change
);

bool tp_calc_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, rsize_t grammer_num)
{
    for (bool is_change = true; is_change; ){

        fprintf(symbol_table->member_write_log_file, "\n=== calc_first_set_loop ===\n");

        is_change = false;

        for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[i]->member_grammer; ++i){

            bool is_current_first = true;

            if ( ! calc_first_set_element(
                symbol_table, grammer_tbl, grammer_tbl[i], grammer_tbl[i], &is_current_first, &is_change)){

                return false;
            }

            for (rsize_t j = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[j]->member_grammer; ++j){

                grammer_tbl[j]->member_is_visit = false;
            }
        }
    }

    for (rsize_t i = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[i]->member_grammer; ++i){

        if ( ! print_first_set(symbol_table, grammer_tbl[i])){

            return false;
        }
    }

    return true;
}

bool tp_is_contained_empty_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer)
{
    for (rsize_t i = 0; grammer->member_first_set_num > i; ++i){

        if (TP_SYMBOL_EMPTY == grammer->member_first_set[i].member_symbol){

            return true;
        }
    }

    return false;
}

static bool calc_first_set_element(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER** grammer_tbl, TP_GRAMMER* grammer, TP_GRAMMER* current_grammer,
    bool* is_current_first, bool* is_change)
{
    if (current_grammer->member_is_visit){

        return true;
    }

    current_grammer->member_is_visit = true;

    rsize_t element_num = current_grammer->member_element_num;
    TP_GRAMMER_ELEMENT* grammer_element = current_grammer->member_element;

    bool is_current_empty = true;
    bool is_prev_pipe = false;

    for (rsize_t i = 0; element_num > i; ++i){

        switch (grammer_element[i].member_type){
        case TP_GRAMMER_TYPE_TERM:{

            TP_GRAMMER_TERM* grammer_term = &(grammer_element[i].member_body.member_term);

            switch (grammer_term->member_symbol){
            case TP_SYMBOL_EMPTY:
//              break;
            case TP_SYMBOL_IDENTIFIER:
//              break;
            case TP_SYMBOL_PUNCTUATOR:
//              break;
            case TP_SYMBOL_KEYWORD:
//              break;
            case TP_SYMBOL_CONSTANT:

                if (grammer_element[i].member_is_empty){

                    break;
                }else{

                    is_current_empty = false;
                }

                if (*is_current_first){

                    if ( ! append_first_set(symbol_table, grammer, grammer_term, is_change)){

                        return false;
                    }

                    *is_current_first = false;
                }

                is_prev_pipe = false;

                break;
            case TP_SYMBOL_PIPE:

                if (0 == i){

                    return false;
                }else if (element_num == (i + 1)){

                    return false;
                }else if (is_prev_pipe){

                    return false;
                }

                if (is_current_empty){

                    TP_GRAMMER_TERM grammer_term_empty = { .member_symbol = TP_SYMBOL_EMPTY, .member_string = "EMPTY" };

                    if ( ! append_first_set(symbol_table, current_grammer, &grammer_term_empty, is_change)){

                        return false;
                    }
                }else{

                    is_current_empty = true;
                }

                *is_current_first = true;

                is_prev_pipe = true;

                break;
            case TP_SYMBOL_NULL:
                break;
            default:
                return false;
            }

            break;
        }
        case TP_GRAMMER_TYPE_NONTERM:{

            TP_PARSE_TREE_GRAMMER nonterm = grammer_element[i].member_body.member_nonterm;

            for (rsize_t j = 0; TP_PARSE_TREE_GRAMMER_NULL != grammer_tbl[j]->member_grammer; ++j){

                if (nonterm == grammer_tbl[j]->member_grammer){

                    if (grammer_tbl[j]->member_is_empty){

                        break;
                    }else{

                        is_current_empty = false;
                    }

                    if (*is_current_first){

                        if ( ! calc_first_set_element(
                            symbol_table, grammer_tbl, grammer, grammer_tbl[j], is_current_first, is_change)){

                            return false;
                        }

                        if (tp_is_contained_empty_first_set(symbol_table, grammer_tbl[j])){

                            if ( ! append_first_set_from_first_set(symbol_table, grammer_tbl[j], grammer, is_change)){

                                return false;
                            }
                        }else{

                            *is_current_first = false;
                        }
                    }

                    break;
                }
            }

            is_prev_pipe = false;

            break;
        }
        case TP_GRAMMER_TYPE_NULL:

            if (element_num != (i + 1)){

                return false;
            }

            if (is_current_empty){

                TP_GRAMMER_TERM grammer_term_empty = { .member_symbol = TP_SYMBOL_EMPTY, .member_string = "EMPTY" };

                if ( ! append_first_set(symbol_table, current_grammer, &grammer_term_empty, is_change)){

                    return false;
                }
            }

            return true;
        default:
            return false;
        }
    }

    return false;
}

static bool print_first_set(TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer)
{
    if ( ! tp_print_grammer(symbol_table, grammer->member_grammer)){

        return false;
    }

    for (rsize_t i = 0; grammer->member_first_set_num > i; ++i){

        fprintf(symbol_table->member_write_log_file, "%s\n", grammer->member_first_set[i].member_string);
    }

    fprintf(symbol_table->member_write_log_file, "\n");

    return true;
}

static bool append_first_set(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer, TP_GRAMMER_TERM* grammer_term, bool* is_change)
{
    for (rsize_t i = 0; grammer->member_first_set_num > i; ++i){

        if (0 == strcmp(grammer_term->member_string, grammer->member_first_set[i].member_string)){

            return true;
        }
    }

    TP_GRAMMER_TERM* tmp_grammer_term = (TP_GRAMMER_TERM*)TP_REALLOC(symbol_table,
        grammer->member_first_set, sizeof(TP_GRAMMER_TERM) * (grammer->member_first_set_num + 1)
    );

    if (NULL == tmp_grammer_term){

        return false;
    }

    grammer->member_first_set = tmp_grammer_term;

    ++(grammer->member_first_set_num);

    grammer->member_first_set[grammer->member_first_set_num - 1] = *grammer_term;

    *is_change = true;

    return true;
}

static bool append_first_set_from_first_set(
    TP_SYMBOL_TABLE* symbol_table, TP_GRAMMER* grammer_from, TP_GRAMMER* grammer_to, bool* is_change)
{
    for (rsize_t i = 0; grammer_from->member_first_set_num > i; ++i){

        if (TP_SYMBOL_EMPTY == grammer_from->member_first_set[i].member_symbol){

            continue;
        }

        if (0 == grammer_to->member_first_set_num){

            if ( ! append_first_set(symbol_table, grammer_to, &(grammer_from->member_first_set[i]), is_change)){

                return false;
            }
        }

        for (rsize_t j = 0; grammer_to->member_first_set_num > j; ++j){

            if (0 == strcmp(grammer_from->member_first_set[i].member_string, grammer_to->member_first_set[j].member_string)){

                continue;
            }

            if ( ! append_first_set(symbol_table, grammer_to, &(grammer_from->member_first_set[i]), is_change)){

                return false;
            }
        }
    }

    return true;
}


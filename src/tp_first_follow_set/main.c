
// Copyright (C) 2019 Shin'ichi Ichikawa. Released under the MIT license.

#include "tp_first_follow_set.h"

static TP_SYMBOL_TABLE symbol_table = { .member_is_need_test_first_follow_set = true };

static bool parse_cmd_line_param(TP_SYMBOL_TABLE* symbol_table, int argc, char** argv);
static calc_first_follow_set_entry_point(TP_SYMBOL_TABLE* symbol_table);

int main(int argc, char** argv)
{
#if defined(_WIN32)
    errno_t err = _set_errno(0);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    symbol_table.member_write_log_file = stdout;

    if ( ! parse_cmd_line_param(&symbol_table, argc, argv)){

        return EXIT_FAILURE;
    }

    if ( ! calc_first_follow_set_entry_point(&symbol_table)){

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static bool parse_cmd_line_param(TP_SYMBOL_TABLE* symbol_table, int argc, char** argv)
{
    char* command_line_param = NULL;

    for (int i = 0; argc > i; ++i){

        if (0 == i){

            continue;
        }

        char* param = argv[i];

        if (('/' != param[0]) && ('-' != param[0])){

            if (command_line_param){

                goto error_out;
            }else{

                command_line_param = param;
            }
        }else{

            rsize_t length = strlen(param);

            for (int j = 1; length > j; ++j){

                switch (param[j]){
                case TP_CONFIG_OPTION_DISP_USAGE:
                    goto error_out;
                case TP_CONFIG_OPTION_DISABLE_TEST:
                    symbol_table->member_is_need_test_first_follow_set = false;
                    break;
                default:
                    goto error_out;
                }
            }
        }
    }

    return true;

error_out:

    fprintf(stderr, "usage: first_follow_set [-/][hn]\n");
    fprintf(stderr, "  -h : disp usage.\n");
    fprintf(stderr, "  -n : disable test.\n");

    return false;
}

static calc_first_follow_set_entry_point(TP_SYMBOL_TABLE* symbol_table)
{
    fprintf(symbol_table->member_write_log_file, "\n");

    if ( ! tp_calc_first_follow_set(symbol_table)){

        fprintf(symbol_table->member_write_log_file, "\ncalc_first_follow error.\n\n");

        return false;
    }

    fprintf(symbol_table->member_write_log_file, "\ncalc_first_follow success.\n\n");

    return true;
}


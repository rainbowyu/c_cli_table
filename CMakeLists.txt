cmake_minimum_required(VERSION 3.16)
project(c_cli_table C)

set(CMAKE_C_STANDARD 99)
include_directories(. csv_parser unit_test)
add_executable(c_cli_table main.c cli_table.c cli_table.h doubly_link_list.h csv_parser/csv_parser.c csv_parser/csv_parser.h testcase/test_case_csv_parser.c testcase/test_case_cli_table.c unit_test/unit_test.c unit_test/unit_test.h)

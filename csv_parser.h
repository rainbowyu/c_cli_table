//
// Created by yushigengyu on 2021/5/14.
//

#ifndef C_CLI_TABLE_CSV_PARSER_H
#define C_CLI_TABLE_CSV_PARSER_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
    uint16_t row;
    uint16_t column;
    char*** value;
}CSV_STRUCT;

typedef struct{
    uint16_t row;
    uint16_t column;
    uint16_t* rowLenArray;
    const char** value;
}CSV_STRUCT_SPLIT_LINE;

CSV_STRUCT* csv_parser(const char* csvStr);
void csv_delete(CSV_STRUCT* csv);
CSV_STRUCT_SPLIT_LINE* csv_split_on_newlines(const char *csvStr);
#endif //C_CLI_TABLE_CSV_PARSER_H

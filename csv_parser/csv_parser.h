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
    uint16_t len;
    const char* value;
}CSV_STRUCT_FIELD, CSV_STRUCT_LINE;

typedef struct{
    uint16_t row;
    uint16_t* rowLenArray;
    const char** value;
}CSV_STRUCT_SPLIT_LINE;

typedef struct{
    uint16_t row;
    uint16_t* rowFieldCount;
    CSV_STRUCT_FIELD*** field;
}CSV_STRUCT;

CSV_STRUCT* csv_parser(const char* csvStr);
void csv_delete(CSV_STRUCT* csv);
CSV_STRUCT_FIELD* csv_field_get(CSV_STRUCT *csv, uint16_t row, uint16_t column);
CSV_STRUCT_SPLIT_LINE* csv_split_on_lines_create(const char *csvStr);
void csv_split_on_lines_delete(CSV_STRUCT_SPLIT_LINE* csvSLineObject);
#endif //C_CLI_TABLE_CSV_PARSER_H

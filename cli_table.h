//
// Created by yushigengyu on 2021/4/30.
//

#ifndef C_CLI_TABLE_CLI_TABLE_H
#define C_CLI_TABLE_CLI_TABLE_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"
#include "doubly_link_list.h"

#define TABLE_NAME_CHAR_MAX 20
#define CELL_VALUE_LEN_MAX 1000

//print define
#define TABLE_PRINTF printf
#define BACK_SPACE 0x08

typedef enum{
    HLINE_CHAR,
    VLINE_CHAR,
    CORNER_CHAR_TM,
    CORNER_CHAR_BM,
    CORNER_CHAR_MM,
    CORNER_CHAR_LM,
    CORNER_CHAR_RM,
    CORNER_CHAR_TL,
    CORNER_CHAR_TR,
    CORNER_CHAR_BL,
    CORNER_CHAR_BR,
    CHAR_MAX,
}TABLE_PRINT_CHAR;

typedef enum{
    PRINT_TYPE_DEFAULT,
    PRINT_TYPE_2,
    PRINT_TYPE_3,
    PRINT_TYPE_CUSTOM,
}TABLE_PRINT_TYPE;

typedef enum{
    ALIGNMENT_LEFT = 0,
    ALIGNMENT_RIGHT,
    ALIGNMENT_CENTER,
}TABLE_ALIGNMENT;

typedef struct{
    uint16_t len;
    char* str;
}CellValue;

typedef struct{
    uint32_t row;
    uint32_t column;
    TABLE_ALIGNMENT alignment;
    CellValue value;
}CellObject;

typedef struct{
    char name[TABLE_NAME_CHAR_MAX];
    const char* printChar[CHAR_MAX];
    uint32_t rowMax;
    uint32_t columnMax;
    uint16_t* columnWidth;
    CellObject*** cellTable;
}StaticTableObject;

const char *c_cli_table_version_get();
StaticTableObject* cli_static_table_create(uint32_t row, uint32_t column);
StaticTableObject* cli_static_table_csv_str_create(const char* csvStr);
void cli_static_table_delete(StaticTableObject* object);
void cli_static_table_printtype_set(StaticTableObject* object, TABLE_PRINT_TYPE type, const char* charType[CHAR_MAX]);

CellObject *cell_create(const char *value, uint16_t len);
void cell_delete(CellObject* object);
int cell_value_set(CellObject *object, const char *value, uint16_t len);
int cell_align_set(CellObject* object, TABLE_ALIGNMENT align);
int cli_static_table_cell_set(StaticTableObject* object, uint32_t row, uint32_t column, CellObject* cell);
void cli_static_table_print(StaticTableObject* object);
#endif //C_CLI_TABLE_CLI_TABLE_H

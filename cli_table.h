//
// Created by 喻时耕宇 on 2021/4/30.
//

#ifndef C_CLI_TABLE_CLI_TABLE_H
#define C_CLI_TABLE_CLI_TABLE_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "doubly_link_list.h"
// y x 0 1 2 3 . n
// 0   - - - - - -
// 1   - - - - - -
// 2   - - - - - -
// 3   - - - - - -
// .   - - - - - -
// n   - - - - - -


#define TABLE_NAME_CHAR_MAX 20
#define CELL_VALUE_LEN_MAX 1000

//print define
#define TABLE_PRINTF printf
#define HLINE_CHAR  "━"
#define VLINE_CHAR  "┃"
#define BACK_SPACE 0x08
#define CORNER_CHAR_TM "┳"
#define CORNER_CHAR_BM "┻"
#define CORNER_CHAR_MM "╋"
#define CORNER_CHAR_LM "┣"
#define CORNER_CHAR_RM "┫"

#define CORNER_CHAR_TL "┏"
#define CORNER_CHAR_TR "┓"
#define CORNER_CHAR_BL "┗"
#define CORNER_CHAR_BR "┛"


typedef struct{
    uint16_t len;
    char* str;
}CellValue;

typedef struct{
    uint32_t row;
    uint32_t column;
    CellValue value;
}CellObject;

typedef struct{
    char name[TABLE_NAME_CHAR_MAX];
    uint32_t rowMax;
    uint32_t columnMax;
    uint16_t* columnWidth;
    CellObject*** cellTable;
}StaticTableObject;

CellObject* cell_create(const char * value);
void cell_delete(CellObject* object);
int cell_set_value(CellObject* object, const char* value);
int cli_static_table_set_cell(StaticTableObject* object, uint32_t row, uint32_t column, CellObject* cell);
void cli_static_table_delete(StaticTableObject* object);
StaticTableObject* cli_static_table_create(uint32_t row, uint32_t column);
void cli_static_table_print(StaticTableObject* object);
#endif //C_CLI_TABLE_CLI_TABLE_H

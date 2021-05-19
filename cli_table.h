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

//type1
//┏━━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━━┓
//┃ name ┃ ch1 ┃ ch2 ┃ ch3 ┃ ch4 ┃ ch5 ┃
//┣━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━┫
//┃enable┃false┃false┃false┃false┃false┃
//┣━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━┫
//┃fre   ┃20000┃20000┃20000┃20000┃20000┃
//┣━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━┫
//┃point ┃8192 ┃8192 ┃8192 ┃8192 ┃8192 ┃
//┣━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━┫
//┃cutoff┃15000┃15000┃15000┃15000┃15000┃
//┗━━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━━┛

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
    uint32_t rowMax;
    uint32_t columnMax;
    uint16_t* columnWidth;
    CellObject*** cellTable;
}StaticTableObject;

const char *c_cli_table_version_get();
StaticTableObject* cli_static_table_create(uint32_t row, uint32_t column);
void cli_static_table_delete(StaticTableObject* object);
CellObject *cell_create(const char *value, uint16_t len);
void cell_delete(CellObject* object);
int cell_set_value(CellObject* object, const char* value);
int cell_set_align(CellObject* object, TABLE_ALIGNMENT align);
int cli_static_table_set_cell(StaticTableObject* object, uint32_t row, uint32_t column, CellObject* cell);
void cli_static_table_print(StaticTableObject* object);
#endif //C_CLI_TABLE_CLI_TABLE_H

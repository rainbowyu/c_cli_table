//
// Created by 喻时耕宇 on 2021/4/30.
//

#ifndef C_CLI_TABLE_CLI_TABLE_H
#define C_CLI_TABLE_CLI_TABLE_H

#include <stdint.h>
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
typedef struct{
    uint16_t max;
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
    CellObject** cellTable;
}TableObject;

#endif //C_CLI_TABLE_CLI_TABLE_H

//
// Created by 喻时耕宇 on 2021/4/30.
//

#include "cli_table.h"
inline static void cell_init(CellObject* object, uint32_t row, uint32_t column){
    object->row = row;
    object->column = column;
    object->value.max = 0;
    object->value.str = NULL;
}

int cli_table_cell_set_value(uint32_t row, uint32_t column, const char* str, uint16_t maxLen){
    uint16_t len = strlen(str);
    len = len > maxLen ? maxLen : len;
    
    return len;
}

void cli_table_delete(TableObject* object){
    if(object) {
        if(object->cellTable){
            for (uint32_t i = 0; i < object->columnMax; i++) {
                if (object->cellTable[i] != NULL) {
                    free(object->cellTable[i]);
                }
            }
            free(object->cellTable);
        }
        free(object);
    }
}

TableObject* cli_table_create(uint32_t row, uint32_t column){
    TableObject* object = malloc(sizeof(TableObject));
    if(object == NULL)
        goto end;
    object->cellTable = malloc(column * sizeof(CellObject*));
    if(object->cellTable == NULL) {
        cli_table_delete(object);
        object = NULL;
        goto end;
    }
    object->columnMax = column;
    for(uint32_t i = 0; i < row; i++){
        object->cellTable[i] = malloc(row * sizeof(CellObject));
        if(object->cellTable[i] == NULL) {
            cli_table_delete(object);
            object = NULL;
            goto end;
        }
        cell_init(object->cellTable[i]);
    }
    object->rowMax = row;
    end:
    return object;
}

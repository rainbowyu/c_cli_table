//
// Created by 喻时耕宇 on 2021/4/30.
//
#include "cli_table.h"
const char *version = "v0.1.0";

static int cell_init(CellObject* object, const char* value){
    int res = 0;
    if(value != NULL) {
        object->value.len = strlen(value);
        object->value.str = malloc(object->value.len + 1);
        if(object->value.str == NULL){
            res = -1;
            goto end;
        }
        memcpy(object->value.str, value, object->value.len);
    }else {
        object->value.len = 0;
        object->value.str = NULL;
    }
    end:
    return res;
}

static void cell_deinit(CellObject* object){
    if(object->value.len > 0 && object->value.str != NULL){
        object->value.len = 0;
        free(object->value.str);
        object->value.str = NULL;
    }
}

static uint32_t static_table_row_get_width(StaticTableObject* object, uint32_t row){
    uint32_t width = 0;
    for(uint32_t i = 0; i < object->columnMax; i++)
        if(object->cellTable[row][i] != NULL)
            width += (object->cellTable[row][i]->value.len + 1);
        else
            width += 1;
    width += 1;
    return width;
}

static uint32_t static_table_column_get_width(StaticTableObject* object, uint32_t column){
    uint32_t width = 0;
    for(uint32_t i = 0; i < object->rowMax; i++){
        if(object->cellTable[i][column] != NULL)
            if(object->cellTable[i][column]->value.len > width)
                width = object->cellTable[i][column]->value.len;
    }
    return width;
}

CellObject* cell_create(const char * value){
    CellObject* object = calloc(1, sizeof(CellObject));
    if(object == NULL)
        goto end;
    if(cell_init(object, value) != 0){
        free(object);
        object = NULL;
        goto end;
    }
    end:
    return object;
}

void cell_delete(CellObject* object){
    if(object != NULL) {
        cell_deinit(object);
        free(object);
    }
}

int cell_set_value(CellObject* object, const char* value){
    int res = 0;
    if(value == NULL){
        res = -1;
        goto end;
    }
    uint16_t len = strlen(value);
    len = len > CELL_VALUE_LEN_MAX ? CELL_VALUE_LEN_MAX : len;
    char *new_ptr = realloc(object->value.str, len + 1);
    if(new_ptr == NULL){
        res = -1;
        goto end;
    }
    object->value.str = new_ptr;
    memcpy(object->value.str, value, len);
    object->value.str[len] = '\0';
    object->value.len = len + 1;
    res = len;
    end:
    return res;
}

int cli_static_table_set_cell(StaticTableObject* object, uint32_t row, uint32_t column, CellObject* cell){
    int res = 0;
    if(object == NULL || cell == NULL){
        res = -1;
        goto end;
    }
    if(row >= object->rowMax || column >= object->columnMax){
        res = -2;
        goto end;
    }
    CellObject* temp = object->cellTable[row][column];
    if(temp != NULL)
        cell_delete(temp);
    object->cellTable[row][column] = cell;
    if(cell->value.len > 0){
        uint32_t columnWidth = cell->value.len;
        object->columnWidth[column] = object->columnWidth[column] > columnWidth ?
                object->columnWidth[column] : columnWidth;
    }
    end:
    return res;
}

CellObject* cli_static_table_get_cell(StaticTableObject* object, uint32_t row, uint32_t column){
    CellObject* cell = NULL;
    if(object == NULL)
        goto end;
    if(row >= object->rowMax || column >= object->columnMax)
        goto end;
    cell = object->cellTable[row][column];
    end:
    return cell;
}

int cli_static_table_delete_cell(StaticTableObject* object, uint32_t row, uint32_t column){
    CellObject* cell = NULL;
    int res = 0;
    if(object == NULL) {
        res = -1;
        goto end;
    }
    if(row >= object->rowMax || column >= object->columnMax) {
        res = -2;
        goto end;
    }
    if(object->cellTable[row][column] != NULL) {
        cell_delete(object->cellTable[row][column]);
        object->cellTable[row][column] = NULL;
    }
    end:
    return res;
}

void cli_static_table_delete(StaticTableObject* object){
    if(object) {
        if(object->cellTable){
            for (uint32_t i = 0; i < object->rowMax; i++) {
                if (object->cellTable[i] != NULL) {
                    for(uint32_t j = 0; j < object->columnMax; j++)
                        cell_delete(object->cellTable[i][j]);
                    free(object->cellTable[i]);
                }
            }
            free(object->cellTable);
        }
        if(object->columnWidth)
            free(object->columnWidth);
        free(object);
    }
}

StaticTableObject* cli_static_table_create(uint32_t row, uint32_t column){
    StaticTableObject* object = malloc(sizeof(StaticTableObject));
    if(object == NULL)
        goto end;
    object->columnWidth = NULL;
    object->columnMax = column;
    object->rowMax = row;

    //malloc row
    object->cellTable = calloc(row, sizeof(CellObject**));
    if(object->cellTable == NULL) {
        cli_static_table_delete(object);
        object = NULL;
        goto end;
    }

    //malloc column
    for(uint32_t i = 0; i < row; i++){
        object->cellTable[i] = calloc(column, sizeof(CellObject*));
        if(object->cellTable[i] == NULL) {
            cli_static_table_delete(object);
            object = NULL;
            goto end;
        }
    }

    object->columnWidth = malloc(column * sizeof(uint16_t));
    if(object->columnWidth == NULL) {
        cli_static_table_delete(object);
        object = NULL;
        goto end;
    }
    for(uint32_t i = 0; i < column; i++)
        object->columnWidth[i] = static_table_column_get_width(object, i);

    end:
    return object;
}

static void cli_static_table_print_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", CORNER_CHAR_LM);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", HLINE_CHAR);
        TABLE_PRINTF("%s", CORNER_CHAR_MM);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", CORNER_CHAR_RM);
}

static void cli_static_table_print_top_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", CORNER_CHAR_TL);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", HLINE_CHAR);
        TABLE_PRINTF("%s", CORNER_CHAR_TM);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", CORNER_CHAR_TR);
}

static void cli_static_table_print_bottom_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", CORNER_CHAR_BL);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", HLINE_CHAR);
        TABLE_PRINTF("%s", CORNER_CHAR_BM);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", CORNER_CHAR_BR);
}

static void cli_static_table_print_cell_line(StaticTableObject* object, uint32_t row) {
    char buff[10];
    memset(buff, 0 , 10);
    for(uint32_t i=0; i < object->columnMax; i++){
        snprintf(buff, 10, "%%%ds", object->columnWidth[i]);
        TABLE_PRINTF("%s", VLINE_CHAR);
        if(object->cellTable[row][i] != NULL){
            if(object->cellTable[row][i]->value.str)
                TABLE_PRINTF(buff, object->cellTable[row][i]->value.str);
        }else
            TABLE_PRINTF(buff, "");
    }
    TABLE_PRINTF("%s\n", VLINE_CHAR);
}

void cli_static_table_print(StaticTableObject* object){
    if(object) {
        cli_static_table_print_top_line(object);
        cli_static_table_print_cell_line(object, 0);
        for(uint32_t i = 1; i < object->rowMax; i++){
            cli_static_table_print_line(object);
            cli_static_table_print_cell_line(object, i);
        }
        cli_static_table_print_bottom_line(object);
    }
}

//
// Created by yushigengyu on 2021/4/30.
//
#include "cli_table.h"
static const char *version = "v0.5.0";

//type default
//╭────────┬───────┬───────┬───────┬───────┬───────╮
//│"name"  │"ch1"  │"ch2"  │"ch3"  │"ch4"  │"ch5"  │
//├────────┼───────┼───────┼───────┼───────┼───────┤
//│"enable"│"false"│"false"│"false"│"false"│"false"│
//├────────┼───────┼───────┼───────┼───────┼───────┤
//│"fre"   │"20000"│"20000"│"20000"│"20000"│"20000"│
//├────────┼───────┼───────┼───────┼───────┼───────┤
//│"point" │"8192" │"8192" │"8192" │"8192" │"8192" │
//├────────┼───────┼───────┼───────┼───────┼───────┤
//│"cutoff"│"15000"│"15000"│"15000"│"15000"│"15000"│
//╰────────┴───────┴───────┴───────┴───────┴───────╯

static const char* table_print_char_default[] = {
        "─" , "│", "┬", "┴", "┼", "├", "┤", "╭", "╮", "╰", "╯"
};

//type2
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
static const char* table_print_char_type2[CHAR_MAX] = {
        "━" , "┃", "┳", "┻", "╋", "┣", "┫", "┏", "┓", "┗", "┛"
};

//type3
//╔════════╦═══════╦═══════╦═══════╦═══════╦═══════╗
//║"name"  ║"ch1"  ║"ch2"  ║"ch3"  ║"ch4"  ║"ch5"  ║
//╠════════╬═══════╬═══════╬═══════╬═══════╬═══════╣
//║"enable"║"false"║"false"║"false"║"false"║"false"║
//╠════════╬═══════╬═══════╬═══════╬═══════╬═══════╣
//║"fre"   ║"20000"║"20000"║"20000"║"20000"║"20000"║
//╠════════╬═══════╬═══════╬═══════╬═══════╬═══════╣
//║"point" ║"8192" ║"8192" ║"8192" ║"8192" ║"8192" ║
//╠════════╬═══════╬═══════╬═══════╬═══════╬═══════╣
//║"cutoff"║"15000"║"15000"║"15000"║"15000"║"15000"║
//╚════════╩═══════╩═══════╩═══════╩═══════╩═══════╝
static const char* table_print_char_type3[CHAR_MAX] = {
        "═" , "║", "╦", "╩", "╬", "╠", "╣", "╔", "╗", "╚", "╝"
};


static int cell_init(CellObject *object, const char *value, uint16_t len) {
    int res = 0;
    if(value != NULL) {
        object->value.len = len;
        object->value.str = malloc(object->value.len + 1);
        if(object->value.str == NULL){
            res = -1;
            goto end;
        }
        memcpy(object->value.str, value, object->value.len);
        object->value.str[len] = '\0';
    }else {
        object->value.len = 0;
        object->value.str = NULL;
    }
    object->alignment = ALIGNMENT_LEFT;
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

static uint32_t static_table_row_width_get(StaticTableObject* object, uint32_t row){
    uint32_t width = 0;
    for(uint32_t i = 0; i < object->columnMax; i++)
        if(object->cellTable[row][i] != NULL)
            width += (object->cellTable[row][i]->value.len + 1);
        else
            width += 1;
    width += 1;
    return width;
}

static uint32_t static_table_column_width_get(StaticTableObject* object, uint32_t column){
    uint32_t width = 0;
    for(uint32_t i = 0; i < object->rowMax; i++){
        if(object->cellTable[i][column] != NULL)
            if(object->cellTable[i][column]->value.len > width)
                width = object->cellTable[i][column]->value.len;
    }
    return width;
}

CellObject *cell_create(const char *value, uint16_t len) {
    CellObject* object = calloc(1, sizeof(CellObject));
    if(object == NULL)
        goto end;
    if(cell_init(object, value, len) != 0){
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

int cell_value_set(CellObject *object, const char *value, uint16_t len) {
    int res = 0;
    if(value == NULL || object == NULL){
        res = -1;
        goto end;
    }
    len = len > CELL_VALUE_LEN_MAX ? CELL_VALUE_LEN_MAX : len;
    char *new_ptr = realloc(object->value.str, len + 1);
    if(new_ptr == NULL){
        res = -1;
        goto end;
    }
    object->value.str = new_ptr;
    memcpy(object->value.str, value, len);
    object->value.str[len] = '\0';
    object->value.len = len;
    res = len;
    end:
    return res;
}

CellValue* cell_value_get(CellObject *object){
    CellValue* cellValue = NULL;
    if(object)
        cellValue = &object->value;
    return cellValue;
}

int cell_align_set(CellObject* object, TABLE_ALIGNMENT align){
    int res = 0;
    if(align > ALIGNMENT_CENTER || object == NULL){
        res = -1;
        goto end;
    }
    object->alignment = align;
    end:
    return res;
}

TABLE_ALIGNMENT cell_align_get(CellObject* object){
    return object->alignment;
}

int cli_static_table_cell_set(StaticTableObject* object, uint32_t row, uint32_t column, CellObject* cell){
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

CellObject* cli_static_table_cell_get(StaticTableObject* object, uint32_t row, uint32_t column){
    CellObject* cell = NULL;
    if(object == NULL)
        goto end;
    if(row >= object->rowMax || column >= object->columnMax)
        goto end;
    cell = object->cellTable[row][column];
    end:
    return cell;
}

void cli_static_table_printtype_set(StaticTableObject* object, TABLE_PRINT_TYPE type, const char* charType[CHAR_MAX]) {
    if(object){
        switch(type){
            case PRINT_TYPE_DEFAULT:
                for(TABLE_PRINT_CHAR i = HLINE_CHAR; i < CHAR_MAX; i++)
                    object->printChar[i] = table_print_char_default[i];
                break;
            case PRINT_TYPE_2:
                for(TABLE_PRINT_CHAR i = HLINE_CHAR; i < CHAR_MAX; i++)
                    object->printChar[i] = table_print_char_type2[i];
                break;
            case PRINT_TYPE_3:
                for(TABLE_PRINT_CHAR i = HLINE_CHAR; i < CHAR_MAX; i++)
                    object->printChar[i] = table_print_char_type3[i];
                break;
            case PRINT_TYPE_CUSTOM:
                for(TABLE_PRINT_CHAR i = HLINE_CHAR; i < CHAR_MAX; i++){
                    if(charType[i])
                        object->printChar[i] = charType[i];
                    else
                        object->printChar[i] = table_print_char_default[i];
                }
                break;
            default:
                break;
        }
        object->printType = type;
    }
}
TABLE_PRINT_TYPE cli_static_table_printtype_get(StaticTableObject* object){
    TABLE_PRINT_TYPE type = -1;
    if(object){
        type = object->printType;
    }
    return type;
}

uint32_t cli_static_table_row_get(StaticTableObject* object){
    uint32_t row = 0;
    if(object)
        row = object->rowMax;
    return row;
}

uint32_t cli_static_table_column_get(StaticTableObject* object){
    uint32_t column= 0;
    if(object)
        column = object->columnMax;
    return column;
}

int cli_static_table_cell_delete(StaticTableObject* object, uint32_t row, uint32_t column){
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

StaticTableObject* cli_static_table_create(uint32_t row, uint32_t column){
    StaticTableObject* object = malloc(sizeof(StaticTableObject));
    if(object == NULL)
        goto end;
    object->columnWidth = NULL;
    object->columnMax = column;
    object->rowMax = row;
    cli_static_table_printtype_set(object, PRINT_TYPE_DEFAULT, NULL);

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
        object->columnWidth[i] = static_table_column_width_get(object, i);

    end:
    return object;
}

StaticTableObject* cli_static_table_csv_str_create(const char* csvStr) {
    StaticTableObject* sTable = NULL;
    CSV_STRUCT* csv = csv_parser(csvStr);
    if(csv == NULL)
        goto end;
    uint16_t columnMax = 0;
    for(uint16_t i = 0; i < csv->row; i++){
        if(columnMax < csv->rowFieldCount[i])
            columnMax = csv->rowFieldCount[i];
    }
    sTable = cli_static_table_create(csv->row, columnMax);
    if(sTable == NULL){
        csv_delete(csv);
        goto end;
    }
    for(uint16_t i = 0; i < csv->row; i++){
        for(uint16_t j = 0; j < csv->rowFieldCount[i]; j++){
            CSV_STRUCT_FIELD* field = csv_field_get(csv, i , j);
            if(field != NULL) {
                CellObject *cell = cell_create(field->value, field->len);
                if (cell == NULL) {
                    cli_static_table_delete(sTable);
                    sTable = NULL;
                    goto end;
                }
                cli_static_table_cell_set(sTable, i, j, cell);
            }
        }
    }
    end:
    return sTable;
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

static void cli_static_table_print_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_LM]);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", object->printChar[HLINE_CHAR]);
        TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_MM]);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", object->printChar[CORNER_CHAR_RM]);
}

static void cli_static_table_print_top_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_TL]);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", object->printChar[HLINE_CHAR]);
        TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_TM]);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", object->printChar[CORNER_CHAR_TR]);
}

static void cli_static_table_print_bottom_line(StaticTableObject* object) {
    TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_BL]);
    for(uint32_t i=0; i < object->columnMax; i++) {
        for(uint16_t j=0; j < object->columnWidth[i]; j++)
            TABLE_PRINTF("%s", object->printChar[HLINE_CHAR]);
        TABLE_PRINTF("%s", object->printChar[CORNER_CHAR_BM]);
    }
    TABLE_PRINTF("%c", BACK_SPACE);
    TABLE_PRINTF("%s\n", object->printChar[CORNER_CHAR_BR]);
}

static void print_cell(CellObject* object, uint16_t columnWidth){
    uint16_t space;
    if(object->value.str){
        switch(object->alignment){
            case ALIGNMENT_LEFT:
                space = 0;
                break;
            case ALIGNMENT_RIGHT:
                space = columnWidth - object->value.len;
                break;
            case ALIGNMENT_CENTER:
                space = ((uint16_t)(columnWidth-object->value.len)>>1U);
                break;
            default:
                space = 0;
                break;
        }
        for(uint16_t i=0; i < space; i++)
            TABLE_PRINTF(" ");

        TABLE_PRINTF("%s", object->value.str);

        switch(object->alignment){
            case ALIGNMENT_LEFT:
                space = columnWidth - object->value.len;
                break;
            case ALIGNMENT_RIGHT:
                space = 0;
                break;
            case ALIGNMENT_CENTER:
                space = columnWidth - space - object->value.len;
                break;
            default:
                space = columnWidth - object->value.len;
                break;
        }
        for(uint16_t i=0; i < space; i++)
            TABLE_PRINTF(" ");
    } else
        for(uint16_t i=0; i < columnWidth; i++)
            TABLE_PRINTF(" ");

}

static void cli_static_table_print_cell_line(StaticTableObject* object, uint32_t row) {
    for(uint32_t i=0; i < object->columnMax; i++){
        uint16_t columnWidth = object->columnWidth[i];
        TABLE_PRINTF("%s", object->printChar[VLINE_CHAR]);
        if(object->cellTable[row][i] != NULL) {
            print_cell(object->cellTable[row][i], columnWidth);
        }else
            for(uint16_t j=0; j<columnWidth; j++)
                TABLE_PRINTF(" ");
    }
    TABLE_PRINTF("%s\n", object->printChar[VLINE_CHAR]);
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

const char *c_cli_table_version_get(){
    return version;
}

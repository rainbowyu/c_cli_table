//
// Created by yushigengyu on 2021/5/18.
//
#include "cli_table.h"
static const char* tableStr[5][6] = {
        {"name", "ch1", "ch2", "ch3", "ch4", "ch5"},
        {"enable", "false", "false", "false", "false", "false"},
        {"fre", "20000", "20000", "20000", "20000", "20000"},
        {"point", "8192", "8192", "8192", "8192", "8192"},
        {"cutoff", "15000", "15000", "15000", "15000", "15000"},
};

static const char* csvString =
        "\"name\",\"ch1\",\"ch2\",\"ch3\",\"ch4\",\"ch5\"\n"
        "\"enable\",\"false\",\"false\",\"false\",\"false\",\"false\"\n"
        "\"fre\",\"20000\",\"20000\",\"20000\",\"20000\",\"20000\"\n"
        "\"point\",\"8192\",\"8192\",\"8192\",\"8192\",\"8192\"\n"
        "\"cutoff\",\"15000\",\"15000\",\"15000\",\"15000\",\"15000\"";

int test_cli_static_table_create(void){
    StaticTableObject* table;
    printf("\n");
    printf("=====================cli_static_table_create=======================\n");
    table = cli_static_table_create(5, 6);
    if(table != NULL) {
        for(uint32_t j =0; j < 6; j++){
            CellObject* cell = cell_create(tableStr[0][j], strlen(tableStr[0][j]));
            cell_set_align(cell, ALIGNMENT_CENTER);
            cli_static_table_set_cell(table, 0, j, cell);
        }
        for(uint32_t i = 1; i < 5; i++){
            for(uint32_t j =0; j < 6; j++){
                CellObject* cell = cell_create(tableStr[i][j], strlen(tableStr[i][j]));
                cell_set_align(cell, ALIGNMENT_LEFT);
                cli_static_table_set_cell(table, i, j, cell);
            }
        }
        cli_static_table_print(table);
        cli_static_table_delete(table);
    }
    return 0;
};

int test_cli_static_table_csv_create(void){
    printf("\n");
    printf("=====================test_cli_static_table_csv_create=======================\n");
    StaticTableObject* sTable = cli_static_table_csv_str_create(csvString);
    if(sTable){
        cli_static_table_print(sTable);
        cli_static_table_delete(sTable);
    }
    return 0;
}

#include <stdio.h>
#include "cli_table.h"
const char* tableStr[5][6] = {
        {"name", "ch1", "ch2", "ch3", "ch4", "ch5"},
        {"enable", "false", "false", "false", "false", "false"},
        {"fre", "20000", "20000", "20000", "20000", "20000"},
        {"point", "8192", "8192", "8192", "8192", "8192"},
        {"cutoff", "15000", "15000", "15000", "15000", "15000"},
};

int main() {
    StaticTableObject* table;
    table = cli_static_table_create(5, 6);
    if(table != NULL) {
        for(uint32_t i = 0; i < 5; i++){
            for(uint32_t j =0; j < 6; j++){
                CellObject* cell = cell_create(tableStr[i][j]);
                cli_static_table_set_cell(table, i, j, cell);
            }
        }
        cli_static_table_print(table);
        cli_static_table_delete(table);
    }
    return 0;
}

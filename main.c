#include <stdio.h>
#include "cli_table.h"
const char* tableStr[5][6] = {
        {"name", "ch1", "ch2", "ch3", "ch4", "ch5"},
        {"enable", "0", "1", "2", "3", "4"},
        {"fre", "20000", "20001", "20002", "20003", "20004"},
        {"point", "8192", "8193", "8194", "8195", "8196"},
        {"cutoff", "15000", "15001", "15002", "15003", "15004"},
};

int main() {
    StaticTableObject* table;
    table = cli_static_table_create(5, 6, **tableStr);
    if(table != NULL) {
        cli_static_table_print(table);
        cli_static_table_delete(table);
    }
    return 0;
}

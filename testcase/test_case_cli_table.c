//
// Created by yushigengyu on 2021/5/18.
//
#include "cli_table.h"
#include "unit_test.h"
static const char* tableStr[5][6] = {
        {"name", "ch1", "ch2", "ch3", "ch4", "ch5"},
        {"enable", "false", "false", "false", "false", "false"},
        {"fre", "20000", "20000", "20000", "20000", "20000"},
        {"point", "8192", "8192", "8192", "8192", "8192"},
        {"cutoff", "15000", "15000", "15000", "15000", "15000"},
};

int test_cli_static_table_create(void){
    StaticTableObject* table;
    printf("\n");
    printf("=====================cli_static_table_create=======================\n");
    table = cli_static_table_create(5, 6);
    unit_test_assert_int_equal(cli_static_table_row_get(table), 5);
    unit_test_assert_int_equal(cli_static_table_column_get(table), 6);
    for(uint32_t j =0; j < 6; j++){
        uint16_t len = strlen(tableStr[0][j]);
        CellObject* cell = cell_create(tableStr[0][j], len);
        unit_test_assert_not_null(cell);
        unit_test_assert_not_null(cell->value.str);
        unit_test_assert_int_equal(cell->value.len, len);
        unit_test_assert_str_equal(cell->value.str, tableStr[0][j], len);
        cell_align_set(cell, ALIGNMENT_CENTER);
        cli_static_table_cell_set(table, 0, j, cell);
        unit_test_assert_pointer_equal(cli_static_table_cell_get(table, 0, j), cell);
    }
    for(uint32_t i = 1; i < 5; i++){
        for(uint32_t j =0; j < 6; j++){
            uint16_t len = strlen(tableStr[i][j]);
            CellObject* cell = cell_create(tableStr[i][j], len);
            unit_test_assert_not_null(cell);
            unit_test_assert_not_null(cell->value.str);
            unit_test_assert_int_equal(cell->value.len, len);
            unit_test_assert_str_equal(cell->value.str, tableStr[i][j], len);
            cell_align_set(cell, ALIGNMENT_LEFT);
            cli_static_table_cell_set(table, i, j, cell);
            unit_test_assert_pointer_equal(cli_static_table_cell_get(table, i, j), cell);
        }
    }
    cli_static_table_delete(table);
    return 0;
};

static const char* csvString =
        "\"name\",\"ch1\",\"ch2\",\"ch3\",\"ch4\",\"ch5\"\n"
        "\"enable\",\"false\",\"false\",\"false\",\"false\",\"false\"\n"
        "\"fre\",\"20000\",\"20000\",\"20000\",\"20000\",\"20000\"\n"
        "\"point\",\"8192\",\"8192\",\"8192\",\"8192\",\"8192\"\n"
        "\"cutoff\",\"15001\",\"15002\",\"15003\",\"15004\",\"15005\"";

int test_cli_static_table_csv_create(void){
    CellObject* cell;
    printf("\n");
    printf("=====================test_cli_static_table_csv_create=======================\n");
    StaticTableObject* sTable = cli_static_table_csv_str_create(csvString);
    unit_test_assert_not_null(sTable);
    unit_test_assert_int_equal(cli_static_table_row_get(sTable), 5);
    unit_test_assert_int_equal(cli_static_table_column_get(sTable), 6);

    cell = cli_static_table_cell_get(sTable, 0, 0);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"name\"", sizeof("\"name\""));

    cell = cli_static_table_cell_get(sTable, 1, 0);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"enable\"", sizeof("\"enable\""));

    cell = cli_static_table_cell_get(sTable, 4, 1);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"15001\"", sizeof("\"15001\""));

    cell = cli_static_table_cell_get(sTable, 4, 2);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"15002\"", sizeof("\"15002\""));

    cell = cli_static_table_cell_get(sTable, 4, 3);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"15003\"", sizeof("\"15003\""));

    cell = cli_static_table_cell_get(sTable, 4, 4);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"15004\"", sizeof("\"15004\""));

    cell = cli_static_table_cell_get(sTable, 4, 5);
    unit_test_assert_not_null(cell);
    unit_test_assert_str_equal(cell->value.str, "\"15005\"", sizeof("\"15005\""));
    cli_static_table_delete(sTable);
    return 0;
}

static const char* printChar[] = {
        "-" , "|", "┬", "┴", "+", "├", "┤", "-", "-", "-", "-"
};

int test_cli_static_table_print(void){
    printf("\n");
    printf("=====================test_cli_static_table_csv_create=======================\n");
    StaticTableObject* sTable = cli_static_table_csv_str_create(csvString);
    unit_test_assert_not_null(sTable);
    printf("default type:\n");
    cli_static_table_printtype_set(sTable, PRINT_TYPE_DEFAULT, NULL);
    cli_static_table_print(sTable);
    printf("type2:\n");
    cli_static_table_printtype_set(sTable, PRINT_TYPE_2, NULL);
    cli_static_table_print(sTable);
    printf("type3:\n");
    cli_static_table_printtype_set(sTable, PRINT_TYPE_3, NULL);
    cli_static_table_print(sTable);
    printf("custom type:\n");
    cli_static_table_printtype_set(sTable, PRINT_TYPE_CUSTOM, printChar);
    unit_test_assert_int_equal(cli_static_table_printtype_get(sTable), PRINT_TYPE_CUSTOM);
    cli_static_table_print(sTable);
    cli_static_table_delete(sTable);
    return 0;
}

const char* cellValue = "test\ncell\"\"";
const char* cellValue1 = "test\ncell12354789\"\"";
int test_table_cell(void){
    printf("\n");
    printf("=====================test_table_cell=======================\n");
    CellObject* cell = cell_create(cellValue, strlen(cellValue));
    unit_test_assert_not_null(cell);
    CellValue* value = cell_value_get(cell);
    unit_test_assert_not_null(value);
    unit_test_assert_int_equal(value->len, strlen(cellValue));
    unit_test_assert_str_equal(value->str, cellValue, value->len);
    cell_value_set(cell, cellValue1, strlen(cellValue1));
    value = cell_value_get(cell);
    unit_test_assert_not_null(value);
    unit_test_assert_int_equal(value->len, strlen(cellValue1));
    unit_test_assert_str_equal(value->str, cellValue1, value->len);
    cell_delete(cell);
    return 0;
}

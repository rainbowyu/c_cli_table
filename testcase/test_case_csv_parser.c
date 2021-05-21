//
// Created by yushigengyu on 2021/5/17.
//
#include "csv_parser.h"
#include "unit_test.h"
static const char* csvString =
        "foo,bar,baz\n"                                 //000 foo,bar,baz
        "foo,\"bar\",baz\n"                             //001 foo,"bar",baz
        "foo,\"b\"\"ar\",baz\n"                         //002 foo,"b""ar",baz
        "foo,\"b\na\nr\",baz\n"                         //003 foo,"b\na\nr",baz
        "foo,\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\",baz\n"     //004 foo,"\n\nb\n\n""a""\n\nr\n\n",baz
        "foo,bar,\"baz\"\n"                             //005 foo,bar,"baz"
        "foo,bar,\"\"baz\"\"\n"                         //006 foo,bar,""baz""
        "foo,bar,\"\"\"baz\"\"\"\n"                     //007 foo,bar,"""baz""",
        "foo,bar,baz";                                  //008 foo,bar,baz

int test_csv_split_on_newline(void){
    printf("\n");
    printf("=====================test_csv_split_on_newline=======================\n");
    CSV_STRUCT_SPLIT_LINE* csv_line_object = csv_split_on_lines_create(csvString);
    unit_test_assert_not_null(csv_line_object);
    unit_test_assert_int_equal(csv_line_object->row, 9);
    unit_test_assert_str_equal(csv_line_object->value[0], "foo,bar,baz", strlen("foo,bar,baz"));
    unit_test_assert_str_equal(csv_line_object->value[1], "foo,\"bar\",baz", strlen("foo,\"bar\",baz"));
    unit_test_assert_str_equal(csv_line_object->value[2], "foo,\"b\"\"ar\",baz", strlen("foo,\"b\"\"ar\",baz"));
    unit_test_assert_str_equal(csv_line_object->value[3], "foo,\"b\na\nr\",baz", strlen("foo,\"b\na\nr\",baz"));
    unit_test_assert_str_equal(csv_line_object->value[4], "foo,\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\",baz", strlen("foo,\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\",baz"));
    unit_test_assert_str_equal(csv_line_object->value[5], "foo,bar,\"baz\"", strlen("foo,bar,\"baz\"")-1);
    unit_test_assert_str_equal(csv_line_object->value[6], "foo,bar,\"\"baz\"\"", strlen("foo,bar,\"\"baz\"\""));
    unit_test_assert_str_equal(csv_line_object->value[7], "foo,bar,\"\"\"baz\"\"\"", strlen("foo,bar,\"\"\"baz\"\"\""));
    unit_test_assert_str_equal(csv_line_object->value[8], "foo,bar,baz", strlen("foo,bar,baz"));
    csv_split_on_lines_delete(csv_line_object);
    return 0;
}

int test_csv_parse(void){
    printf("\n");
    printf("=====================test_csv_parse=======================\n");
    CSV_STRUCT* csv_object = csv_parser(csvString);
    unit_test_assert_not_null(csv_object);
    unit_test_assert_int_equal(csv_object->row, 9);
    unit_test_assert_not_null(csv_object->rowFieldCount);
    unit_test_assert_not_null(csv_object->field);
    for(uint16_t i = 0; i < csv_object->row; i++) {
        unit_test_assert_int_equal(csv_object->rowFieldCount[i], 3);
        unit_test_assert_not_null(csv_object->field[i]);
        for(uint16_t j = 0; j < csv_object->rowFieldCount[i]; j++)
            unit_test_assert_not_null(csv_object->field[i][j]);
    }
    unit_test_assert_int_equal(csv_object->field[0][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[0][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[0][1]->len, strlen("bar"));
    unit_test_assert_str_equal(csv_object->field[0][1]->value, "bar", strlen("bar"));
    unit_test_assert_int_equal(csv_object->field[0][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[0][2]->value, "baz", strlen("baz"));

    unit_test_assert_int_equal(csv_object->field[1][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[1][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[1][1]->len, strlen("\"bar\""));
    unit_test_assert_str_equal(csv_object->field[1][1]->value, "\"bar\"", strlen("\"bar\""));
    unit_test_assert_int_equal(csv_object->field[1][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[1][2]->value, "baz", strlen("baz"));

    unit_test_assert_int_equal(csv_object->field[2][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[2][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[2][1]->len, strlen("\"b\"\"ar\""));
    unit_test_assert_str_equal(csv_object->field[2][1]->value, "\"b\"\"ar\"", strlen("\"b\"\"ar\""));
    unit_test_assert_int_equal(csv_object->field[2][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[2][2]->value, "baz", strlen("baz"));

    unit_test_assert_int_equal(csv_object->field[3][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[3][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[3][1]->len, strlen("\"b\na\nr\""));
    unit_test_assert_str_equal(csv_object->field[3][1]->value, "\"b\na\nr\"", strlen("\"b\na\nr\""));
    unit_test_assert_int_equal(csv_object->field[3][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[3][2]->value, "baz", strlen("baz"));

    unit_test_assert_int_equal(csv_object->field[4][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[4][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[4][1]->len, strlen("\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\""));
    unit_test_assert_str_equal(csv_object->field[4][1]->value, "\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\"", strlen("\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\""));
    unit_test_assert_int_equal(csv_object->field[4][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[4][2]->value, "baz", strlen("baz"));

    unit_test_assert_int_equal(csv_object->field[5][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[5][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[5][1]->len, strlen("bar"));
    unit_test_assert_str_equal(csv_object->field[5][1]->value, "bar", strlen("bar"));
    unit_test_assert_int_equal(csv_object->field[5][2]->len, strlen("\"baz\""));
    unit_test_assert_str_equal(csv_object->field[5][2]->value, "\"baz\"", strlen("\"baz\""));

    unit_test_assert_int_equal(csv_object->field[6][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[6][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[6][1]->len, strlen("bar"));
    unit_test_assert_str_equal(csv_object->field[6][1]->value, "bar", strlen("bar"));
    unit_test_assert_int_equal(csv_object->field[6][2]->len, strlen("\"\"baz\"\""));
    unit_test_assert_str_equal(csv_object->field[6][2]->value, "\"\"baz\"\"", strlen("\"\"baz\"\""));

    unit_test_assert_int_equal(csv_object->field[7][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[7][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[7][1]->len, strlen("bar"));
    unit_test_assert_str_equal(csv_object->field[7][1]->value, "bar", strlen("bar"));
    unit_test_assert_int_equal(csv_object->field[7][2]->len, strlen("\"\"\"baz\"\"\""));
    unit_test_assert_str_equal(csv_object->field[7][2]->value, "\"\"\"baz\"\"\"", strlen("\"\"\"baz\"\"\""));

    unit_test_assert_int_equal(csv_object->field[8][0]->len, strlen("foo"));
    unit_test_assert_str_equal(csv_object->field[8][0]->value, "foo", strlen("foo"));
    unit_test_assert_int_equal(csv_object->field[8][1]->len, strlen("bar"));
    unit_test_assert_str_equal(csv_object->field[8][1]->value, "bar", strlen("bar"));
    unit_test_assert_int_equal(csv_object->field[8][2]->len, strlen("baz"));
    unit_test_assert_str_equal(csv_object->field[8][2]->value, "baz", strlen("baz"));
    csv_delete(csv_object);
    return 0;
}

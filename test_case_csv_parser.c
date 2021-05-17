//
// Created by yushigengyu on 2021/5/17.
//
#include "csv_parser.h"
const char* csvString =
        "foo,bar,baz\nfoo,\"bar\",baz\nfoo,\"b\"\"ar\",baz\nfoo,\"b\na\nr\",baz\nfoo,\"\n\nb\n\n\"\"a\"\"\n\nr\n\n\",baz\nfoo,bar,\"baz\"\nfoo,bar,\"\"baz\"\"\nfoo,bar,\"\"\"baz\"\"\"\nfoo,bar,baz";

int test_csv_split_on_newline() {
    CSV_STRUCT_SPLIT_LINE* csv_line_object = csv_split_on_newlines(csvString);
    if(csv_line_object != NULL){
        printf("ok");
    }
    return 0;
}
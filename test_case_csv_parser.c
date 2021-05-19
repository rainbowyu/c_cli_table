//
// Created by yushigengyu on 2021/5/17.
//
#include "csv_parser.h"
const char* csvString =
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
    CSV_STRUCT_SPLIT_LINE* csv_line_object = csv_split_on_lines_create(csvString);
    if(csv_line_object){
        printf("\n");
        printf("=====================test_csv_split_on_newline=======================\n");
        printf("split on newlines, row:%d\n", csv_line_object->row);
        for(uint16_t i = 0; i < csv_line_object->row; i++){
            printf("[%02d]:", i);
            for(uint16_t j = 0; j < csv_line_object->rowLenArray[i]; j++){
                if(csv_line_object->value[i][j] == '\n')
                    printf("\\n");
                else
                    printf("%c", csv_line_object->value[i][j]);
            }
            printf("\n");
        }
        csv_split_on_lines_delete(csv_line_object);
    }
    return 0;
}

int test_csv_parse(void){
    CSV_STRUCT* csv_object = csv_parser(csvString);
    if(csv_object){
        printf("\n");
        printf("=====================test_csv_parse=======================\n");
        printf("csv date print, row :%d\n", csv_object->row);
        for(uint16_t i = 0; i < csv_object->row; i++){
            if(csv_object->rowFieldCount) {
                for(uint16_t j = 0; j < csv_object->rowFieldCount[i]; j++){
                    if(csv_object->field[i][j]){
                        printf("[%d][%d]:", i,j);
                        for (uint16_t k = 0; k < csv_object->field[i][j]->len; k++) {
                            if(csv_object->field[i][j]->value[k] == '\n')
                                printf("\\n");
                            else
                                printf("%c", csv_object->field[i][j]->value[k]);
                        }
                        printf(" ");
                    }else
                        printf("csv_object->field[%d][%d] is NULL\n", i, j);
                }
                printf("\n");
            } else
                printf("csv_object->rowFieldCount is NULL\n");
        }
        csv_delete(csv_object);
    }
    return 0;
}

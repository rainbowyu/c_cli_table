//
// Created by yushigengyu on 2021/5/14.
//
//#define DEBUG
#include "csv_parser.h"

static CSV_STRUCT_FIELD* csv_fields_create(uint16_t len, const char* value){
    CSV_STRUCT_FIELD* field = malloc(sizeof(CSV_STRUCT_FIELD));
    if(field == NULL)
        goto end;
    field->value = value;
    field->len = len;
    end:
    return field;
}

static uint16_t csv_count_lines(const char *csvStr){
    const char *ptr;
    uint8_t flag;
    uint16_t lines;
    for (lines = 1, ptr = csvStr, flag = 0; *ptr; ptr++){
        if(flag){
            if(*ptr == '\"'){
                if (ptr[1] == '\"'){
                    ptr++;
                    continue;
                }
                flag = 0;
            }
        }else if(*ptr == '\"'){
            flag = 1;
        }else if(*ptr == '\n'){
            lines++;
        }
    }
    return lines;
}

CSV_STRUCT_SPLIT_LINE* csv_split_on_lines_create(const char *csvStr){
    const char *ptr, *lineStart;
    CSV_STRUCT_SPLIT_LINE* csvSplitLine = NULL;
    uint8_t flag;
    uint16_t lines, linesCount;

    csvSplitLine = calloc(1, sizeof(CSV_STRUCT_SPLIT_LINE));
    if(csvSplitLine == NULL)
        goto end;

    //get how many row we need
    lines = csv_count_lines(csvStr);
    csvSplitLine->row = lines;
#ifdef DEBUG
    printf("csv lines: %d\n", lines);
#endif
    csvSplitLine->value = calloc(lines, sizeof(char*));
    if (csvSplitLine->value == NULL) {
        free(csvSplitLine);
        goto end;
    }
    csvSplitLine->rowLenArray = calloc(lines, sizeof(uint16_t));
    if (csvSplitLine->rowLenArray == NULL) {
        free(csvSplitLine->value);
        free(csvSplitLine);
        goto end;
    }

    //指针对应数据，不保存额外字符
    lineStart = csvStr;
    csvSplitLine->value[0] = lineStart;
    ptr = csvStr;
    flag = 0;
    linesCount = 0;
    for(;;ptr++){
        if (flag){
            if(*ptr == '\"'){
                if (ptr[1] == '\"'){
                    ptr++;
                    continue;
                }
                flag = 0;
                continue;
            }else if(*ptr)
                continue;
        }

        if(*ptr == '\"')
            flag = 1;
        else if(*ptr == '\n' || !*ptr){    //end of line
            size_t lineStrLen = ptr - lineStart;
            if(lineStrLen == 0){ //空行，parse over
#ifdef DEBUG
                printf("space lines, parse over\n");
#endif
                goto end;
            }
            csvSplitLine->rowLenArray[linesCount] = lineStrLen;
#ifdef DEBUG
            printf("%03d:", linesCount);
            for(uint16_t i = 0; i < csvSplitLine->rowLenArray[linesCount]; i++)
                printf("%c", lineStart[i]);
            printf("\n");
#endif
            if(*ptr){
                lineStart = ptr + 1;
                linesCount++;
                csvSplitLine->value[linesCount] = lineStart;
            }else   //end of str, return
                goto end;
        }
    }
    end:
    return csvSplitLine;
}

void csv_split_on_lines_delete(CSV_STRUCT_SPLIT_LINE* csvSLineObject){
    if(csvSLineObject){
        if(csvSLineObject->value)
            free(csvSLineObject->value);
        if(csvSLineObject->rowLenArray)
            free(csvSLineObject->rowLenArray);
    }
}

CSV_STRUCT* csv_parser(const char* csvStr){
    CSV_STRUCT* csv = NULL;
    uint8_t errFlag = 0;
    CSV_STRUCT_SPLIT_LINE* csvSplitLine = csv_split_on_lines_create(csvStr);
    if(csvSplitLine == NULL) {
        errFlag = 1;
        goto end;
    }

    csv = calloc(1, sizeof(CSV_STRUCT));
    if(csv == NULL) {
        errFlag = 1;
        goto end;
    }

    csv->row = csvSplitLine->row;
    csv->rowFieldCount = calloc(csv->row, sizeof(uint16_t));
    csv->field = calloc(csv->row, sizeof(CSV_STRUCT_FIELD**));
    if(csv->rowFieldCount == NULL || csv->field == NULL){
        errFlag = 1;
        goto end;
    }

    for(uint16_t i = 0; i < csvSplitLine->row; i++){
        //get this row count
        uint16_t cnt = 1;
        uint8_t flag = 0;
        const char *ptr = csvSplitLine->value[i];       //line start char
        uint16_t len = csvSplitLine->rowLenArray[i];    //line char len
        for(uint16_t j = 0; j < len; j++, ptr++){
            if(flag){
                if(*ptr == '\"'){
                    if(*(ptr+1) == '\"'){
                        ptr++;
                        continue;
                    }
                    flag = 0;
                }
                continue;
            }

            switch(*ptr){
                case '\"':
                    flag = 1;
                    break;
                case ',':
                    cnt++;
                    break;
                default:
                    break;
            }
        }
        if(flag){   //引号不配对
            errFlag = 1;
            goto end;
        }

        //malloc field array
        csv->field[i] = calloc(cnt, sizeof(CSV_STRUCT_FIELD*));
        if(csv->field[i] == NULL){
            errFlag = 1;
            goto end;
        }
        csv->rowFieldCount[i] = cnt;

        //拆分field
        const char *startStr = csvSplitLine->value[i];
        int16_t fieldLen = 0;
        flag = 0;
        cnt = 0;
        ptr = csvSplitLine->value[i];       //line start char
        len = csvSplitLine->rowLenArray[i]; //line char len
        for(uint16_t j = 0; j < len; j++, ptr++, fieldLen++){
            if(flag){
                if(*ptr == '\"'){
                    if(*(ptr+1) == '\"'){
                        ptr++;
                        fieldLen++;
                        j++;
                        continue;
                    }
                    flag = 0;
                }
                continue;
            }

            switch(*ptr){
                case '\"':
                    flag = 1;
                    break;
                case ',':
                    csv->field[i][cnt] = csv_fields_create(fieldLen, startStr);
                    if(csv->field[i][cnt] == NULL){
                        errFlag = 1;
                        goto end;
                    }
                    fieldLen = -1;
                    startStr = ptr+1;
                    cnt++;
                    break;
                default:
                    break;
            }
        }
        csv->field[i][cnt] = csv_fields_create(fieldLen, startStr);
    }

    end:
    if(errFlag){
        if(csv){
            csv_delete(csv);
            csv = NULL;
        }
    }
    if(csvSplitLine)
        free(csvSplitLine);
    return csv;
}

void csv_delete(CSV_STRUCT *csv) {
    if(csv){
        if (csv->field) {
            for (uint16_t i = 0; i < csv->row; i++) {
                if (csv->field[i]) {
                    for (uint16_t j = 0; j < csv->rowFieldCount[i]; j++) {
                        if (csv->field[i][j])
                            free(csv->field[i][j]);
                    }
                    free(csv->field[i]);
                }
            }
            free(csv->field);
        }
        if (csv->rowFieldCount)
            free(csv->rowFieldCount);
        free(csv);
    }
}

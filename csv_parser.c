//
// Created by yushigengyu on 2021/5/14.
//
#define DEBUG
#include "csv_parser.h"
static uint16_t csv_count_fields(const char *line){
    const char *ptr = line;
    uint16_t cnt = 1;
    uint8_t flag = 0;

    while(*ptr != '\0'){
        if (flag) {
            if (*ptr == '\"'){
                if (*(ptr+1) == '\"'){
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

    if (flag)
        cnt = -1;
    return cnt;
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

CSV_STRUCT_SPLIT_LINE* csv_split_on_newlines(const char *csvStr) {
    const char *ptr, *lineStart;
    CSV_STRUCT_SPLIT_LINE* csvSplitLine = NULL;
    uint8_t flag;
    uint16_t lines, linesCount;

    csvSplitLine = calloc(1, sizeof(CSV_STRUCT_SPLIT_LINE));
    if(csvSplitLine == NULL)
        goto end;

    //get how many row we need
    lines = csv_count_lines(csvStr);
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

static int csv_check(const char* csvStr, uint16_t* row, uint16_t* column){
    int res = 0;
    const char* ptr = csvStr;


//    *row = r;
//    *column = c;
    end:
    return res;
}

CSV_STRUCT* csv_parser(const char* csvStr){
    CSV_STRUCT* csv = NULL;
    const char* ptr = csvStr;
    if(csv != NULL)
        goto end;
    end:
    return csv;
}
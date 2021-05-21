//
// Created by yushigengyu on 2021/5/19.
//

#include "unit_test.h"
static const char *file_basename(const char *file){
    char *end_ptr = NULL;
    char *rst = NULL;

    if (!((end_ptr = strrchr(file, '\\')) != NULL || \
        (end_ptr = strrchr(file, '/')) != NULL) || \
        (strlen(file) < 2)){
        rst = (char *)file;
    }
    else
        rst = (char *)(end_ptr + 1);
    return (const char *)rst;
}

void unit_test_assert(int value, const char *msg, const char *fileName, int lineNum, const char *funcName){
    if(!value) {
        UINT_TEST_PRINTF("[FAIL] at (%s); func: (%s:%d); msg: (%s)\n", file_basename(fileName), funcName, lineNum, msg);
        abort();
    }
    else
        UINT_TEST_PRINTF("[PASS] (%s:%d) is passed\n", funcName, lineNum);
}

void unit_test_assert_array(const uint8_t *x, const uint8_t *y, size_t len, uint8_t equal, const char *msg, const char *fileName,
        int lineNum, const char *funcName){
    if(x == NULL || y == NULL)
        unit_test_assert(0, msg, fileName, lineNum, funcName);
    unit_test_assert(memcmp(x, y, len) == 0 == equal, msg, fileName, lineNum, funcName);
}
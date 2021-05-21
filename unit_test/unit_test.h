//
// Created by yushigengyu on 2021/5/19.
//

#ifndef C_CLI_TABLE_UNIT_TEST_H
#define C_CLI_TABLE_UNIT_TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define UINT_TEST_PRINTF printf
void unit_test_assert(int value, const char *msg, const char *fileName, int lineNum, const char *funcName);
void unit_test_assert_array(const uint8_t *x, const uint8_t *y, size_t len, uint8_t equal, const char *msg, const char *fileName,
                            int lineNum, const char *funcName);

#define unit_test_assert_true(value)      unit_test_assert(value, "(" #value ") is false")
#define unit_test_assert_false(value)     unit_test_assert(!(value), "(" #value ") is true")
#define unit_test_assert_null(value)      unit_test_assert((const char *)(value) == NULL, "(" #value ") is not null", __FILE__, __LINE__, __func__)
#define unit_test_assert_not_null(value)  unit_test_assert((const char *)(value) != NULL, "(" #value ") is null", __FILE__, __LINE__, __func__)
#define unit_test_assert_int_equal(a, b)           unit_test_assert((a) == (b), "(" #a ") not equal to (" #b ")", __FILE__, __LINE__, __func__)
#define unit_test_assert_int_not_equal(a, b)       unit_test_assert((a) != (b), "(" #a ") equal to (" #b ")", __FILE__, __LINE__, __func__)
#define unit_test_assert_pointer_equal(a, b)           unit_test_assert((a) == (b), "(" #a ") not equal to (" #b ")", __FILE__, __LINE__, __func__)
#define unit_test_assert_pointer_not_equal(a, b)       unit_test_assert((a) != (b), "(" #a ") equal to (" #b ")", __FILE__, __LINE__, __func__)
#define unit_test_assert_str_equal(a, b, len)      unit_test_assert_array((const uint8_t*)(a), (const uint8_t*)(b), len, 1, "string not equal", __FILE__, __LINE__, __func__)
#define unit_test_assert_str_not_equal(a, b, len)  unit_test_assert_array((const uint8_t*)(a), (const uint8_t*)(b), len, 0,  "string equal", __FILE__, __LINE__, __func__)
#endif //C_CLI_TABLE_UNIT_TEST_H

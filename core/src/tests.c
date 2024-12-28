/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#include <tests.h>

#ifdef ENABLE_TESTS

#include <base/types.h>
#include <string.h>

#define TEST(exp) { error.code |= ((exp)?ERROR_NONE:ERROR_TEST); strcpy(error.msg, #exp);}

#define TEST_FUN(x) if(error.code == ERROR_NONE){error = x;}

static inline Error _datatype_size() {
    Error error = { ERROR_NONE, "", true};

    TEST(sizeof(i8)==1);
    TEST(sizeof(i16)==2);
    TEST(sizeof(i32)==4);
    TEST(sizeof(i64)==8);
    TEST(sizeof(i128)==16);

    TEST(sizeof(u8)==1);
    TEST(sizeof(u16)==2);
    TEST(sizeof(u32)==4);
    TEST(sizeof(u64)==8);
    TEST(sizeof(u128)==16);

    TEST(sizeof(f32)==4);
    TEST(sizeof(f64)==8);
    TEST(sizeof(f96)==16);

    return error;
}

static inline Error _argument() {
    Error error = { ERROR_NONE, "", true};

    return error;
}

Error init_tests() {
   Error error = {ERROR_NONE, "", false };

   TEST_FUN(_datatype_size());
   TEST_FUN(_argument());

   return error;
}

#endif
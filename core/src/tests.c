/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#include <tests.h>

#ifdef ENABLE_TESTS

#include <base/types.h>

// TODO: Remove
#include <string.h>

#define TEST(exp) if(!(exp)){error.code|=ERROR_TEST;strcpy(error.msg, #exp);}


Error _init_tests() {
    Error error = {.code = ERROR_NONE};

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

#endif
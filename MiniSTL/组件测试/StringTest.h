/*
** StringTest.cpp
** Created by Rayn on 2015/03/03
*/
#ifndef _STRING_TEST_H_
#define _STRING_TEST_H_

#include "../String.h"
#include <cstdio>
#include <iostream>

namespace rayn {
    namespace test {
        void test_string_1() {
            rayn::string str("Hello World!");
            printf("%s\n", str.c_str());
        }
        void test_string() {
            test_string_1();
        }
    }
}

#endif
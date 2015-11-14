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
            std::cout << str.rfind("ld") << std::endl;
            auto beg = str.begin();
            auto end = beg;
            end++;
            end++;
            str.erase(beg, end);
            std::cout << str << std::endl;
        }
        void test_string() {
            test_string_1();
        }
    }
}

#endif
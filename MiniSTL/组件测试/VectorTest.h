#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include "../Vector.h"
#include "../String.h"
#include <iostream>
#include <string>

namespace rayn {
    namespace test {
        void test_vector_1() {
            vector<std::string> vec(5, "abc");
            for (auto cur : vec) {
                std::cout << cur << std::endl;
            }
        }
        void test_vector() {
            test_vector_1();
        }
    }
}

#endif
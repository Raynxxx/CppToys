#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include <string>
#include <iostream>

#include "../Vector.h"

namespace rayn {
    namespace test {
        void test_vector_1() {
            vector<std::string> vec(5, "abc");
            for (auto cur : vec) {
                std::cout << cur << std::endl;
            }
        }
        void test_vector_2() {

        }
    }
}

#endif
#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include <iostream>

#include "../Vector.h"

namespace rayn {
    namespace test {
        void test_vector_1() {
            vector<double> vec(5, 3.14);
            for (auto cur : vec) {
                std::cout << cur << std::endl;
            }
        }
    }
}

#endif
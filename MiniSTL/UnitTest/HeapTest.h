/*
** HeapTest.cpp
** Created by Rayn on 2015/10/11
*/
#ifndef _HEAP_TEST_H_
#define _HEAP_TEST_H_

#include <cstdio>
#include <iostream>

#include "../Vector.h"
#include "../Heap.h"

namespace rayn {
    namespace test {
        template <class Compare>
        void test_heap_1(Compare comp) {
            int arr[] = { 1, 2, 3, 4, 5 };
            rayn::vector<int> vec(arr, arr + 5);

            rayn::make_heap(vec.begin(), vec.end(), comp);
            for (int i = 0; i < vec.size(); ++i) {
                std::cout << vec[i] << " ";
            }
            std::cout << std::endl;

            vec.push_back(6);
            rayn::push_heap(vec.begin(), vec.end(), comp);
            for (int i = 0; i < vec.size(); ++i) {
                std::cout << vec[i] << " ";
            }
            std::cout << std::endl;


            rayn::pop_heap(vec.begin(), vec.end(), comp);
            vec.pop_back();
            for (int i = 0; i < vec.size(); ++i) {
                std::cout << vec[i] << " ";
            }
            std::cout << std::endl;

            rayn::sort_heap(vec.begin(), vec.end(), comp);
            for (int i = 0; i < vec.size(); ++i) {
                std::cout << vec[i] << " ";
            }
            std::cout << std::endl;
        }
    
        void test_heap() {
            test_heap_1(std::less<int>());
            test_heap_1(std::greater<int>());
        }
        
    }
}

#endif

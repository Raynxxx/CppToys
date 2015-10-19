/*
** TestSTL.cpp
** Created by Rayn on 2015/02/25
*/
#include "VectorTest.h"
#include "StringTest.h"
#include "HeapTest.h"

int main() {
    rayn::test::test_vector();
    rayn::test::test_string();
    rayn::test::test_heap();
    return 0;
}
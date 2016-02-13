/*
** unit test for Array
** Created by Rayn on 2016/02/13
*/
#include "catch.hpp"
#include "../Array.h"


TEST_CASE("array template", "[array]") {
    rayn::array<int, 5> arr1 = { 2, 5, 4, 8, 16 };
    REQUIRE(arr1.size() == 5);
    rayn::array<int, 0> arr2;
    REQUIRE(arr2.size() == 0);
    REQUIRE(arr2.begin() == nullptr);
}

TEST_CASE("array iterators", "[array]") {
    rayn::array<int, 5> arr1 = { 2, 5, 4, 8, 16 };
    REQUIRE(*arr1.begin() == 2);
    REQUIRE(*(arr1.end() - 1) == 16);
    REQUIRE(*arr1.rbegin() == 16);
}

TEST_CASE("array element access", "[array]") {
    rayn::array<int, 5> arr1 = { 2, 5, 4, 8, 16 };
    REQUIRE(arr1[0] == 2);
    REQUIRE(arr1[1] == 5);
    REQUIRE(arr1.at(4) == 16);
    REQUIRE(arr1.back() == 16);
}

TEST_CASE("array modifiers", "[array]") {
    rayn::array<int, 5> arr1 = { 2, 5, 4, 8, 16 };
    REQUIRE(arr1.front() == 2);
    
    arr1.fill(5);
    REQUIRE(arr1.front() == 5);
    REQUIRE(arr1.back() == 5);

    rayn::array<int, 5> arr2 = { 2, 6, 4, 8, 16 };
    arr1.swap(arr2);
    REQUIRE(arr1.front() == 2);
    REQUIRE(arr2.front() == 5);
}

TEST_CASE("array get, comparisons", "[array]") {
    rayn::array<int, 5> arr1 = { 2, 5, 4, 8, 16 };
    REQUIRE(rayn::get<0>(arr1) == 2);

    rayn::array<int, 5> arr2 = { 20, 15, 24, 78, 96 };

    REQUIRE(arr1 < arr2);

    arr1.fill(5);
    arr2.fill(5);
    REQUIRE(arr1 == arr2);
}
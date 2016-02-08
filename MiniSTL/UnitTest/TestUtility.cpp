/*
** unit test for Utility
** Created by Rayn on 2016/02/04
*/
#include "catch.hpp"
#include "../Utility.h"
#include "../String.h"

TEST_CASE("pair", "[pair]") {
    rayn::pair<rayn::string, int> p1("hello", 5);
    REQUIRE(p1.first == "hello");
    REQUIRE(p1.second == 5);

    rayn::pair<rayn::string, int> p2(rayn::move(p1));

    REQUIRE(p1.first.empty());
    REQUIRE(p1.second == 5);
    REQUIRE(p2.first == "hello");
}

TEST_CASE("swap", "[utility]") {
    int x = 4, y = 24;
    rayn::swap(x, y);
    REQUIRE(x == 24);
    REQUIRE(y == 4);

    int a[4] = { 1, 2, 3, 4 };
    int b[4] = { 5, 6, 7, 8 };
    rayn::swap(a, b);
    REQUIRE(a[0] == 5);
    REQUIRE(b[0] == 1);
}

TEST_CASE("move", "[move]") {
    rayn::string bar = "bar-string";
    REQUIRE(rayn::is_rvalue_reference<decltype(rayn::move(bar))>::value);
}
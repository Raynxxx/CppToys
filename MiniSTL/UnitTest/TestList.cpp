/*
** unit test for string
** Created by Rayn on 2016/02/14
*/
#include "catch.hpp"
#include "../List.h"

TEST_CASE("list constructor", "[list]") {
    rayn::list<int> first;
    rayn::list<int> second(4u, 100);

    SECTION("default constructor") {
        REQUIRE(first.empty());
    }

    SECTION("fill constructor") {
        REQUIRE(second.size() == 4);
        REQUIRE(second.front() == 100);
    }

}
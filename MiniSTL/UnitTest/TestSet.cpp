/*
** unit test for set
** Created by Rayn on 2016/02/19
*/
#include "catch.hpp"
#include "../Src/Set.h"
#include "../Src/MultiSet.h"

// *************************************
// set

TEST_CASE("set constructor", "[set]") {
    rayn::set<int> first;
    REQUIRE(first.empty());

    int myints[] = { 10, 20, 30, 40, 50 };
    rayn::set<int> second(myints, myints + 5);
    REQUIRE(second.size() == 5);

    rayn::set<int> third(second);
    REQUIRE(*second.begin() == *third.begin());

    rayn::set<int> fourth(second.begin(), second.end());
    REQUIRE(*fourth.rbegin() == *second.rbegin());

    rayn::set<int> fifth = fourth;
    REQUIRE(fifth.size() == fourth.size());
}

TEST_CASE("set insert") {

}

// *************************************
// multiset
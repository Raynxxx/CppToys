/*
** unit test for vector
** Created by Rayn on 2015/12/23
*/
#include "catch.hpp"
#include "../Vector.h"

TEST_CASE("vector construct", "[vector]") {
    rayn::vector<double> v1(5, 2.33);

    REQUIRE(v1.size() == 5);

    SECTION("vector can be construct from another vector") {
        rayn::vector<double> v2(v1);
        REQUIRE(v1 == v2);
    }
    SECTION("vector can be construct from iterator") {
        rayn::vector<double> v2(v1.begin(), v1.end() - 1);
        REQUIRE(v2.size() == v1.size() - 1);
        REQUIRE(v1 != v2);
    }
}

SCENARIO("vector can be sized and resized", "[vector]") {

    GIVEN("A vector with some items") {
        rayn::vector<int> v(5, 233);

        REQUIRE(v[0] == 233);
        REQUIRE(v.size() == 5); 
        REQUIRE(v.capacity() >= 5);

        WHEN("the size is increased") {
            v.resize(10);

            THEN("the size and capacity change") {
                REQUIRE(v.size() == 10);
                REQUIRE(v.capacity() >= 10);
            }
        }
        WHEN("the size is reduced") {
            v.resize(0);

            THEN("the size changes but not capacity") {
                REQUIRE(v.size() == 0);
                REQUIRE(v.capacity() >= 5);
            }
        }
        WHEN("more capacity is reserved") {
            v.reserve(10);

            THEN("the capacity changes but not the size") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 10);
            }
        }
        WHEN("less capacity is reserved") {
            v.reserve(0);

            THEN("neither size nor capacity are changed") {
                REQUIRE(v.size() == 5);
                REQUIRE(v.capacity() >= 5);
            }
        }
    }
}

TEST_CASE("element access function of vector", "[vector]") {
    rayn::vector<int> v1;

    REQUIRE(v1.empty());

    for (int i = 0; i < 10; ++i) {
        v1.push_back(i);
    }
    REQUIRE(v1.size() == 10);
    REQUIRE(v1.front() == 0);

    SECTION("vector insert") {
        v1.insert(v1.end(), 11);
        v1.insert(v1.end(), 12);

        REQUIRE(v1.size() == 12);
        REQUIRE(v1.back() == 12);
    }

    SECTION("vector erase") {
        v1.erase(v1.begin());
        v1.pop_back();

        REQUIRE(v1.size() == 8);
        REQUIRE(v1.front() == 1);
        REQUIRE(v1.back() == 8);
    }
}
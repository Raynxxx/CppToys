#include "catch.hpp"
#include "../String.h"

TEST_CASE("string constructor", "[string]") {
    rayn::string s1("Hello World!");

    REQUIRE(s1 == "Hello World!");
    REQUIRE(s1.length() == 12);

    SECTION("string can be construct from another string") {
        rayn::string s2(s1);

        REQUIRE(s1 == s2);
    }

    SECTION("string can be construct from substring") {
        rayn::string s2(s1, 2, 6);

        REQUIRE(s2 == "llo Wo");
        REQUIRE(s2.length() == 6);
    }
}
/*
** unit test for string
** Created by Rayn on 2015/12/23
*/
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

TEST_CASE("string operator", "[string][operator]") {
    rayn::string str1, str2, str3;
    str1 = "operator= char* ";
    str2 = 'c';

    SECTION("operator =") {
        str3 = str1;
        REQUIRE(str1 == "operator= char* ");
        REQUIRE(str2.length() == 1);
    }

    SECTION("operator +") {
        str3 = str1 + str2;
        REQUIRE(str3 == "operator= char* c");
        str3 = str1 + "str";
        REQUIRE(str3 == "operator= char* str");
        str3 = str2 + 'h';
        REQUIRE(str3 == "ch");
    }

    SECTION("operator < > <= >= == !=") {
        rayn::string foo = "alpha";
        rayn::string bar = "beta";

        REQUIRE(foo < bar);
        REQUIRE(bar > foo);
        REQUIRE(foo != bar);
        REQUIRE(foo <= bar);
        REQUIRE(bar >= foo);
    }

    SECTION("operator[]") {
        rayn::string str("Test string");
        REQUIRE(str.length() == 11);
        REQUIRE(str[4] == ' ');
    }
    
}

TEST_CASE("string iterators", "[string][iterator]") {
    rayn::string str("Test string");
    REQUIRE(*str.begin() == 'T');
    REQUIRE(*(str.end() - 1) == 'g');
    REQUIRE(*str.rbegin() == 'g');
    REQUIRE(*(str.rend() - 1) == 'T');
    REQUIRE(str.front() == 'T');
    REQUIRE(str.back() == 'g');
}

TEST_CASE("string capacity function", "[string]") {
    rayn::string str("Test string");

    REQUIRE(str.size() == 11);
    REQUIRE(str.length() == 11);
    REQUIRE(str.capacity() >= 11);

    SECTION("string can be resize smaller") {
        str.resize(10);
        REQUIRE(str.length() == 10);
        REQUIRE(str == "Test strin");
    }

    SECTION("string can be resize bigger") {
        str.resize(16);
        REQUIRE(str.length() == 16);
        REQUIRE(str.back() == '\0');
        REQUIRE(str.substr(0, 11) == "Test string");
    }

    SECTION("string reserve and shrink to fit") {
        str.reserve(32);
        REQUIRE(str.length() == 11);
        REQUIRE(str.capacity() == 32);

        str.shrink_to_fit();
        REQUIRE(str.length() == 11);
        REQUIRE(str.capacity() == 11);
    }

    SECTION("string clear") {
        str.clear();
        REQUIRE(str.length() == 0);
        REQUIRE(str.empty());
    }
    
}
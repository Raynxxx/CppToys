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

TEST_CASE("string element access", "[string]") {
    rayn::string str("test string");

    REQUIRE(str[0] == 't');
    REQUIRE(str.at(4) == ' ');
    REQUIRE(str.front() == 't');
    REQUIRE(str.back() == 'g');
}

TEST_CASE("string operator+=", "[string]") {
    rayn::string str1("test");
    rayn::string str2("test string");

    str1 += str2;
    REQUIRE(str1.length() == 15);
    REQUIRE(str1 == "testtest string");

    str2 += "!!";
    REQUIRE(str2.length() == 13);
    str2 += '?';
    REQUIRE(str2.length() == 14);
    REQUIRE(str2 == "test string!!?");
}

TEST_CASE("string append", "[string]") {
    rayn::string str1;
    rayn::string str2 = "Writing ";
    rayn::string str3 = "print 10 and then 5 more";

    SECTION("append the whole string") {
        str1.append(str2);
        REQUIRE(str1 == "Writing ");
    }
    
    SECTION("append substring") {
        str1.append(str3, 6, 2);
        REQUIRE(str1 == "10");

        str2.append(str3, 20);
        REQUIRE(str2 == "Writing more");
    }

    SECTION("append cstring") {
        str1.append("cool cool", 4);
        REQUIRE(str1 == "cool");

        str2.append("here");
        REQUIRE(str2 == "Writing here");
    }

    SECTION("append with fill & range") {
        str1.append(5, 'c');
        REQUIRE(str1 == "ccccc");

        str2.append(str3.begin() + 9, str3.end());
        REQUIRE(str2 == "Writing and then 5 more");
    }
}

TEST_CASE("string push_back & pop_back", "[string]") {
    rayn::string str = "Writing ";
    str.push_back('6');
    REQUIRE(str == "Writing 6");
    str.pop_back();
    str.pop_back();
    REQUIRE(str == "Writing");
}

TEST_CASE("string assign", "[string]") {
    std::string str;
    std::string base = "The quick brown fox jumps over a lazy dog.";

    SECTION("assign the whole string") {
        str.assign(base);
        REQUIRE(str == base);
    }

    SECTION("assign substring") {
        str.assign(base, 10, 9);
        REQUIRE(str == "brown fox");
    }

    SECTION("assign cstring") {
        str.assign("alphabeta");
        REQUIRE(str == "alphabeta");
        str.assign("alphabeta", 5);
        REQUIRE(str == "alpha");
    }

    SECTION("assign with fill & range") {
        str.assign(10, '*');
        REQUIRE(str == "**********");

        str.assign(base.begin() + 16, base.end() - 12);
        REQUIRE(str == "fox jumps over");
    }
}
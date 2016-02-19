/*
** unit test for string
** Created by Rayn on 2016/02/14
*/
#include "catch.hpp"
#include "../Src/List.h"

TEST_CASE("list constructor", "[list]") {
    rayn::list<int> first;
    rayn::list<int> second(4, 100);
    rayn::list<int> third(second.begin(), second.end());
    rayn::list<int> fourth(third);

    SECTION("default constructor") {
        REQUIRE(first.empty());
    }

    SECTION("fill constructor") {
        REQUIRE(second.size() == 4);
        REQUIRE(second.front() == 100);
    }

    SECTION("range constructor") {
        REQUIRE(third.size() == second.size());
        REQUIRE(third.front() == second.front());
    }

    SECTION("copy constructor") {
        REQUIRE(fourth.size() == third.size());
        REQUIRE(fourth.front() == third.front());
    }
}

TEST_CASE("list operator=", "[list]") {
    rayn::list<int> first;
    rayn::list<int> second(4, 56);
    first = second;
    REQUIRE(second.size() == 4);
    REQUIRE(first.size() == second.size());
    REQUIRE(first.front() == second.front());
}

TEST_CASE("list iterator", "[list][iterator]") {
    rayn::list<int> lst;
    lst.push_back(5);
    lst.push_back(56);
    lst.push_back(78);

    REQUIRE(*lst.begin() == 5);
    REQUIRE(*(--lst.end()) == 78);
    REQUIRE(*lst.rbegin() == 78);
    REQUIRE(*(++lst.rbegin()) == 56);
}

TEST_CASE("list capacity", "[list]") {
    rayn::list<int> lst;

    REQUIRE(lst.empty());

    for (int i = 1; i <= 56; ++i) {
        lst.push_back(i);
    }

    REQUIRE(lst.size() == 56);
}

TEST_CASE("list element access", "[list]") {
    rayn::list<int> lst;

    for (int i = 1; i <= 56; ++i) {
        lst.push_back(i);
    }

    REQUIRE(lst.front() == 1);
    REQUIRE(lst.back() == 56);
}

TEST_CASE("list assign", "[list]") {
    rayn::list<int> first;
    rayn::list<int> second;

    first.assign(7, 100);
    second.assign(first.begin(), first.end());

    REQUIRE(first.size() == 7);
    REQUIRE(first.front() == 100);
    REQUIRE(second.size() == 7);
    
    int arr[] = { 177, 56, 78 };
    first.assign(arr, arr + 3);

    REQUIRE(first.size() == 3);
    REQUIRE(first.front() == 177);
    REQUIRE(second.size() == 7);
}

TEST_CASE("list insert", "[list]") {
    rayn::list<int> lst;

    for (int i = 1; i <= 10; ++i) {
        lst.push_back(i);
    }

    REQUIRE(lst.size() == 10);
    REQUIRE(lst.front() == 1);

    auto it = lst.begin();

    lst.insert(it, 0);

    REQUIRE(lst.size() == 11);
    REQUIRE(lst.front() == 0);

    lst.insert(it, 2, 11);

    REQUIRE(lst.size() == 13);
    REQUIRE(*(++lst.begin()) == 11);

    int arr[] = { 177, 56, 78 };
    lst.insert(lst.begin(), arr, arr + 3);

    REQUIRE(lst.size() == 16);
    REQUIRE(*(++lst.begin()) == 56);
}

TEST_CASE("list erase", "[list]") {
    rayn::list<int> lst;

    for (int i = 1; i <= 10; ++i) {
        lst.push_back(i);
    }

    REQUIRE(lst.size() == 10);
    REQUIRE(lst.front() == 1);

    auto it = lst.begin();
    rayn::advance(it, 5);

    it = lst.erase(it);

    REQUIRE(lst.size() == 9);
    REQUIRE(*it == 7);

    auto it2 = lst.end();
    it2 = lst.erase(it, it2);

    REQUIRE(lst.size() == 5);
    REQUIRE(it2 == lst.end());
}

TEST_CASE("list modifiers", "[list]") {
    rayn::list<int> first;
    first.push_back(56);
    first.push_front(78);
    first.push_front(9);

    REQUIRE(first.size() == 3);
    REQUIRE(first.front() == 9);

    first.pop_back();
    first.push_back(99);
    first.pop_front();
    REQUIRE(first.size() == 2);
    REQUIRE(first.front() == 78);
}
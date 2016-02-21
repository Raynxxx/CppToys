/*
** unit test for red-black tree
** Created by Rayn on 2016/02/19
*/
#include "catch.hpp"
#include "../Src/Tree.h"
#include "../Src/Functional.h"

TEST_CASE("rb-tree operator= & assign", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int>> tree;
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int >> tree2;

    int arr[] = { 1, 2, 3, 4, 5 };
    int arr2[] = { 6, 7, 8, 9, 10, 10, 9 };
    tree.insert_unique(arr, arr + 5);

    REQUIRE(tree.size() == 5);
    REQUIRE(*tree.begin() == 1);
    REQUIRE(tree2.empty());
    
    SECTION("operator=") {
        tree2 = tree;
        REQUIRE(tree2.size() == 5);
    }

    SECTION("assign unique") {
        tree2.assign_unique(arr2, arr2 + 7);
        REQUIRE(tree2.size() == 5);
    }

    SECTION("assign equal") {
        tree2.assign_equal(arr2, arr2 + 7);
        REQUIRE(tree2.size() == 7);
    }
}

TEST_CASE("rb-tree insert", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int>> tree;

    REQUIRE(tree.size() == 0);
    
    tree.insert_unique(10);
    tree.insert_unique(7);
    tree.insert_unique(8);
    tree.insert_unique(15);
    tree.insert_unique(5);
    tree.insert_unique(6);

    REQUIRE(tree.size() == 6);
    REQUIRE(*tree.begin() == 5);
    REQUIRE(*(--tree.end()) == 15);

    SECTION("insert unique") {
        auto ret = tree.insert_unique(6);
        REQUIRE(ret.second == false);
        REQUIRE(tree.size() == 6);
    }

    SECTION("insert equal") {
        tree.insert_equal(5);
        tree.insert_equal(6);

        REQUIRE(tree.size() == 8);
        REQUIRE(*tree.begin() == 5);
        REQUIRE(*(--tree.end()) == 15);
    }

    SECTION("insert unique range") {
        int arr[] = { 1, 2, 3, 4, 5 };
        tree.insert_unique(arr, arr + 5);
        REQUIRE(tree.size() == 10);
    }

    SECTION("insert equal range") {
        int arr[] = { 1, 2, 3, 4, 5 };
        tree.insert_equal(arr, arr + 5);
        REQUIRE(tree.size() == 11);
    }
    
}

TEST_CASE("rb-tree erase", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>, rayn::less<int>> tree;

    REQUIRE(tree.size() == 0);

    tree.insert_equal(10);
    tree.insert_equal(7);
    tree.insert_equal(8);
    tree.insert_equal(15);
    tree.insert_equal(5);
    tree.insert_equal(6);
    tree.insert_equal(6);
    tree.insert_equal(6);

    REQUIRE(tree.size() == 8);

    tree.erase(5);
    tree.erase(6);

    REQUIRE(tree.size() == 4);
    REQUIRE(*tree.begin() == 7);
    REQUIRE(*(--tree.end()) == 15);

    for (int i = 1; i <= 5; ++i) {
        tree.insert_equal(i);
    }

    int arr[] = { 1, 2, 3, 4, 5 };
    tree.erase(arr, arr + 5);

    REQUIRE(tree.size() == 4);
}

TEST_CASE("rb-tree clear", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int>> tree;

    tree.insert_equal(10);
    tree.insert_equal(7);
    tree.insert_equal(8);
    tree.insert_equal(15);

    REQUIRE(tree.size() == 4);

    tree.clear();

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.empty());
}

TEST_CASE("rb-tree find operations.", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>, 
        rayn::less<int>> tree;

    REQUIRE(tree.size() == 0);

    tree.insert_equal(10);
    tree.insert_equal(7);
    tree.insert_equal(8);
    tree.insert_equal(15);
    tree.insert_equal(5);
    tree.insert_equal(6);
    tree.insert_equal(5);

    SECTION("find") {
        auto it = tree.find(15);
        REQUIRE(*it == 15);
        it = tree.find(78);
        REQUIRE(it == tree.end());
    }

    SECTION("count") {
        auto n = tree.count(5);
        REQUIRE(n == 2);
    }

    SECTION("lower_bound") {
        auto it = tree.lower_bound(7);
        REQUIRE(*it == 7);
        it = tree.lower_bound(20);
        REQUIRE(it == tree.end());
    }
    
    SECTION("upper_bound") {
        auto it = tree.upper_bound(7);
        REQUIRE(*it == 8);
        it = tree.upper_bound(15);
        REQUIRE(it == tree.end());
    }
}

TEST_CASE("rb-tree Relational operators", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int >> tree;
    rayn::rb_tree<int, int, rayn::identity<int>,
        rayn::less<int >> tree2;

    int arr[] = { 1, 2, 3, 4, 5 };
    int arr2[] = { 6, 7, 8, 9, 10 };
    tree.insert_unique(arr, arr + 5);

    SECTION("operator==") {
        tree2.insert_unique(arr, arr + 5);
        REQUIRE(tree2 == tree);
    }

    SECTION("operator<") {
        tree2.insert_unique(arr2, arr2 + 5);
        REQUIRE(tree < tree2);
    }

}
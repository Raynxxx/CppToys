/*
** unit test for red-black tree
** Created by Rayn on 2016/02/19
*/
#include "catch.hpp"
#include "../Src/Tree.h"
#include "../Src/Functional.h"


TEST_CASE("rb-tree insert_unique", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>, rayn::less<int>> tree;

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

    auto ret = tree.insert_unique(6);
    REQUIRE(ret.second == false);
    REQUIRE(tree.size() == 6);
}

TEST_CASE("rb-tree insert_equal", "[rb-tree]") {
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
    REQUIRE(*tree.begin() == 5);
    REQUIRE(*(--tree.end()) == 15);
}

TEST_CASE("rb-tree find", "[rb-tree]") {
    rayn::rb_tree<int, int, rayn::identity<int>, rayn::less<int>> tree;

    REQUIRE(tree.size() == 0);

    tree.insert_equal(10);
    tree.insert_equal(7);
    tree.insert_equal(8);
    tree.insert_equal(15);
    tree.insert_equal(5);
    tree.insert_equal(6);

    auto it = tree.find(15);
    REQUIRE(*it == 15);

    it = tree.find(78);
    REQUIRE(it == tree.end());
}
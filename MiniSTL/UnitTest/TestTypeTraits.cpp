/*
** unit test for type_traits
** Created by Rayn on 2016/01/29
*/
#include "catch.hpp"
#include "../TypeTraits.h"
#include "../String.h"

TEST_CASE("integral_constant", "[type_traits]") {
    typedef rayn::integral_constant<int, 1> one_t;
    REQUIRE(one_t::value == 1);
    REQUIRE(one_t::type::value == 1);

    int i = one_t();
    REQUIRE(i == 1);
}

TEST_CASE("is_array", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_array<int>::value);
    REQUIRE(rayn::is_array<int[3]>::value);
    REQUIRE(rayn::is_array<rayn::string[3]>::value);
}

TEST_CASE("is_class, is_enum, is_union", "[type_traits]") {
    union A { int i; float j; };
    enum class B { x, y, z };
    enum C { x, y, z };

    REQUIRE_FALSE(rayn::is_class<int>::value);
    REQUIRE(rayn::is_class<rayn::string>::value);
    REQUIRE_FALSE(rayn::is_class<A>::value);

    REQUIRE(rayn::is_enum<B>::value);
    REQUIRE(rayn::is_enum<C>::value);
    REQUIRE(rayn::is_enum<decltype(C::x)>::value);

    REQUIRE_FALSE(rayn::is_union<int>::value);
    REQUIRE(rayn::is_union<A>::value);
}

TEST_CASE("is_floating_point", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_floating_point<int>::value);
    REQUIRE(rayn::is_floating_point<float>::value);
    REQUIRE(rayn::is_floating_point<const double>::value);
}

TEST_CASE("is_void", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_void<int>::value);
    REQUIRE(rayn::is_void<void>::value);
}
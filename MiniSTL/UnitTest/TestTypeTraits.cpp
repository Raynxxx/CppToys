/*
** unit test for type_traits
** Created by Rayn on 2016/01/29
*/
#include "catch.hpp"
#include "../TypeTraits.h"

TEST_CASE("integral_constant", "[type_traits]") {
    typedef rayn::integral_constant<int, 1> one_t;
    REQUIRE(one_t::value == 1);
    REQUIRE(one_t::type::value == 1);

    int i = one_t();
    REQUIRE(i == 1);
}
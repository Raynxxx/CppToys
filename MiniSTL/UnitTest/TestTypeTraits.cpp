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

TEST_CASE("is_void", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_void<int>::value);
    REQUIRE(rayn::is_void<void>::value);
}

TEST_CASE("is_floating_point", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_floating_point<int>::value);
    REQUIRE(rayn::is_floating_point<float>::value);
    REQUIRE(rayn::is_floating_point<const double>::value);
}

TEST_CASE("is_integral", "[type_traits]") {
    REQUIRE(rayn::is_integral<char>::value);
    REQUIRE(rayn::is_integral<int>::value);
    REQUIRE_FALSE(rayn::is_integral<float>::value);
    REQUIRE_FALSE(rayn::is_integral<const double>::value);
}

TEST_CASE("is_pointer", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_pointer<int>::value);
    REQUIRE(rayn::is_pointer<int*>::value);
    REQUIRE(rayn::is_pointer<int**>::value);
    REQUIRE(rayn::is_pointer<int(*)(int)>::value);
}

TEST_CASE("is_lvalue_reference", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_lvalue_reference<int>::value);
    REQUIRE(rayn::is_lvalue_reference<int&>::value);
    REQUIRE_FALSE(rayn::is_lvalue_reference<int&&>::value);
}

TEST_CASE("is_rvalue_reference", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_rvalue_reference<int>::value);
    REQUIRE_FALSE(rayn::is_rvalue_reference<int&>::value);
    REQUIRE(rayn::is_rvalue_reference<int&&>::value);
}

// Declare for test is_function

int fun1(int i) {
    return i + 1;
}

int (*fun2)(int) = fun1;

struct fun3_t {
    int operator() (int i) {
        return i + 1;
    }
} fun3;

TEST_CASE("is_function", "[type_traits]") {
    REQUIRE(rayn::is_function<decltype(fun1)>::value);
    REQUIRE_FALSE(rayn::is_function<decltype(fun2)>::value);
    REQUIRE_FALSE(rayn::is_function<decltype(fun3)>::value);
    REQUIRE_FALSE(rayn::is_function<fun3_t>::value);

    REQUIRE(rayn::is_function<int(int, float)>::value);
    REQUIRE(rayn::is_function<int(int, float, ...)>::value);
}

TEST_CASE("is_member_function_pointer, is_member_object_pointer", "[type_traits]") {
    struct A {
        int bar;
        int foo() { return bar; };
    };

    int(A::*pfoo)() = &A::foo;
    int A::* pbar = &A::bar;

    // is_member_function_pointer
    REQUIRE_FALSE(rayn::is_member_function_pointer<A*>::value);
    REQUIRE(rayn::is_member_function_pointer<int(A::*)()>::value);
    REQUIRE(rayn::is_member_function_pointer<decltype(pfoo)>::value);

    // is_member_object_pointer
    REQUIRE_FALSE(rayn::is_member_object_pointer<A*>::value);
    REQUIRE(rayn::is_member_object_pointer<int A::*>::value);
    REQUIRE(rayn::is_member_object_pointer<decltype(pbar)>::value);

}


/// Composite type categories UnitTest

TEST_CASE("is_arithmetic", "[type_traits]") {
    REQUIRE(rayn::is_arithmetic<char>::value);
    REQUIRE(rayn::is_arithmetic<int>::value);
    REQUIRE(rayn::is_arithmetic<long>::value);
    REQUIRE(rayn::is_arithmetic<long long>::value);
    REQUIRE(rayn::is_arithmetic<float>::value);
    REQUIRE(rayn::is_arithmetic<double>::value);
    REQUIRE_FALSE(rayn::is_arithmetic<int*>::value);
}

TEST_CASE("is_compound", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_compound<char>::value);
    REQUIRE_FALSE(rayn::is_compound<int>::value);
    REQUIRE(rayn::is_compound<long*>::value);
    REQUIRE(rayn::is_compound<long long*>::value);
    REQUIRE_FALSE(rayn::is_compound<decltype(nullptr)>::value);
}

TEST_CASE("is_fundamental", "[type_traits]") {
    REQUIRE(rayn::is_fundamental<char>::value);
    REQUIRE(rayn::is_fundamental<int>::value);
    REQUIRE(rayn::is_fundamental<long>::value);
    REQUIRE(rayn::is_fundamental<long long>::value);
    REQUIRE(rayn::is_fundamental<decltype(nullptr)>::value);
    REQUIRE_FALSE(rayn::is_fundamental<int*>::value);
}

TEST_CASE("is_reference", "[type_traits]") {
    REQUIRE_FALSE(rayn::is_reference<int>::value);
    REQUIRE(rayn::is_reference<int&>::value);
    REQUIRE(rayn::is_reference<int&&>::value);
}

TEST_CASE("is_object", "[type_traits]") {
    class A {};
    REQUIRE(rayn::is_object<int>::value);
    REQUIRE(rayn::is_object<A>::value);
    REQUIRE(rayn::is_object<A*>::value);
    REQUIRE(rayn::is_object<int(*)(int)>::value);
    REQUIRE_FALSE(rayn::is_object<A&>::value);
    REQUIRE_FALSE(rayn::is_object<int(int)>::value);
}

TEST_CASE("is_member_pointer", "[type_traits]") {
    struct A {
        int x;
    };
    int A::* pt = &A::x;
    REQUIRE_FALSE(rayn::is_member_pointer<A*>::value);
    REQUIRE(rayn::is_member_pointer<int A::*>::value);
    REQUIRE(rayn::is_member_pointer<void(A::*)()>::value);
    REQUIRE(rayn::is_member_pointer<decltype(pt)>::value);
}

TEST_CASE("is_scalar", "[type_traits]") {
    class A {
        int x;
    };
    enum B { x, y, z };
    REQUIRE_FALSE(rayn::is_scalar<A>::value);
    REQUIRE_FALSE(rayn::is_scalar<A&>::value);
    REQUIRE(rayn::is_scalar<int>::value);
    REQUIRE(rayn::is_scalar<A*>::value);
    REQUIRE(rayn::is_scalar<B>::value);
}

/// Type properties UnitTest

TEST_CASE("is_const", "[type_traits]") {
    REQUIRE(rayn::is_const<const int>::value);
    REQUIRE_FALSE(rayn::is_const<const int*>::value);
    REQUIRE(rayn::is_const<int* const>::value);
}

TEST_CASE("is_volatile", "[type_traits]") {
    REQUIRE(rayn::is_volatile<volatile int>::value);
    REQUIRE_FALSE(rayn::is_volatile<volatile int*>::value);
    REQUIRE(rayn::is_volatile<int* volatile>::value);
}

TEST_CASE("is_signed", "[type_traits]") {
    struct A {
        int x;
    };
    REQUIRE(rayn::is_signed<int>::value);
    REQUIRE(rayn::is_signed<double>::value);
    REQUIRE_FALSE(rayn::is_signed<unsigned long>::value);
    REQUIRE_FALSE(rayn::is_signed<A>::value);
}

TEST_CASE("is_unsigned", "[type_traits]") {
    struct A {};
    REQUIRE(rayn::is_unsigned<unsigned int>::value);
    REQUIRE_FALSE(rayn::is_unsigned<long>::value);
    REQUIRE_FALSE(rayn::is_unsigned<A>::value);
}

/// Type Property queries

TEST_CASE("alignment_of", "[type_traits]") {
    REQUIRE(rayn::alignment_of<char>::value == 1);
    REQUIRE(rayn::alignment_of<int>::value == 4);
    REQUIRE(rayn::alignment_of<int[10]>::value == 4);
    REQUIRE(rayn::alignment_of<long long>::value == 8);
}

TEST_CASE("rank", "[type_traits]") {
    REQUIRE(rayn::rank<int>::value == 0);
    REQUIRE(rayn::rank<int[4]>::value == 1);
    REQUIRE(rayn::rank<int[4][10]>::value == 2);
    REQUIRE(rayn::rank<int[10][10]>::value == 2);
}

TEST_CASE("extent", "[type_traits]") {
    REQUIRE(rayn::extent<int>::value == 0);
    REQUIRE(rayn::extent<int[3][4]>::value == 3);
    auto v2 = rayn::extent<int[3][10], 1>::value;
    REQUIRE(v2 == 10);
}

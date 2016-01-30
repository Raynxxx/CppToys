/*
** TypeTraits.h
** Created by Rayn on 2015/01/30
** 获取传入类型T的类型特性
*/
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

#include <type_traits>

namespace rayn {
    /// Helper classes

    // integral_constant
    template <typename T, T val>
    struct integral_constant {
        static const T                      value = val;
        typedef T                           value_type;
        typedef integral_constant<T, val>   type;

        operator T() { return value; }
    };

    // true type
    typedef integral_constant<bool, true>   true_type;

    // false type
    typedef integral_constant<bool, false>  false_type;


    /// Primary type categories

    template <typename>
    struct remove_cv;

    template <typename>
    struct __is_void_helper : public false_type {};

    template <>
    struct __is_void_helper<void> : public true_type {};

    // is_void
    template <typename T>
    struct is_void
        : public __is_void_helper<typename remove_cv<T>::type>::type {};

    // is_array
    template <typename T>
    struct is_array : public false_type {};

    template <typename T, size_t Size>
    struct is_array<T[Size]> : public true_type {};

    template <typename T>
    struct is_array<T[]> : public true_type {};

    // is_class, depend on compiler
    template <typename T>
    struct is_class : public integral_constant <bool, __is_class(T)> {};

    // is_enum, depend on compiler
    template <typename T>
    struct is_enum : public integral_constant <bool, __is_enum(T)> {};

    // is_union, depend on compiler
    template <typename T>
    struct is_union : public integral_constant <bool, __is_union(T)> {};

    // is_floating_point
    template <typename T>
    struct __is_floating_point_helper
        : public false_type {};

    template <>
    struct __is_floating_point_helper<float>
        : public true_type {};

    template <>
    struct __is_floating_point_helper<double>
        : public true_type {};

    template <>
    struct __is_floating_point_helper<long double>
        : public true_type {};

    template <typename T>
    struct is_floating_point
        : public __is_floating_point_helper<typename remove_cv<T>::type>::type {};

    template <typename T>
    struct remove_const {
        typedef T   type;
    };

    template <typename T>
    struct remove_const<T const> {
        typedef T   type;
    };

    template <typename T>
    struct remove_volatile {
        typedef T   type;
    };

    template <typename T>
    struct remove_volatile<T volatile> {
        typedef T   type;
    };

    /// remove_cv
    template <typename T>
    struct remove_cv {
        typedef typename
        remove_const<typename remove_volatile<T>::type>::type   type;
    };

    /*
    ** primary template
    */

    struct _true_type {};
    struct _false_type {};

    template<class T>
    struct _type_traits {
        typedef _true_type      this_dummy_member_must_be_first;
        typedef _false_type     has_trivial_default_constructor;
        typedef _false_type     has_trivial_copy_constructor;
        typedef _false_type     has_trivial_assignment_operator;
        typedef _false_type     has_trivial_destructor;
        typedef _false_type     is_POD_type;
    };

    /*
    ** 为built_in类型做特化
    */
    template<>
    struct _type_traits < bool > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < signed char > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < wchar_t > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    template<>
    struct _type_traits < short > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned short > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < int > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned int > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned long long > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < float > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < double > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < long double > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    /*
    ** T ptr & const T ptr特化
    */
    template<class T>
    struct _type_traits < T* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<class T>
    struct _type_traits < const T* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };

    /*
    * C-Style String <XXX char*> 特化
    */
    template<>
    struct _type_traits < char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < unsigned char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < signed char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const unsigned char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
    template<>
    struct _type_traits < const signed char* > {
        typedef _true_type	has_trivial_default_constructor;
        typedef _true_type	has_trivial_copy_constructor;
        typedef _true_type	has_trivial_assignment_operator;
        typedef _true_type	has_trivial_destructor;
        typedef _true_type	is_POD_type;
    };
}

#endif
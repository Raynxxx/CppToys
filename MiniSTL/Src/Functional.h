/*
** Functional.h
** Created by Rayn on 2015/10/10
*/
#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_

namespace rayn {

    // unary_function
    template <class Arg, class Result>
    struct unary_function {
        typedef Arg         argument_type;
        typedef Result      result_type;
    };

    // binary_function
    template <class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1        first_argument_type;
        typedef Arg2        second_argument_type;
        typedef Result      result_type;
    };


    /// Arithmetic operations

    // plus
    template <class T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x + y;
        }
    };

    // minus
    template <class T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x - y;
        }
    };

    // multiplies
    template <class T>
    struct multiplies : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x * y;
        }
    };

    // divides
    template <class T>
    struct divides : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x / y;
        }
    };

    // modulus
    template <class T>
    struct modulus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x % y;
        }
    };

    // negate
    template <class T>
    struct negate : public unary_function<T, T> {
        T operator()(const T& x) const {
            return -x;
        }
    };

    /// Comparison operations

    // equal_to
    template <class T>
    struct equal_to : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x == y;
        }
    };

    // not_equal_to
    template <class T>
    struct not_equal_to : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x != y;
        }
    };

    // greater
    template <class T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x > y;
        }
    };

    // less
    template <class T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x < y;
        }
    };

    // greater_equal
    template <class T>
    struct greater_equal : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x >= y;
        }
    };

    // less_equal
    template <class T>
    struct less_equal : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x <= y;
        }
    };

    /// Logical operations

    // logical_and
    template <class T>
    struct logical_and : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x && y;
        }
    };

    // logical_or
    template <class T>
    struct logical_or : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const {
            return x || y;
        }
    };

    // logical_not
    template <class T>
    struct logical_not : public unary_function<T, bool> {
        bool operator()(const T& x) const {
            return !x;
        }
    };

    /// Bitwise operations

    // bit_and
    template <class T>
    struct bit_and : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x & y;
        }
    };

    // bit_or
    template <class T>
    struct bit_or : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x | y;
        }
    };

    // bit_xor
    template <class T>
    struct bit_xor : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const {
            return x ^ y;
        }
    };

    // bit_not
    template <class T>
    struct bit_not : public unary_function<T, bool> {
        T operator()(const T& x) const {
            return ~x;
        }
    };

    /// Other

    // identity
    template <class T>
    struct identity : public unary_function<T, T> {
        T& operator()(T& x) const {
            return x;
        }
        const T& operator()(const T& x) const {
            return x;
        }
    };

    // select1st
    template <class Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        typename Pair::first_type&
        operator()(Pair& x) const {
            return x.first;
        }
        const typename Pair::first_type&
        operator()(const Pair& x) const {
            return x.first;
        }
    };

    // select2nd
    template <class Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type> {
        typename Pair::second_type&
        operator()(Pair& x) const {
            return x.second;
        }
        const typename Pair::second_type&
        operator()(const Pair& x) const {
            return x.second;
        }
    };
}

#endif
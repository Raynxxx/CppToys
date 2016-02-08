/*
** Pair.h
** Created by Rayn on 2015/06/09
*/
#ifndef _PAIR_H_
#define _PAIR_H_

#include "Move.h"

namespace rayn {

    /// piecewise_construct_t
    struct piecewise_construct_t {};

    /// piecewise_construct
    const piecewise_construct_t piecewise_construct = piecewise_construct_t();


    template < class T1, class T2 >
    class pair {
    protected:
        typedef T1      first_type;
        typedef T2      second_type;

    public:
        first_type      first;
        second_type     second;

    public:
        // default constructor
        pair() : first(), second() {}
        
        // copy constructor with implicit conversion
        template <typename T3, typename T4>
        pair(const pair<T3, T4>& other) :
            first(other.first), second(other.second) {}

        // move constructor with implicit conversion
        template <typename T3, typename T4>
        pair(pair<T3, T4>&& other) :
            first(rayn::forward<T3>(other.first)),
            second(rayn::forward<T4>(other.second)) {}

        // copy constructor
        pair(const pair&) = default;

        // move constructor
        pair(pair&&) = default;

        // initialization constructor
        pair(const T1& a, const T2& b) : first(a), second(b) {}
        template <typename T3, typename T4>
        pair(T1&& a, T2&& b) :
            first(rayn::forward<T1>(a)),
            second(rayn::forward<T>(b)) {}

        // operator= copy
        pair& operator= (const pair& other) {
            first = other.first;
            second = other.second;
            return *this;
        }

        // operator= move
        pair& operator= (pair&& other) {
            first = rayn::forward<T1>(other.first);
            second = rayn::forward<T2>(other.second);
            return *this;
        }

        // operator= copy with implicit conversion
        template <class T3, class T4>
        pair& operator= (const pair<T3, T4>& other) {
            first = other.first;
            second = other.second;
            return *this;
        }

        // operator= move with implicit conversion
        template <class T3, class T4>
        pair& operator= (pair<T3, T4>&& other) {
            first = rayn::forward<T1>(other.first);
            second = rayn::forward<T2>(other.second);
            return *this;
        }

        // swap pair
        void swap(pair& other) {
            rayn::swap(first, other.first);
            rayn::swap(second, other.second);
        }

    };

    // swap overloaded for pair
    template <typename T1, typename T2>
    inline void swap(pair<T1, T2>& p1, pair<T1, T2>& p2) {
        p1.swap(p2);
    }

    // make_pair
    // TODO c++11 move version
    template <class T1, class T2>
    inline pair<T1, T2> make_pair(T1 x, T2 y) {
        return pair<T1, T2>(x, y);
    }

    template <class T1, class T2>
    inline bool operator== (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class T1, class T2>
    inline bool operator!= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs == rhs);
    }
    template <class T1, class T2>
    inline bool operator< (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first < rhs.first ||
            (!(lhs.first < rhs.first) && lhs.second < rhs.second);
    }
    template <class T1, class T2>
    inline bool operator> (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs > lhs;
    }
    template <class T1, class T2>
    inline bool operator<= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs > lhs;
    }
    template <class T1, class T2>
    inline bool operator>= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs < rhs;
    }
}

#endif
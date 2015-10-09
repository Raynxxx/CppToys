/*
** Pair.h
** Created by Rayn on 2015/06/09
*/
#ifndef _PAIR_H_
#define _PAIR_H_

namespace rayn {
    template < class T1, class T2 >
    class pair {
    protected:
        typedef T1  first_type;
        typedef T2  second_type;

        first_type  first;
        second_type second;

    public:
        pair()                          : first(T1()), second(T2()) {}
        pair(const T1& a, const T2& b)  : first(a), second(b) {}
        pair(const pair& other)         : first(other.first), second(other.second) {}
        template <class T3, class T4>
        pair(const pair<T3, T4>& other) : first(other.first), second(other.second) {}

        pair& operator= (const pair&);
        template <class T3, class T4>
        pair& operator= (const pair<T3, T4>&);

        inline void swap(pair& other);
    };

    template <class T1, class T2>
    pair<T1, T2>& pair<T1, T2>::operator= (const pair<T1, T2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <class T1, class T2>
    template <class T3, class T4>
    pair<T1, T2>& pair<T1, T2>::operator= (const pair<T3, T4>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <class T1, class T2>
    inline void
    pair<T1, T2>::swap(pair<T1, T2>& other) {
        swap(first, other.first);
        swap(second, other.second);
    }

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
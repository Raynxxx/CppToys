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
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

    public:
        pair(): first(), second() {}
        pair(const T1& a, const T2& b): first(a), second(b) {}
        pair(const pair<T1, T2>& other) : first(other.first), second(other.second) {}

    };

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
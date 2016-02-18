/*
** Move.h
** Created by Rayn on 2016/02/04
*/
#ifndef _MOVE_H_
#define _MOVE_H_

#include "TypeTraits.h"

namespace rayn {

    // forward
    template <typename T>
    T&& forward(typename remove_reference<T>::type& arg) {
        return static_cast<T&&>(arg);
    }

    template <typename T>
    T&& forward(typename remove_reference<T>::type&& arg) {
        return static_cast<T&&>(arg);
    }

    // move
    template <typename T>
    typename remove_reference<T>::type&&
    move(T&& arg) {
        return static_cast<typename remove_reference<T>::type&&>(arg);
    }

    // swap with move
    template <class T>
    inline void swap(T& a, T& b) {
        T temp = move(a);
        a = move(b);
        b = move(temp);
    }

    // swap overloaded for array types.
    // Swap the contents of two arrays.
    template <typename T, size_t Size>
    inline void swap(T (&a)[Size], T (&b)[Size]) {
        for (size_t i = 0; i < Size; ++i) {
            swap(a[i], b[i]);
        }
    }

}

#endif
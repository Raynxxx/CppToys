/*
** AlgoBase.h
** Created by Rayn on 2015/02/25
*/
#ifndef _ALGO_BASE_H_
#define _ALGO_BASE_H_

#include <cstring>

#include "Allocator.h"
#include "Iterator.h"
#include "TypeTraits.h"

namespace rayn {

    /*
    ** void fill(first, last, value);
    ** @brief       Fill the Range [first, last] with value.
    ** @complexity  O(N)
    */
    template <class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for (; first != last; ++first) {
            *first = value;
        }
    }
    //C-style string 偏特化
    inline void fill(char *first, char *last, const char& value) {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(char));
    }
    inline void fill(wchar_t *first, wchar_t *last, const wchar_t& value) {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
    }

    /*
    ** iter fill_n(first, n, value);
    ** @brief       Fill the Range [first, first + n] with value.
    ** @return      return the iterator point to the (first + n).
    ** @complexity  O(N)
    */
    template <class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
        for (; n > 0; --n, ++first) {
            *first = value;
        }
        return first;
    }
    //C-style string 偏特化
    template <class Size>
    inline char *fill_n(char *first, Size n, const char& value) {
        memset(first, static_cast<unsigned char>(value), n * sizeof(char));
        return first + n;
    }
    template <class Size>
    inline wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value) {
        memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
        return first + n;
    }

    /*
    ** T min(a, b);
    ** @brief       Get the minimum value.
    ** @complexity  O(1)
    */
    template <class T>
    inline const T& min(const T& a, const T& b) {
        return b < a ? b : a;
    }
    /*
    ** T min(a, b, comp);
    ** @brief       Get the minimum value using comp().
    ** @complexity  O(1)
    */
    template <class T, class Compare>
    inline const T& min(const T& a, const T& b, Compare comp) {
        return comp(b, a) ? b : a;
    }
    /*
    ** T max(a, b);
    ** @brief       Get the maximum value.
    ** @complexity  O(1)
    */
    template <class T>
    inline const T& max(const T& a, const T& b) {
        return a < b ? b : a;
    }
    /*
    ** T max(a, b, comp);
    ** @brief       Get the maximum value using comp().
    ** @complexity  O(1)
    */
    template <class T, class Compare>
    inline const T& max(const T& a, const T& b, Compare comp) {
        return comp(a, b) ? b : a;
    }

    /*
    ** boolean equal(first1, last1, first2);
    ** @brief       Judge seq1 and seq2 equal or not.
    ** @complexity  O(N)
    */
    template <class InputIterator1, class InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2) {
        //遍历[first1, last1), 同时递增first2
        //如果序列1的元素多于序列2, 会发生不可预料的错误.
        for (; first1 != last1; ++first1, ++first2) {
            if (*first1 != *first2) {
                return false;
            }
        }
        return true;
    }
    /*
    ** boolean equal(first1, last1, first2, binary_pred);
    ** @brief       Judge seq1 and seq2 equal or not with BinaryPredicate().
    ** @complexity  O(N)
    */
    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
    inline bool equal(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, BinaryPredicate binary_pred) {
        for (; first1 != last1; ++first1, ++first2) {
            if (!binary_pred(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }
    /*
    ** void swap(a, b);
    ** @brief       Swap two object
    ** @complexity  O(1)
    */
    template <class T>
    inline void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    
}

#endif
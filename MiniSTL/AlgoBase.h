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
    void fill(char *first, char *last, const char& value) {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(char));
    }
    void fill(wchar_t *first, wchar_t *last, const wchar_t& value) {
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
    char *fill_n(char *first, Size n, const char& value) {
        memset(first, static_cast<unsigned char>(value), n * sizeof(char));
        return first + n;
    }
    template <class Size>
    wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value) {
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


}

#endif
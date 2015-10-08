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
    //-----------------------------------------------------------------
    // fill
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
    //C-style string specialization
    inline void fill(char *first, char *last, const char& value) {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(char));
    }
    inline void fill(wchar_t *first, wchar_t *last, const wchar_t& value) {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
    }

    //-----------------------------------------------------------------
    // fill_n
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
    //C-style string specialization
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

    
    //-----------------------------------------------------------------
    // min
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

    //-----------------------------------------------------------------
    // max
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


    //-----------------------------------------------------------------
    // equal
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

    //-----------------------------------------------------------------
    // swap
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

    //-----------------------------------------------------------------
    // lexicographical_compare
    template <class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 < *first2)  return true;
            if (*first2 < *first1)  return false;
        }
        return first1 == last1 && first2 != last2;
    }
    template <class InputIterator1, class InputIterator2, class Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        Compare comp) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (comp(*first1, *first2)) return true;
            if (comp(*first2, *first1)) return false;
        }
        return first1 == last1 && first2 != last2;
    }
    // specialization
    bool lexicographical_compare(const unsigned char *first1,
        const unsigned char *last1,
        const unsigned char *first2,
        const unsigned char *last2) {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int res = memcmp(first1, first2, min(len1, len2));
        return res != 0 ? res < 0 : len1 < len2;
    }
    
    //-----------------------------------------------------------------
    // copy
    template <class RandomAccessIterator, class OutputIterator, class Distance>
    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
        OutputIterator result, Distance*) {
        for (Distance n = last - first; n > 0; --n, ++first, ++result) {
            *result = *first;
        }
        return result;
    }
    template <class T>
    inline T* __copy_t(const T* first, const T* last, T* result, _true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template <class T>
    inline T* __copy_t(const T* first, const T* last, T* result, _false_type) {
        return __copy_d(first, last, result, (ptrdiff_t*) 0);
    }
    template <class InputIterator, class OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, 
        OutputIterator result, const input_iterator_tag&) {
        for (; first != last; ++first, ++result) {
            *result = *first;
        }
        return result;
    }
    template <class RandomAccessIterator, class OutputIterator, class Distance>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
        OutputIterator result, const random_access_iterator_tag&) {
        return __copy_d(first, last, result, distance_type(first));
    }

    template <class InputIterator, class OutputIterator>
    struct __copy_dispatch {
        OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, iterator_category(first));
        }
    };
    template <class T>
    struct __copy_dispatch< T*, T* > {
        T* operator() (T* first, T* last, T* result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator type;
            return __copy_t(first, last, result, type());
        }
    };
    template <class T>
    struct __copy_dispatch< const T*, T* > {
        T* operator() (T* first, T* last, T* result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator type;
            return __copy_t(first, last, result, type());
        }
    };

    template <class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }
    // specialization
    inline char* copy(const char* first, const char* last, char* result) {
        memmove(result, first, sizeof(char) * (last - first));
        return result + (last - first);
    }
    inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    //-----------------------------------------------------------------
    // copy_backward
    template <class RandomAccessIterator, class BidirectionalIterator, class Distance>
    inline BidirectionalIterator __copy_backward_d(RandomAccessIterator first, RandomAccessIterator last,
        BidirectionalIterator result, Distance*) {
        for (Distance n = last - first; n > 0; --n) {
            *--result = *--last;
        }
        return result;
    }
    template <class T>
    inline T* __copy_backward_t(const T* first, const T* last, T* result, _true_type) {
        const ptrdiff_t num = last - first;
        memmove(result - num, first, num);
        return result - num;
    }
    template <class T>
    inline T* __copy_backward_t(const T* first, const T* last, T* result, _false_type) {
        return __copy_backward_d(first, last, result, (ptrdiff_t*)0);
    }
    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
        BidirectionalIterator2 result, const bidirectional_iterator_tag&) {
        while (last != first) {
            *--result = *--last;
        }
        return result;
    }
    template <class RandomAccessIterator, class BidirectionalIterator>
    inline BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last,
        BidirectionalIterator result, const random_access_iterator_tag&) {
        return __copy_backward_d(first, last, result, distance_type(first));
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    struct __copy_backward_dispatch {
        BidirectionalIterator2 operator() (BidirectionalIterator1 first, BidirectionalIterator1 last,
            BidirectionalIterator2 result) {
            return __copy_backward(first, last, result, iterator_category(first));
        }
    };
    template <class T>
    struct __copy_backward_dispatch < T*, T* > {
        T* operator() (T* first, T* last, T* result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator type;
            return __copy_t(first, last, result, type());
        }
    };
    template <class T>
    struct __copy_backward_dispatch < const T*, T* > {
        T* operator() (const T* first, const T* last, T* result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator type;
            return __copy_t(first, last, result, type());
        }
    };

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward_dispatch<BidirectionalIterator, BidirectionalIterator>()(first, last, result);
    }
    // specialization
    inline char* copy_backward(const char* first, const char* last, char* result) {
        const ptrdiff_t num = last - first;
        memmove(result - num, first, num);
        return result - num;
    }
    inline wchar_t* copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        const ptrdiff_t num = last - first;
        memmove(result - num, first, num);
        return result - num;
    }

}

#endif
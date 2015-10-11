/*
** Heap.h
** Created by Rayn on 2015/10/09
*/
#ifndef _HEAP_H_
#define _HEAP_H_

#include <functional>
#include "Iterator.h"

namespace rayn {

    template <class RandomAccessIterator, class Distance,
              class T, class Compare>
    void __push_heap(RandomAccessIterator first,
                     Distance holeIndex,
                     Distance topIndex,
                     T value, Compare comp)
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(*(first + parent), value)) {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    /*
    ** @brief   Push an element onto a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap + element.
    */
    template <class RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first,
                          RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;
        typedef typename iterator_traits<RandomAccessIterator>::difference_type
            DifferenceType;

        __push_heap(first, DifferenceType(last - first - 1),
                    DifferenceType(0), ValueType(*(last - 1)),
                    std::less<ValueType>());
        // TODO
        // first holeIndex = last - first - 1
    }

    /*
    ** @brief   Push an element onto a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap + element.
    ** @param   comp    Comparison functor.
    */
    template <class RandomAccessIterator, class Compare>
    inline void push_heap(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;
        typedef typename iterator_traits<RandomAccessIterator>::difference_type
            DifferenceType;

        __push_heap(first, DifferenceType(last - first - 1),
                    DifferenceType(0), ValueType(*(last - 1)),
                    comp);
    }


    template <class RandomAccessIterator, class Distance,
              class T, class Compare>
    void __adjust_heap(RandomAccessIterator first, 
                       Distance holeIndex,
                       Distance len, T value,
                       Compare comp)
    {
        Distance topIndex = holeIndex;
        Distance secondChild = holeIndex * 2 + 2;
        while (secondChild < len) {
            // have right child, set secondchild = max(left, right)
            if (comp(*(first + secondChild), *(first + (secondChild - 1)))) {
                secondChild--;
            }
            *(first + holeIndex) = *(first + secondChild);
            holeIndex = secondChild;
            secondChild = holeIndex * 2 + 2;
        }
        if (secondChild == len) {
            // have not right child
            *(first + holeIndex) = *(first + (secondChild - 1));
            holeIndex = secondChild - 1;
        }
        __push_heap(first, holeIndex, topIndex, value, comp);
    }

    /*
    ** @brief   Pop an element off a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    */
    template <class RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first,
                         RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;
        typedef typename iterator_traits<RandomAccessIterator>::difference_type
            DifferenceType;

        // set last = first, then result = last, it can be pop_back()
        // finally, adjust value in [first, last - 1)
        ValueType value = *(last - 1);
        *(last - 1) = *first;
        DifferenceType len = last - first - 1;
        __adjust_heap(first, DifferenceType(0), len, value, 
                      std::less<ValueType>());
    }

    /*
    ** @brief   Pop an element off a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    ** @param   comp    Comparison functor.
    */
    template <class RandomAccessIterator, class Compare>
    inline void pop_heap(RandomAccessIterator first,
                         RandomAccessIterator last,
                         Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;
        typedef typename iterator_traits<RandomAccessIterator>::difference_type
            DifferenceType;

        ValueType value = *(last - 1);
        *(last - 1) = *first;
        DifferenceType len = last - first - 1;
        __adjust_heap(first, DifferenceType(0), len, value, comp);
    }

    /*
    ** @brief   Sort a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    */
    template <class RandomAccessIterator>
    inline void sort_heap(RandomAccessIterator first,
                          RandomAccessIterator last)
    {
        while (last - first > 1)
            pop_heap(first, last--);
    }

    /*
    ** @brief   Sort a heap.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    ** @param   comp    Comparison functor.
    */
    template <class RandomAccessIterator, class Compare>
    inline void sort_heap(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Compare comp)
    {
        while (last - first > 1)
           pop_heap(first, last--, comp);
    }


    template <class RandomAccessIterator, class Distance,
              class T, class Compare>
    void __make_heap(RandomAccessIterator first,
                     RandomAccessIterator last,
                     T*, Distance*, Compare comp)
    {
        if (last - first < 2) return;
        Distance len = last - first;
        Distance parent = (len - 2) / 2;

        while (true) {
            __adjust_heap(first, parent, len, T(*(first + parent)), comp);
            if (parent == 0) return;
            parent--;
        }
    }


    /*
    ** @brief   Construct a heap over a range.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    */
    template <class RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first,
                          RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;

        __make_heap(first, last, value_type(first), distance_type(first),
                    std::less<ValueType>());
    }

    /*
    ** @brief   Construct a heap over a range.
    ** @param   first   Start of heap.
    ** @param   last    End of heap.
    ** @param   comp    Comparison functor.
    */
    template <class RandomAccessIterator, class Compare>
    inline void make_heap(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type
            ValueType;

        __make_heap(first, last, value_type(first), distance_type(first),
                    comp);
    }
}

#endif
/*
** Uninitialized.h
** Created by Rayn on 2015/01/30
*/
#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Algorithm.h"

namespace rayn {
    /********** uninitialized_copy **********/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _true_type) {
        return copy(first, last, result);
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, _false_type) {
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            construct(&*cur, *first);
        }
        return cur;
    }
    template<class InputIterator, class ForwardIterator, class T>
    ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
                                        ForwardIterator result, T*) {
        typedef typename _type_traits<T>::is_POD_type is_POD;
        return _uninitialized_copy_aux(first, last, result, is_POD());
    }
    /*
    ** @brief Copy the range [first, last) into result.
    ** @param first     An input iterator
    ** @param last      An input iterator
    ** @param result    An forward iterator
    ** @return result + (last - first)
    */
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return _uninitialized_copy(first, last, result, value_type(result));
    }

    /********** uninitialized_fill **********/
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type) {
        //TODO
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type) {
        ForwardIterator cur = first;
        for (; cur != last; ++cur) {
            construct(&*cur, value);
        }
    }
    template<class ForwardIterator, class T, class T1>
    void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*) {
        typedef typename _type_traits<T1>::is_POD_type is_POD_type;
        _uninitialized_fill_aux(first, last, value, is_POD_type());
    }
    /*
    ** @brief Fill the value xxx into range [first, last).
    ** @param first     An forward iterator
    ** @param last      An forward iterator
    ** @param value     The source value
    ** @return Nothing.
    */
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
        _uninitialized_fill(first, last, value, value_type(first));
    }


    /********** uninitialized_fill_n **********/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type) {
        return rayn::fill_n(first, n, value);
    }
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type) {
        ForwardIterator cur = first;
        for (; n > 0; --n, ++cur) {
            construct(&*cur, value);
        }
        return cur;
    }
    template<class ForwardIterator, class Size, class T, class T1>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& value, T1*) {
        typedef typename _type_traits<T1>::is_POD_type is_POD_type;
        return _uninitialized_fill_n_aux(first, n, value, is_POD_type());
    }
    /*
    ** @brief Fill the value xxx into range [first, first + n).
    ** @param first     An forward iterator
    ** @param n         The number of obj to fill with value 
    ** @param value     The source value
    ** @return first + n
    */
    template<class ForwardIterator, class Size, class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
        return _uninitialized_fill_n(first, n, value, value_type(first));
    }
}

#endif
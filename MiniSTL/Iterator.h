/*
** Iterator.h
** Created by Rayn on 2015/01/30
*/
#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <cstddef>

namespace rayn {

    /*
    ** Input Iterator: 只读 read only
    ** Output Iterator: 只写 write only
    ** Forward Iterator: 写入型算法，单向移动，区间读写操作
    ** Bidirectional Iterator: 双向移动，区间读写操作
    ** Random Access Iterator: 支持原生指针的全部能力
    */

    //标记迭代器类型，STL规定，对于算法，使用所能接受之最低阶迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template <class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag	iterator_category;
        typedef T					value_type;
        typedef Distance			difference_type;
        typedef T*					pointer;
        typedef T&					reference;
    };
    struct output_iterator {
        typedef output_iterator_tag	iterator_category;
        typedef void				value_type;
        typedef void				difference_type;
        typedef void				pointer;
        typedef void				reference;
    };
    template <class T, class Distance>
    struct forward_iterator {
        typedef forward_iterator_tag	iterator_category;
        typedef T						value_type;
        typedef Distance				difference_type;
        typedef T*						pointer;
        typedef T&						reference;
    };
    template <class T, class Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T*							pointer;
        typedef T&							reference;
    };
    template <class T, class Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef Distance					difference_type;
        typedef T*							pointer;
        typedef T&							reference;
    };

    template <class Category, class T, class Distance = ptrdiff_t,
                class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category	iterator_category;
        typedef T			value_type;
        typedef Distance	difference_type;
        typedef Pointer		pointer;
        typedef Reference	reference;
    };

    //Iterator traits 定义
    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category	iterator_category;
        typedef typename Iterator::value_type			value_type;
        typedef typename Iterator::difference_type		difference_type;
        typedef typename Iterator::pointer				pointer;
        typedef typename Iterator::reference 			reference;
    };

    //为原生指针(native pointer)设计的 traits 偏特化
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef ptrdiff_t					difference_type;
        typedef T*							pointer;
        typedef T&							reference;
    };
    //为原生之 pointer to const 设计的 traits 偏特化
    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag	iterator_category;
        typedef T							value_type;
        typedef ptrdiff_t					difference_type;
        typedef const T*					pointer;
        typedef const T&					reference;
    };
    //决定某个迭代器的类型 category
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
        iterator_category(const Iterator& iter) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }
    //决定某个迭代器的类型 value type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
        value_type(const Iterator& iter) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }
    //决定某个迭代器的类型 difference type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
        distance_type(const Iterator& iter) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }
    
    
    template <class InputIterator, class Distance>
    inline void _advance(InputIterator& it, Distance n, input_iterator_tag) {
        while (n--) ++it;
    }
    template <class BidirectionalIterator, class Distance>
    inline void _advance(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--) ++it;
        } else {
            while (n++) --it;
        }
    }
    template <class RandomAccessIterator, class Distance>
    inline void _advance(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
        it += n;
    }
    /*
    ** @brief Increase n step at the iterator
    ** @param it The increased iterator
    ** @param n The distance that the iterator will be increased.
    ** @return Nothing.
    */
    template <class InputIterator, class Distance>
    inline void advance(InputIterator& it, Distance n) {
        _advance(it, n, iterator_category(it));
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
        _distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }
    template <class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
        _distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        return last - first;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
        distance(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        _distance(first, last, category());
    }
}

#endif
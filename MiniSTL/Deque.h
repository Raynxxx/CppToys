/*
** Deque.h
** Created by Rayn on 2015/04/25
*/
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "Allocator.h"
#include "TypeTraits.h"
#include "Iterator.h"
#include "ReverseIterator.h"

namespace rayn {
    /*
    ** if n == 0, 表示bufsize使用默认值
    ** if sz < 512, 传回 512/sz
    ** if sz >= 512, 传回 1
    */
    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template <class T, class Ref, class Ptr, size_t BufSize>
    struct __deque_iterator {
        typedef __deque_iterator                                    self;
        typedef __deque_iterator<T, T&, T*, BufSize>                iterator;
        typedef __deque_iterator<T, const T&, const T*, BufSize>    const_iterator;

        static size_t buffer_size() {
            return __deque_buf_size(BufSize, sizeof(T));
        }

        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Ptr                         pointer;
        typedef Ref                         reference;
        typedef ptrdiff_t                   difference_type;
        typedef size_t                      size_type;
        typedef T*                          ele_pointer;
        typedef T**                         map_pointer;

        
        ele_pointer cur;    //所指缓存区当前元素
        ele_pointer first;  //所指缓冲区头部
        ele_pointer last;   //所指缓冲区尾后 (含备用空间)
        map_pointer node;   //指向管控中心

        __deque_iterator() : cur(), first(), last(), node() {}
        __deque_iterator(ele_pointer x, map_pointer y) :
            cur(x),
            first(*y),
            last(*y + difference_type(buffer_size())), 
            node(y)
        {}
        __deque_iterator(const iterator& other) :
            cur(other.cur),
            first(other.first),
            last(other.last),
            node(other.node)
        {}

        iterator _const_cast() const {
            return iterator(cur, node);
        }

        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator* () const { return *cur; }
        pointer operator-> () const { return cur; }
        
        self& operator++ () {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++ (int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator-- () {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator-- (int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
        self& operator+= (difference_type n) {
            difference_type offset = n + (cur - first);
            if (offset >= 0 && offset < difference_type(buffer_size())) {
                //目标在同一缓冲区中
                cur += n;
            } else {
                difference_type node_offset = offset > 0 ?
                    offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }
        self operator+ (difference_type n) const {
            self tmp = *this;
            return tmp += n;
        }
        self& operator-= (difference_type n) {
            return *this += -n;
        }
        self operator- (difference_type n) const {
            self tmp = *this;
            return tmp -= n;
        }

        difference_type operator- (const self& other) const {
            return difference_type(buffer_size()) * (node - other.node - 1)
                + (cur - first) + (other.last - other.cur);
        }

        reference operator[] (difference_type n) const { return *(*this + n) }
        bool operator== (const self& other) const {
            return cur == other.cur;
        }
        bool operator!= (const self& other) const {
            return !(*this == other);
        }
        bool operator< (const self& other) const {
            return (node == other.node) ? (cur < other.cur) : (node < other.node);
        }
        bool operator> (const self& other) const {
            return other < *this;
        }
    };

    template <class T, size_t BufSize = 0>
    class deque {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef T&          reference;
        typedef const T*    const_pointer;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef __deque_iterator<T, reference, pointer, BufSize>                iterator;
        typedef __deque_iterator<T, const_reference, const_pointer, BufSize>    const_iterator;
        typedef reverse_iterator_t<iterator>                                    reverse_iterator;
        typedef reverse_iterator_t<const_iterator>                              const_reverse_iterator;

    protected:
        // pointer of pointer of T
        typedef pointer*    map_pointer;

    //Data member
    protected:
        iterator    _start;
        iterator    _finish;
        map_pointer map;    //指向map, map是块连续空间,每个元素是一个指针,指向一块缓存区
        size_type   map_size;   //map内可以容纳多少指针

    //Public member and method
    public:
        deque();
        deque(const deque& other);

        iterator begin() { return _start; }
        iterator end() { return _finish; }

    //Private member and method
    private:

    };
}

#endif
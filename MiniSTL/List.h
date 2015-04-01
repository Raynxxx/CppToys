#ifndef _LIST_H_
#define _LIST_H_

#include <type_traits>

#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Uninitialized.h"

namespace rayn {
    template <class T>
    class list;

    // 列表节点
    template <class T>
    struct __list_node {
        typedef __list_node* pointer;
        pointer prev;
        pointer next;
        T data;
        list<T>* container;
        __list_node(const T& d, pointer p, pointer n, list<T> *c):
            data(d), prev(p), next(n), container(c) {}

        bool operator== (const __list_node& other) {
            return data == other.data && prev == other.prev
                && next == other.next && container == other.container;
        }
    };

    // 列表迭代器
    template <class T>
    struct __list_iterator : public bidirectional_iterator <T, ptrdiff_t> {
        typedef __list_iterator<T>  iterator;
        typedef __list_iterator<T>  self;

        typedef __list_node<T>*     link_type;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;

        // 指向list的节点
        link_type node;

        // Constructor
        __list_iterator(): node(nullptr) {}
        __list_iterator(link_type x) : node(x) {}
        __list_iterator(const iterator& x) : node(x.node) {}

        inline bool operator== (const self& x) const { return node == x.node; }
        inline bool operator!= (const self& x) const { return node != x.node; }
        reference operator* () const { return node->data; }
        pointer operator-> () const { return &(operator*()); }

        self& operator++ () {
            node = node->next;
            return *this;
        }
        self operator++ (int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator-- () {
            node = node->prev;
            return *this;
        }
        self operator-- (int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <class T>
    class list {
    protected:
        typedef __list_node<T>                  list_node;
        typedef allocator<list_node>            node_allocator;
        typedef list_node*                      link_type;
    public:
        typedef T                               value_type;
        typedef T&                              reference;
        typedef __list_iterator<T>              iterator;
        typedef reverse_iterator_t<iterator>    reverse_iterator;
        typedef size_t                          size_type;

    protected:
        link_type node;

    public:
        list() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }


        iterator begin() { return node->next; }
        iterator end() { return node; }
        bool empty() const {
            return node->next == node;
        }

    protected:
        link_type get_node() {
            return node_allocator::allocate();
        }
        void put_node(link_type p) {
            node_allocator::deallocate(p);
        }
        link_type create_node(const value_type& val) {
            link_type p = get_node();
            rayn::construct(&(p->data), val);
            return p;
        }
        void destory_node(link_type p) {
            rayn::destroy(&(p->data));
            put_node(p);
        }
    };

}

#endif
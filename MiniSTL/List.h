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
    struct __list_iterator : public bidirectional_iterator<T, ptrdiff_t> {
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
        typedef __list_node<T>                      list_node;
        typedef allocator<list_node>                node_allocator;
        typedef list_node*                          link_type;
    public:
        typedef T                                   value_type;
        typedef T&                                  reference;
        typedef const T&                            const_reference;
        typedef __list_iterator<T>                  iterator;
        typedef __list_iterator<const T>            const_iterator;
        typedef reverse_iterator_t<iterator>        reverse_iterator;
        typedef reverse_iterator_t<const_iterator>  const_reverse_iterator;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

    protected:
        link_type node;

    public:
        // Default Constructor
        list() {
            empty_initialize();
        }
        explicit list(size_type count);
        list(size_type count, const T& value);
        template <class InputIterator>
        list(InputIterator first, InputIterator last);
        list(const list& other);
        list(list&& other);
        ~list();

        list& operator== (const list& other);
        list& operator== (list&& other);

        void assign(size_type count, const T& value);
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);

        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        iterator begin() { return node->next; }
        const_iterator begin() const;
        const_iterator cbegin() const;

        iterator end() { return node; }
        const_iterator end() const;
        const_iterator cend() const;

        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator crbegin() const;

        reverse_iterator rend();
        const_reverse_iterator rend() const;
        const_reverse_iterator crend() const;

        bool empty() const {
            return node->next == node;
        }
        size_type size() const;
        
        void clear();
        iterator insert(iterator position, const T& value);
        iterator insert(const_iterator position, const T& value);
        iterator insert(const_iterator position, T&& value);
        iterator erase(iterator position);
        void push_back(const T& x);
        void pop_back();
        void push_front(const T& x);
        void pop_front();
        void resize();
        void swap();
        
        void merge(list& x);
        void spiice(iterator position, list& x);
        void spiice(iterator position, list& x, iterator i);
        void spiice(iterator position, list& x, iterator first, iterator last);
        void remove(const T& value);
        void remove_if();
        void reverse();
        void unique();
        void sort();

    public:
        friend bool operator== (list& lhs, list& rhs);
        friend bool operator!= (list& lhs, list& rhs);
        friend bool operator< (list& lhs, list& rhs);
        friend bool operator<= (list& lhs, list& rhs);
        friend bool operator> (list& lhs, list& rhs);
        friend bool operator>= (list& lhs, list& rhs);
        friend void swap(list& lhs, list& rhs);

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

        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        void transfer(iterator position, iterator first, iterator last);

    };

}

#endif
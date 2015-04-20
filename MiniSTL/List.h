/*
** List.h
** Created by Rayn on 2015/03/02
** List 定义
*/
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
        self _const_cast() const {
            return *this;
        }
    };

    template <class T>
    class list {
    protected:
        typedef __list_node<T>                      list_node;
        typedef allocator<list_node>                node_allocator;
        typedef list_node*                           link_type;
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
            this->empty_initialize();
        }
        // Constructor with count elements.
        explicit list(size_type count) {
            this->empty_initialize();
            this->insert(end(), count, value_type());
        }
        list(size_type count, const T& value) {
            this->empty_initialize();
            this->insert(begin(), count, value);
        }
        // Contructor with Range [first, last).
        template <class InputIterator>
        list(InputIterator first, InputIterator last) {
            this->empty_initialize();
            this->insert(end(), first, last);
        }
        // Copy Contructor
        list(const list& other) {
            this->empty_initialize();
            iterator first = other.begin();
            iterator last = other.end();
            for (auto it = first; it != last; ++it) {
                this->push_back(*it);
            }
        }
        list(list&& other) {
            this->empty_initialize();
            this->swap(other);
        }
        ~list() {
            this->clear();
        }

        list& operator= (const list& other) {
            this->assign(other.begin(), other.end());
        }
        list& operator= (list&& other) {
            this->clear();
            this->swap(other);
            return *this;
        }

        void assign(size_type count, const T& value) {
            iterator it = begin();
            for (; it != end() && count > 0; ++it, --count) {
                *it = value;
            }
            if (count > 0) {
                this->insert(end(), count, value);
            } else {
                this->erase(it, end());
            }
        }
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last) {
            iterator first1 = begin(), last1 = end();
            for (; first1 != last1, first != last;  ++first1, ++first) {
                *first1 = *first;
            }
            if (first == last) {
                this->erase(first1, last1);
            } else {
                this->insert(last1, first, last);
            }
        }

        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(--end()); }
        const_reference back() const { return *(--end()); }

        iterator begin() { return node->next; }
        const_iterator begin() const { return node->next; }
        const_iterator cbegin() const { return node->next; }

        iterator end() { return node; }
        const_iterator end() const { return node; }
        const_iterator cend() const { return node; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return reverse_iterator(begin()); }
        const_reverse_iterator crend() const { return reverse_iterator(begin()); }

        bool empty() const { return node->next == node; }
        size_type size() const {
            return distance(begin(), end());
        }
        
        void clear();
        iterator insert(const_iterator position, const T& value);
        iterator insert(const_iterator position, size_type count, const T& value);
        template <class InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last);

        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T& value) { insert(end(), value); }
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        void push_front(const T& value) { insert(begin(), value); }
        void pop_front() { erase(begin()); }

        void resize(size_type count);
        void resize(size_type count, const T& value);
        void swap(list& other);
        
        void merge(list& other);
        template <class Compare>
        void merge(list& other, Compare comp);

        void splice(iterator position, list& other);
        void splice(iterator position, list& other, iterator it);
        void splice(iterator position, list& other, const_iterator first,
            const_iterator last);

        void remove(const T& value);
        template <class UnaryPredicate>
        void remove_if(UnaryPredicate p);

        void reverse();

        void unique();
        template <class BinaryPredicate>
        void unique(BinaryPredicate p);

        void sort();
        template <class Compare>
        void sort(Compare comp);

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
        link_type create_node(const value_type& value) {
            link_type p = get_node();
            rayn::construct(&(p->data), value);
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
        iterator _const_cast(const_iterator cit) {
            return iterator(const_cast<link_type>(cit.node));
        }
        /*
        ** @brief   Move [first, last) to the front of position.
        */
        void transfer(iterator position, iterator first, iterator last) {
            if (last != position) {
                last.node->prev->next = position;
                first.node->prev->next = last.node;
                position.node->prev->next = first.node;
                link_type tmp = position.node->prev;
                position.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }
    };

    template <class T>
    void list<T>::clear() {
        link_type cur = node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = cur->next;
            destory_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    template <class T>
    typename list<T>::iterator
        list<T>::insert(const_iterator position, const T& value) {
        link_type tmp = create_node(value);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        position.node->prev->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }
    template <class T>
    typename list<T>::iterator
        list<T>::insert(const_iterator position, size_type count, const T& value) {
        if (n) {
            list tmp(n, value);
            iterator it = tmp.begin();
            splice(position, tmp);
            return it;
        }
        return iterator(position);
    }
    template <class T>
    template <class InputIterator>
    typename list<T>::iterator
        list<T>::insert(const_iterator position, InputIterator first, InputIterator last) {
        list tmp(first, last);
        if (!tmp.empty()) {
            iterator it = tmp.begin();
            splice(position, tmp);
            return it;
        }
        return iterator(position);
    }

    template <class T>
    typename list<T>::iterator
        list<T>::erase(const_iterator position) {
        link_type next_node = position.node->next;
        link_type prev_node = position.node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(position.node);
        return next_node;
    }
    template <class T>
    typename list<T>::iterator
        list<T>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return last;
    }

    template <class T>
    void list<T>::resize(size_type count) {
        iterator it = begin();
        size_type len = 0;
        for (; it != end() && len < count; ++it, ++len);
        if (len == count) {
            erase(it, end());
        } else {
            insert(end(), count - len, T());
        }
    }
    template <class T>
    void list<T>::resize(size_type count, const T& value) {
        iterator it = begin();
        size_type len = 0;
        for (; it != end() && len < count; ++it, ++len);
        if (len == count) {
            erase(it, end());
        } else {
            insert(end(), count - len, value);
        }
    }

    template <class T>
    void list<T>::swap(list<T>& other) {
        rayn::swap(node, other.node);
    }

    template <class T>
    void list<T>::merge(list& other) {
        iterator first1 = begin(), last1 = end();
        iterator first2 = other.begin(), last2 = other.end();
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
    }
    template <class T>
    template <class Compare>
    void list<T>::merge(list& other, Compare comp) {
        iterator first1 = begin(), last1 = end();
        iterator first2 = other.begin(), last2 = other.end();
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
    }

    template <class T>
    void list<T>::splice(iterator position, list& other) {
        if (!other.empty()) {
            transfer(position, other.begin(), other.end());
        }
    }
    template <class T>
    void list<T>::splice(iterator position, list& other, iterator it) {
        iterator next = it;
        ++next;
        if (position == it || position == next) return;
        transfer(position, it, next);
    }
    template <class T>
    void list<T>::splice(iterator position, list& other, const_iterator first,
        const_iterator last) {
        if (first != last) {
            transfer(position, first, last);
        }
    }

    template <class T>
    void list<T>::remove(const T& value) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            if (*first == value) {
                this->erase(first);
            }
            first = ++next;
        }
    }
    template <class T>
    template <class UnaryPredicate>
    void list<T>::remove_if(UnaryPredicate p) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            if (UnaryPredicate(*first)) {
                this->erase(first);
            }
            first = ++next;
        }
    }

    template <class T>
    void list<T>::reverse() {
        if (node->next == node || node->next->next == node) {
            return;
        }
        iterator cur = begin();
        ++cur;
        while (cur != end()) {
            iterator pre = cur;
            ++cur;
            transfer(begin(), pre, cur);
        }
    }

    template <class T>
    void list<T>::unique() {
        iterator first = begin();
        iterator last = end();
        if (first == last) return; // 空链表
        iterator next = first;
        while (++next != last) {
            if (*first == *next) {
                erase(next);
            } else {
                first = next;
            }
            next = first;
        }

    }
    template <class T>
    template <class BinaryPredicate>
    void list<T>::unique(BinaryPredicate p) {
        iterator first = begin();
        iterator last = end();
        if (first == last) return; // 空链表
        iterator next = first;
        while (++next != last) {
            if (BinaryPredicate(*first, *next)) {
                erase(next);
            } else {
                first = next;
            }
            next = first;
        }
    }

    template <class T>
    void list<T>::sort() {
        // size = 0 or size = 1, 无需操作
        if (node->next == node || node->next->next == node) {
            return;
        }
        list<T> carry;
        list<T> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]);
        }
        this->swap(counter[fill - 1]);
    }
    template <class T>
    template <class Compare>
    void list<T>::sort(Compare comp) {
        // size = 0 or size = 1, 无需操作
        if (node->next == node || node->next->next == node) {
            return;
        }
        list<T> carry;
        list<T> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry, comp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]);
        }
        this->swap(counter[fill - 1]);
    }
}

#endif
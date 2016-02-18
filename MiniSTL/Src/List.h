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

    template <class T> class list;

    // List Node
    template <class T>
    struct __list_node {
        typedef __list_node*    __node_ptr;

        __node_ptr  prev;
        __node_ptr  next;
        T           data;

        __list_node(const T& d, __list_node* p, __list_node* n) :
            data(d), prev(p), next(n) {}

        bool operator== (const __list_node& other) {
            return data == other.data && prev == other.prev
                && next == other.next;
        }

    };

    // List Iterator
    template <class T>
    struct __list_iterator {

        typedef __list_iterator<T>                  _self;
        typedef __list_node<T>                      _node;

        typedef rayn::bidirectional_iterator_tag    iterator_category;
        typedef ptrdiff_t                           difference_type;
        typedef size_t                              size_type;
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;

        // point to the list element
        __list_node<T>* node;

        __list_iterator()
            : node() {}

        explicit
        __list_iterator(__list_node<T>* x)
            : node(x) {}

        _self _const_cast() const {
            return *this;
        }

        reference operator* () const {
            return static_cast<_node*>(node)->data;
        }
        pointer operator-> () const {
            return &(static_cast<_node*>(node)->data);
        }

        _self& operator++ () {
            node = node->next;
            return *this;
        }
        _self operator++ (int) {
            _self tmp = *this;
            node = node->next;
            return tmp;
        }
        _self& operator-- () {
            node = node->prev;
            return *this;
        }
        _self operator-- (int) {
            self tmp = *this;
            node = node->prev;
            return tmp;
        }


        bool operator== (const _self& other) const {
            return node == other.node;
        }

        bool operator!= (const _self& other) const {
            return node != other.node;
        }

    };

    template <class T>
    struct __list_const_iterator {

        typedef __list_iterator<T>                  iterator;
        typedef __list_const_iterator<T>            _self;
        typedef const __list_node<T>                _node;

        typedef rayn::bidirectional_iterator_tag    iterator_category;
        typedef ptrdiff_t                           difference_type;
        typedef size_t                              size_type;
        typedef T                                   value_type;
        typedef const T*                            pointer;
        typedef const T&                            reference;
        

        // 指向list的节点
        const __list_node<T>* node;

        // Constructor
        __list_const_iterator()
            : node(nullptr) {}

        explicit __list_const_iterator(const __list_node<T>* x)
            : node(x) {}

        __list_const_iterator(const iterator& x)
            : node(x.node) {}

        iterator _const_cast() const {
            return iterator(const_cast<__list_node<T>*>(node));
        }

        reference operator* () const {
            return static_cast<_node*>(node)->data;
        }
        pointer operator-> () const {
            return &(static_cast<_node*>(node)->data);
        }

        _self& operator++ () {
            node = node->next;
            return *this;
        }
        _self operator++ (int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        _self& operator-- () {
            node = node->prev;
            return *this;
        }
        _self operator-- (int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
        
        bool operator== (const _self& x) const {
            return node == x.node;
        }

        bool operator!= (const _self& x) const {
            return node != x.node;
        }
    };

    template <class T>
    inline bool operator== (const __list_iterator<T>& x,
                            const __list_const_iterator<T>& y) {
        return x.node == y.node;
    }

    template <class T>
    inline bool operator!= (const __list_iterator<T>& x,
                            const __list_const_iterator<T>& y) {
        return x.node != y.node;
    }


    template <class T>
    class list {
    protected:
        typedef __list_node<T>                      list_node;
        typedef allocator<list_node>                node_allocator;

    public:
        typedef T                                   value_type;
        typedef T&                                  reference;
        typedef const T&                            const_reference;
        typedef __list_iterator<T>                  iterator;
        typedef __list_const_iterator<T>            const_iterator;
        typedef reverse_iterator_t<iterator>        reverse_iterator;
        typedef reverse_iterator_t<const_iterator>  const_reverse_iterator;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

    protected:
        list_node* node;

    public:
        // Default Constructor
        list() {
            empty_initialize();
        }
        // Constructor with count elements.
        explicit list(size_type count) {
            empty_initialize();
            insert(end(), count, value_type());
        }
        list(size_type count, const value_type& value) {
            empty_initialize();
            fill_initialize(count, value);
        }
        // Contructor with Range [first, last).
        template <class InputIterator>
        list(InputIterator first, InputIterator last) {
            typedef typename rayn::is_integral<InputIterator>::type Integer;
            empty_initialize();
            initialize_dispatch(first, last, Integer());
        }
        // Copy Contructor
        list(const list& other) {
            empty_initialize();
            initialize_dispatch(other.begin(), other.end(), false_type());
        }
        // Move Contructor
        list(list&& other) {
            empty_initialize();
            swap(other);
        }
        // Default Destroy Function
        ~list() {
            clear();
        }

        list& operator= (const list& other) {
            assign(other.begin(), other.end());
            return *this;
        }

        void assign(size_type count, const T& value) {
            fill_assign(count, value);
        }
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last) {
            typedef typename rayn::is_integral<InputIterator>::type Integer;
            assign_dispatch(first, last, Integer());
        }

        reference       front()         { return *begin(); }
        const_reference front() const   { return *begin(); }
        reference       back()          { return *(--end()); }
        const_reference back() const    { return *(--end()); }

        iterator        begin()         { return iterator(node->next); }
        const_iterator  begin() const   { return const_iterator(node->next); }
        const_iterator  cbegin() const  { return const_iterator(node->next); }

        iterator        end()           { return iterator(node); }
        const_iterator  end() const     { return const_iterator(node); }
        const_iterator  cend() const    { return const_iterator(node); }

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

        void splice(const_iterator position, list& other);
        void splice(const_iterator position, list& other, const_iterator it);
        void splice(const_iterator position, list& other, const_iterator first,
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

    protected:
        list_node* get_node() {
            return node_allocator::allocate();
        }
        void put_node(list_node* p) {
            node_allocator::deallocate(p);
        }
        list_node* create_node(const value_type& value) {
            list_node* p = get_node();
            rayn::construct(&(p->data), value);
            return p;
        }
        void destory_node(list_node* p) {
            rayn::destroy(&(p->data));
            put_node(p);
        }

        // initialize helper
        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        template <class Integer>
        void initialize_dispatch(Integer count, Integer value, rayn::true_type) {
            fill_initialize(static_cast<size_type>(count), value);
        }
        template <class InputIterator>
        void initialize_dispatch(InputIterator first, InputIterator last, rayn::false_type) {
            for (; first != last; ++first) {
                push_back(*first);
            }
        }
        void fill_initialize(size_type count, const value_type& value) {
            for (; count; --count)  {
                push_back(value);
            }
        }

        // assign helper
        template <class Integer>
        void assign_dispatch(Integer count, Integer value, rayn::true_type) {
            fill_assign(static_cast<size_type>(count), value);
        }
        template <class InputIterator>
        void assign_dispatch(InputIterator first2, InputIterator last2, rayn::false_type) {
            iterator first1 = begin(), last1 = end();
            for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
                *first1 = *first2;
            }
            if (first2 == last2) {
                erase(first1, last1);
            } else {
                insert(last1, first2, last2);
            }
        }
        void fill_assign(size_type count, const value_type& value) {
            iterator it = begin();
            for (; it != end() && count > 0; ++it, --count) {
                *it = value;
            }
            if (count > 0) {
                insert(end(), count, value);
            } else {
                erase(it, end());
            }
        }

        /*
        ** @brief   Move [first, last) to the front of position.
        */
        void transfer(iterator position, iterator first, iterator last) {
            if (last != position) {
                last.node->prev->next = position.node;
                first.node->prev->next = last.node;
                position.node->prev->next = first.node;
                list_node* tmp = position.node->prev;
                position.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }
    };

    template <class T>
    void list<T>::clear() {
        list_node* cur = node->next;
        while (cur != node) {
            list_node* tmp = cur;
            cur = cur->next;
            destory_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    template <class T>
    typename list<T>::iterator
        list<T>::insert(const_iterator position, const T& value) {
        list_node* tmp = create_node(value);
        iterator pos = position._const_cast();
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        pos.node->prev->next = tmp;
        pos.node->prev = tmp;
        return iterator(tmp);
    }

    template <class T>
    typename list<T>::iterator
        list<T>::insert(const_iterator position, size_type count, const T& value) {
        if (count) {
            list tmp(count, value);
            iterator it = tmp.begin();
            splice(position, tmp);
            return it;
        }
        return iterator(position._const_cast());
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
        return position._const_cast();
    }

    template <class T>
    typename list<T>::iterator
        list<T>::erase(const_iterator position) {
        list_node* next_node = position.node->next;
        list_node* prev_node = position.node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(position._const_cast().node);
        return iterator(next_node);
    }

    template <class T>
    typename list<T>::iterator
        list<T>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return last._const_cast();
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
    void list<T>::splice(const_iterator position, list& other) {
        if (!other.empty()) {
            transfer(position._const_cast(), other.begin(), other.end());
        }
    }

    template <class T>
    void list<T>::splice(const_iterator position, list& other, const_iterator it) {
        iterator next = it;
        ++next;
        if (position == it || position == next) return;
        transfer(position._const_cast(), it, next);
    }

    template <class T>
    void list<T>::splice(const_iterator position, list& other, const_iterator first,
                         const_iterator last) {
        if (first != last) {
            transfer(position._const_cast(), first, last);
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

    template <class T>
    inline bool operator== (list<T>& lhs, list<T>& rhs) {
        typedef typename list<T>::const_iterator const_iterator;
        const_iterator first1 = lhs.begin(), end1 = lhs.end();
        const_iterator first2 = rhs.begin(), end2 = rhs.end();
        while (first1 != end1 && first2 != end2 && *first1 == *first2) {
            ++first1, ++first2;
        }
        return first1 == end1 && first2 == end2;
    }

    template <class T>
    inline bool operator!= (list<T>& lhs, list<T>& rhs) {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator< (list<T>& lhs, list<T>& rhs) {
        typedef typename list<T>::const_iterator const_iterator;
        const_iterator first1 = lhs.begin(), end1 = lhs.end();
        const_iterator first2 = rhs.begin(), end2 = rhs.end();
        while (first1 != end1) {
            if (first2 == end2 || *first2 < *first1) {
                return false;
            } else if (*first1 < *first2) {
                return true;
            }
            ++first1, ++first2;
        }
        return first2 != end2;
    }

    template <class T>
    inline bool operator<= (list<T>& lhs, list<T>& rhs) {
        return !(rhs < lhs);
    }

    template <class T>
    inline bool operator> (list<T>& lhs, list<T>& rhs) {
        return rhs < lhs;
    }

    template <class T>
    inline bool operator>= (list<T>& lhs, list<T>& rhs) {
        return !(lhs < rhs);
    }

    template <class T>
    inline void swap(list<T>& lhs, list<T>& rhs) {
        lhs.swap(rhs);
    }
}

#endif
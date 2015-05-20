/*
** Deque.h
** Created by Rayn on 2015/04/25
*/
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include <stdexcept>

#include "Allocator.h"
#include "Uninitialized.h"
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
        typedef pointer*                map_pointer;
        typedef allocator<value_type>   data_allocator;
        typedef allocator<pointer>      map_allocator;
        enum { __initial_map_size = 8 };

    protected:
        // Data member
        iterator    _start;     //头部迭代器
        iterator    _finish;    //尾后迭代器
        map_pointer map;        //指向map, map是块连续空间,每个元素是一个指针,指向一块缓存区
        size_type   map_size;   //map内可以容纳多少指针
    
    public:
        // Default Contructor
        deque() : _start(), _finish(), map(0), map_size(0) {}
        // Constructor with count copies of elements.
        explicit deque(size_type count) {
            this->fill_initialize(count, value_type());
        }
        // Constructor with count copies of elements with value value.
        deque(size_type count, const value_type& value) {
            this->fill_initialize(count, value);
        }
        // Constructor with the contents of the range [first, last).
        template <class InputIterator>
        deque(InputIterator first, InputIterator last);
        // Copy Contructor
        deque(const deque& other);
        // Move Contructor
        deque(deque&& other);
        // Destructor
        ~deque();

        // Copy Assignment operator
        deque& operator= (const deque& other);
        // Move Assignment operator
        deque& operator= (deque&& other);

        /*
        ** @brief   Replaces the contents with count copies of value.
        */
        void assign(size_type count, const value_type& value);
        /*
        ** @brief   Replaces the contents with copies of those in the range [first, last).
        */
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);

        // Element access
        /*
        ** @brief       Returns a reference to the element at specified location pos.
        ** @exception   std::out_of_range()
        */
        reference at(size_type pos) {
            if (pos >= this->size()) {
                throw std::out_of_range("out of range.");
            }
            return _start[difference_type(n)];
        }
        const_reference at(size_type pos) const {
            if (pos >= this->size()) {
                throw std::out_of_range("out of range.");
            }
            return _start[difference_type(n)];
        }

        /*
        ** @brief   Returns a reference to the element at specified location pos.
        */
        reference operator[] (size_type n) {
            return _start[difference_type(n)];
        }
        const_reference operator[] (size_type n) const {
            return _start[difference_type(n)];
        }

        /*
        ** @brief   Returns a reference to the first element in the container.
        */
        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        /*
        ** @brief   Returns reference to the last element in the container.
        */
        reference back() {
            iterator tmp = end();
            --tmp;
            return *tmp;
        }
        const_reference back() const {
            iterator tmp = end();
            --tmp;
            return *tmp;
        }

        // Iterators
        /*
        ** @brief   Returns an iterator to the first element of the container.
        */
        iterator begin() { return this->_start; }
        const_iterator begin() const { return this->_start; }
        const_iterator cbegin() const { return this->_start; }
        /*
        ** @brief   Returns an iterator to the element following the last element of the container.
        */
        iterator end() { return this->_finish; }
        const_iterator end() const { return this->_finish; }
        const_iterator cend() const { return this->_finish; }
        /*
        ** @brief   Returns a reverse iterator to the first element of the reversed container.
        */
        reverse_iterator rbegin() { return reverse_iterator(this->_finish); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(this->_finish); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(this->_finish); }
        /*
        ** @brief   Returns a reverse iterator to the element following 
        **          the last element of the reversed container. 
        */
        reverse_iterator rend() { return reverse_iterator(this->_start); }
        const_reverse_iterator rend() const { return const_reverse_iterator(this->_start); }
        const_reverse_iterator crend() const { return const_reverse_iterator(this->_start); }

        // Capacity
        /*
        ** @brief   Checks if the container has no elements
        */
        bool empty() { return _finish == _start; }
        /*
        ** @brief   Returns the number of elements in the container.
        */
        size_type size() const { return _finish - _start; }
        /*
        ** @brief   Returns the maximum number of elements the container is
        **          able to hold due to system or library implementation limitations
        */
        size_type max_size() const {
            return size_type(-1);
        }
        /*
        ** @brief   Requests the removal of unused capacity.
        */
        void shrink_to_fit();

        // Modifiers
        // Removes all elements from the container.
        void clear();
        // inserts value before pos
        iterator insert(const_iterator pos, const value_type& value);
        // inserts count copies of the value before pos
        iterator insert(const_iterator pos, size_type count, const value_type& value);
        // inserts elements from range [first, last) before pos.
        template <class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
        // Removes the element at pos.
        iterator erase(const_iterator pos);
        // Removes the elements in the range [first, last).
        iterator erase(const_iterator first, const_iterator last);

        // Appends the given element value to the end of the container.
        void push_back(const value_type& value);
        // Removes the last element of the container.
        void pop_back();
        // Prepends the given element value to the beginning of the container.
        void push_front(const value_type& value);
        // Removes the first element of the container.
        void pop_front();

        // Resizes the container to contain count elements.
        void resize(size_type count);
        void resize(size_type count, const value_type& value);
        // Exchanges the contents of the container with those of other. 
        void swap(deque& other);
    
    private:
        //private member and method
        static size_t buffer_size() {
            return __deque_buf_size(BufSize, sizeof(T));
        }
        void create_nodes(map_pointer start, map_pointer finish);
        void destroy_nodes(map_pointer start, map_pointer finish);
        void fill_initialize(size_type count, const value_type& value);
        void initialize_map(size_type num_elements);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        void push_back_aux(const value_type& value);
        void push_front_aux(const value_type& value);
        void reserve_map_at_back(size_type nodes_to_add = 1);
        void reserve_map_at_front(size_type nodes_to_add = 1);
    };

    template <class T, size_t BufSize>
    deque<T, BufSize>& deque<T, BufSize>::operator= (const deque<T, BufSize>& other) {
        const size_type len = size();
        if (&other != this) {
            if (len >= other.size()) {
                erase(rayn::copy(other.begin(), other.end(), this->_start), this->_finish);
            } else {
                const_iterator mid = other.begin() + difference_type(len);
                rayn::copy(other.begin(), mid, this->_start);
                insert(this->_finish, mid, other.end());
            }
        }
        return *this;
    }

    template <class T, size_t BufSize>
    void deque<T, BufSize>::assign(size_type count, const value_type& value);

    template <class T, size_t BufSize>
    template <class InputIterator>
    void deque<T, BufSize>::assign(InputIterator first, InputIterator last);

    template <class T, size_t BufSize>
    void deque<T, BufSize>::push_back(const value_type& value) {
        if (_finish.cur != _finish.last - 1) {
            rayn::construct(_finish.cur, value);
            ++_finish.cur;
        } else {
            push_back_aux(value);
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::push_front(const value_type& value) {
        if (_start.cur != _start.first) {
            rayn::construct(_start.cur - 1, value);
            --_start.cur;
        } else {
            push_front_aux(value);
        }
    }

    // Helper functions
    template <class T, size_t BufSize>
    void deque<T, BufSize>::create_nodes(map_pointer start, map_pointer finish) {
        map_pointer cur = start;
        try {
            for (; cur != finish; ++cur) {
                *cur = data_allocator::allocate(buffer_size());
            }
        } catch (...) {
            destroy_nodes(start, cur);
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::destroy_nodes(map_pointer start, map_pointer finish) {
        for (map_pointer cur = start; cur != finish; ++cur) {
            data_allocator::deallocate(*cur, buffer_size());
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::fill_initialize(size_type count, const value_type& value) {
        initialize_map(count);
        map_pointer cur = _start.node;
        try {
            for (_start.node; cur != _finish.node; ++cur) {
                rayn::uninitialized_fill(*cur, *cur + buffer_size(), value);
            }
            uninitialized_fill(_finish.first + _finish.cur, value);
        } catch (...) {
            for (map_pointer mp = _start.node; mp != cur; ++mp) {
                data_allocator::destroy(*mp, *mp + buffer_size());
            }
            //TODO 留待改善
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::initialize_map(size_type num_elements) {
        size_type num_nodes = num_elements / buffer_size() + 1;
        // 最多是所需节点数 + 2， 前后各预留一个
        this->map_size = max(size_type(__initial_map_size), num_nodes + 2);
        this->map = map_allocator::allocate(map_size);

        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes;
        try {
            create_nodes(nstart, nfinish);
        } catch (...) {
            map_allocator::deallocate(this->map, this->map_size);
            this->map = 0;
            this->map_size = 0;
        }
        _start.set_node(nstart);
        _finish.set_node(nfinish - 1);
        _start.cur = _start.first;
        _finish.cur = _finish.first + num_elements % buffer_size();
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front);
    template <class T, size_t BufSize>
    void deque<T, BufSize>::push_back_aux(const value_type& value) {
        value_type v_copy = value;
        reserve_map_at_back();
        *(_finish.node + 1) = data_allocator::allocate(buffer_size());
        try {
            rayn::construct(_finish.cur, v_copy);
            _finish.set_node(_finish.node + 1);
            _finish.cur = _finish.first;
        } catch (...) {
            _finish.set_node(_finish.node - 1);
            _finish.cur = _finish.last - 1;
            data_allocator::deallocate(*(_finish.node + 1), buffer_size);
            throw;
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::push_front_aux(const value_type& value) {
        value_type v_copy = value;
        reserve_map_at_front();
        *(_start.node - 1) = data_allocator::allocate(buffer_size());
        try {
            _start.set_node(_start.node - 1);
            _start.cur = _start.last - 1;
            rayn::construct(_start.cur, v_copy);
        } catch (...) {
            _start.set_node(_start.node + 1);
            _start.cur = _start.first;
            data_allocator::deallocate(*(_start.node - 1), buffer_size());
            throw;
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::reserve_map_at_back(size_type nodes_to_add = 1) {
        if (nodes_to_add + 1 > map_size - (_finish.node - map)) {
            reallocate_map(nodes_to_add, false);
        }
    }
    template <class T, size_t BufSize>
    void deque<T, BufSize>::reserve_map_at_front(size_type nodes_to_add = 1) {
        if (nodes_to_add > _start.node - map) {
            reallocate_map(nodes_to_add, true);
        }
    }

    // Global functions
    template <class T>
    inline bool operator== (const deque<T>& lhs, const deque<T>& rhs);
    template <class T>
    inline bool operator!= (const deque<T>& lhs, const deque<T>& rhs);
    template <class T>
    inline bool operator< (const deque<T>& lhs, const deque<T>& rhs);
    template <class T>
    inline bool operator<= (const deque<T>& lhs, const deque<T>& rhs);
    template <class T>
    inline bool operator> (const deque<T>& lhs, const deque<T>& rhs);
    template <class T>
    inline bool operator>= (const deque<T>& lhs, const deque<T>& rhs);

    template <class T>
    inline void swap(const deque<T>& lhs, const deque<T>& rhs);
}

#endif
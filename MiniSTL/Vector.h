#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>
#include <type_traits>

#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Uninitialized.h"

namespace rayn {
    template <class T, class Alloc = allocator<T>>
    class vector {
    public:
        typedef T                           value_type;
        typedef T*                          iterator;
        typedef const T*                    const_iterator;
        typedef reverse_iterator<T*>        reverse_iterator;
        typedef reverse_iterator<const T*>  const_reverse_iterator;
        typedef T*                          pointer;
        typedef const T*                    const_pointer;
        typedef T&                          reference;
        typedef const T&                    const_reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

    protected:
        typedef Alloc data_allocator;

        T *_start;
        T *_finish;
        T *_endOfStorage;

    public:
        // The constructor
        vector() : _start(0), _finish(0), _endOfStorage(0) {}
        explicit vector(const size_type n);
        vector(const size_type n, const value_type& value);
        template <class InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(const vector& v);
        vector(vector&& v);
        vector& operator = (const vector& v);
        vector& operator = (vector&& v);
        ~vector() { }

        //The compare function
        bool operator == (const vector& v) const;
        bool operator != (const vector& v) const;

        //The functions about iterator.
        iterator begin() { return _start; }
        const_iterator begin() const { return _start; }
        const_iterator cbegin() const { return _start; }
        iterator end() { return _finish; }
        const_iterator end() const { return _finish; }
        const_iterator cend() const { return _finish; }
        reverse_iterator rbegin() { return reverse_iterator(_finish); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(_finish); }
        reverse_iterator rend() { return reverse_iterator(_start); }
        const_reverse_iterator crend() const { return const_reverse_iterator(_start); }

        //Size function
        difference_type size() const { return _finish - _start; }
        difference_type capacity() const { return _endOfStorage - _start; }
        bool empty() const { return _start == _finish; }
        void resize(size_type n, value_type val = value_type());
        void reserve(size_type n);
        void shrink_to_fit() {
            data_allocator::deallocate(_finish, _endOfStorage - _finish);
            _endOfStorage = _finish;
        }

        // Access function
        value_type& operator[](const difference_type index) { return *(begin() + i); }
        value_type& front() { return *(begin()); }
        value_type& back() { return *(end() - 1); }

        // return address of first element
        pointer data() { return this->_start; }
        const_pointer data() const { return this->_start; }

        /*
        ** @brief Clear the vector. 
        ** destory all the object in the vector.
        ** Resize to 0, but not recycle the memory.
        */
        void clear() {
            data_allocator::destory(_start, _finish);
            _finish = _start;
        }
        /*
        ** @brief swap 2 vector.
        */
        void swap(vector& v) {
            //TODO after implementing the <Algorithm.h>
        }
        void push_back(const value_type& value);
        void pop_back();
        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, const size_type&n, const value_type& val);
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);

    };
}

#endif
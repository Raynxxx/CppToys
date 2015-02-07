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
        // The defalut constructor
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

        bool operator == (const vector& v) const;
        bool operator != (const vector& v) const;

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
    };
}

#endif
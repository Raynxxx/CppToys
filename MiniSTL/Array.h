/*
** Array.h
** Created by Rayn on 2016/02/08
*/
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "TypeTraits.h"
#include "ReverseIterator.h"
#include "AlgoBase.h"

namespace rayn {

    template <typename T, size_t Size>
    struct __array_traits {
        typedef T   Type[Size];

        static T&
        _s_ref(const Type& arr, size_t n) {
            return const_cast<T&>(arr[n]);
        }

        static T*
        _s_ptr(const Type& arr) {
            return const_cast<T*>(arr);
        }
    };

    template <typename T>
    struct __array_traits<T, 0> {
        struct Type   {};

        static T& 
        _s_ref(const Type& arr, size_t n) {
            return *static_cast<T*>(nullptr);
        }

        static T*
        _s_ptr(const Type& arr) {
            return nullptr;
        }
    };

    template <typename T, size_t Size>
    struct array {
        typedef T                                   value_type;
        typedef value_type*                         pointer;
        typedef const value_type*                   const_pointer;
        typedef value_type&                         reference;
        typedef const value_type&                   const_reference;
        typedef value_type*                         iterator;
        typedef const value_type*                   const_iterator;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;
        typedef reverse_iterator_t<iterator>        reverse_iterator;
        typedef reverse_iterator_t<const_iterator>  const_reverse_iterator;

        // Support for zero-sized arrays mandatory.
        typedef __array_traits<T, Size>     _AT_type;
        typename _AT_type::Type             _elems;

        // Iterators
        iterator                begin()         { return iterator(data()); }
        const_iterator          begin() const   { return const_iterator(data()); }
        iterator                end()           { return iterator(data() + Size); }
        const_iterator          end() const     { return const_iterator(data() + Size); }
        reverse_iterator        rbegin()        { return reverse_iterator(end()); }
        const_reverse_iterator  rbegin() const  { return const_reverse_iterator(end()); }
        reverse_iterator        rend()          { return reverse_iterator(begin()); }
        const_reverse_iterator  rend() const    { return const_reverse_iterator(begin()); }
        const_iterator          cbegin() const  { return const_iterator(data()); }
        const_iterator          cend() const    { return const_iterator(data() + Size); }
        const_reverse_iterator  crbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator  crend() const   { return const_reverse_iterator(begin()); }

        // Capacity
        size_type   size()      { return Size; }
        size_type   max_size()  { return Size; }
        bool        empty()     { return Size() == 0; }

        // Element access
        reference operator[] (size_type n) {
            return _AT_type::_s_ref(_elems, n);
        }
        const_reference operator[] (size_type n) const {
            return _AT_type::_s_ref(_elems, n);
        }

        reference at(size_type n) {
            return _AT_type::_s_ref(_elems, n);
        }
        const_reference at(size_type n) const {
            return _AT_type::_s_ref(_elems, n);
        }

        reference front() {
            return *begin();
        }
        const_reference front() const {
            return _AT_type::_s_ref(_elems, 0);
        }

        reference back() {
            return Size ? *(end() - 1) : *end();
        }
        const_reference back() const {
            return Size ? _AT_type::_s_ref(_elems, Size - 1)
                        : _AT_type::_s_ref(_elems, 0);
        }

        pointer data() {
            return _AT_type::_s_ptr(_elems);
        }
        const_pointer data() const {
            return _AT_type::_s_ptr(_elems);
        }

        // Modifiers
        void fill(const value_type& u) {
            rayn::fill_n(begin(), size(), u);
        }
        void swap(array& other) {
            rayn::swap_ranges(begin(), end(), other.begin());
        }
    };

    // Array comparisons.
    template <typename T, size_t Size>
    inline bool operator==(const array<T, Size>& one, const array<T, Size>& two) {
        return rayn::equal(one.begin(), one.end(), two.begin());
    }
    template <typename T, size_t Size>
    inline bool operator!=(const array<T, Size>& one, const array<T, Size>& two) {
        return !(one == two);
    }
    template <typename T, size_t Size>
    inline bool operator<(const array<T, Size>& one, const array<T, Size>& two) {
        return rayn::lexicographical_compare(one.begin(), one.end(),
                                             two.begin(), two.end());
    }
    template <typename T, size_t Size>
    inline bool operator>(const array<T, Size>& one, const array<T, Size>& two) {
        return two < one;
    }
    template <typename T, size_t Size>
    inline bool operator<=(const array<T, Size>& one, const array<T, Size>& two) {
        return !(one > two);
    }
    template <typename T, size_t Size>
    inline bool operator>=(const array<T, Size>& one, const array<T, Size>& two) {
        return !(one < two);
    }


    // Specialized algorithms.
    template <typename T, size_t Size>
    inline void swap(array<T, Size>& one, array<T, Size>& two) {
        one.swap(two);
    }

    template <size_t Index, typename T, size_t Size>
    T& get(array<T, Size>& arr) {
        return __array_traits<T, Size>::_s_ref(arr._elems, Index);
    }

    template <size_t Index, typename T, size_t Size>
    T&& get(array<T, Size>&& arr) {
        return rayn::move(get<Index>(arr));
    }
    template <size_t Index, typename T, size_t Size>
    const T& get(const array<T, Size>& arr) {
        return __array_traits<T, Size>::_s_ref(arr._elems, Index);
    }
}

#endif
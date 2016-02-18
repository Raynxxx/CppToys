/*
** Allocator.h
** Created by Rayn on 2015/01/08
** 空间配置器，以变量数目为单位分配
*/
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "Alloc.h"
#include "Construct.h"

#include <new>      // for placement new
#include <cassert>

namespace rayn {

    template <class T>
    class allocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

    public:
        static T *allocate() {
            return static_cast<T *>(alloc::allocate(sizeof(T)));
        }
        static T *allocate(size_t n) {
            if (n == 0) return 0;
            return static_cast<T *>(alloc::allocate(sizeof(T) * n));
        }
        static void deallocate(T *ptr) {
            alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
        }
        static void deallocate(T *ptr, size_t n) {
            if (n == 0) return;
            alloc::deallocate(static_cast<void *>(ptr), sizeof(T) * n);
        }

        static void construct(T *ptr) {
            new(ptr) T();
        }

        static void construct(T *ptr, const T& value) {
            new(ptr)T(value);
        }
        static void destroy(T *ptr) {
            ptr->~T();
        }
        static void destroy(T *first, T *last) {
            for (; first != last; ++first) {
                first->~T();
            }
        }
    };

}

#endif
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
        typedef T                               value_type;
        typedef T*                              iterator;
        typedef const T*                        const_iterator;
        typedef reverse_iterator_t<T*>          reverse_iterator;
        typedef reverse_iterator_t<const T*>    const_reverse_iterator;
        typedef T*                              pointer;
        typedef const T*                        const_pointer;
        typedef T&                              reference;
        typedef const T&                        const_reference;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;

    private:
        typedef Alloc data_allocator;

        T *_start;
        T *_finish;
        T *_endOfStorage;

    public:
        // The Default constructor
        vector() : _start(0), _finish(0), _endOfStorage(0) {}

        //explicit construct a vector, size n.
        explicit vector(const size_type n) {
            allocateAndFillN(n, value_type());
        }
        vector(const size_type n, const value_type& value) {
            allocateAndFillN(n, value);
        }
        //construct vector from [first, last)
        template <class InputIterator>
        vector(InputIterator first, InputIterator last) {
            //判断是迭代器还是数字, 重载vector_aux
            vector_aux(first, last, typename std::is_integral<InputIterator>::type());
        }
        //The Copy Constructor
        vector(const vector& v) {
            allocateAndCopy(v.begin(), v.end());
        }
        //The Move Constructor
        vector(vector&& v) {
            _start = v._start;
            _finish = v._finish;
            _endOfStorage = v._endOfStorage;
            v._start = v._finish = v._endOfStorage = 0;
        }
        //Vector Assignment operator
        vector& operator = (const vector& v) {
            if (this != &v) {
                allocateAndCopy(v.begin(), v.end());
            }
            return *this;
        }
        //Vector Move Assignment operator
        vector& operator = (vector&& v) {
            if (this != &v) {
                //first to destory cur vector
                destoryAndDeallocateAll();
                _start = v._start;
                _finish = v._finish;
                _endOfStorage = v._endOfStorage;
                v._start = v._finish = v._endOfStorage = 0;
            }
            return *this;
        }
        ~vector() {
            destoryAndDeallocateAll();
        }

        //The compare function
        bool operator == (const vector& v) const {
            if (size() != v.size()) {
                return false;
            } else {
                auto it1 = begin();
                auto it2 = v.begin();
                for (; it1 != end() && it2 != v.end(); ++it1, ++it2) {
                    if (*it1 != *it2) {
                        return false;
                    }
                }
                return true;
            }
        }
        bool operator != (const vector& v) const {
            return !(*this == v);
        }

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

        /*
        ** @brief Resize the vector to the specified number of elements.
        ** 调整容器大小，使其能够容纳 count 个元素。
        ** 如果当前大小小于 count，则会追加额外的元素，并将它们初始化为 value。
        ** 如果当前大小大于 count，则容器会缩小到仅包含前 count 个元素。
        */
        void resize(size_type n, value_type val = value_type()) {
            if (n < size()) {
                data_allocator::destory(_start + n, _finish);
                _finish = _start + n;
            } else if (size() < n && n <= capacity()) {
                auto lengthOfAdd = n - size();
                _finish = rayn::uninitialized_fill_n(_start, lengthOfAdd, val);
            } else if (capacity() < n) {
                auto lengthOfAdd = n - size();
                T *newStart = data_allocator::allocate(getNewCapacity(lengthOfAdd));
                T* newFinish = rayn::uninitialized_copy(begin(), end(), newStart);
                newFinish = rayn::uninitialized_fill_n(newFinish, lengthOfAdd, val);
                //first to destory cur vector
                destoryAndDeallocateAll();
                _start = newStart;
                _finish = newFinish;
                _endOfStorage = _start + n;
            }
        }

        /*
        ** @brief Attempt to preallocate enough memory for @c n elements.
        ** @param n Number of elements required.
        */
        void reserve(size_type n) {
            if (n <= capacity()) return;
            T *newStart = data_allocator::allocate(n);
            T *newFinish = rayn::uninitialized_copy(begin(), end(), newStart);
            //first to destory cur vector
            destoryAndDeallocateAll();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = _start + n;
        }
        void shrink_to_fit() {
            data_allocator::deallocate(_finish, _endOfStorage - _finish);
            _endOfStorage = _finish;
        }

        // Elements Access function
        /*
        ** @brief Access to the data contained in the %vector at the @c index
        ** @param index The index of the element to access.
        ** @return  A Read & Write(Non-const) reference to data.
        ** This operator allows for easy, array-style, data access.
        */
        reference operator[](size_type index) {
            return *(begin() + index);
        }
        const_reference operator[](size_type index) const {
            return *(begin() + index);
        }

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
        ** @brief swap data with vector @c v.
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

        Alloc get_allocator() {
            return data_allocator;
        }

    private:
        /*
        ** @brief Destory exist object, deallocate all memory.
        */
        void destoryAndDeallocateAll() {
            if (capacity() != 0) {
                data_allocator::destory(_start, _finish);
                data_allocator::deallocate(_start, capacity());
            }
        }
        /*
        ** @brief allocate memory that is fit for n object.
        ** @param n The number of object.
        ** @param value The value will be fill into vector.
        */
        void allocateAndFillN(const size_type n, const value_type& value) {
            _start = data_allocator::allocate(n);
            rayn::uninitialized_fill_n(_start, n, value);
            _finish = _endOfStorage = _start + n;
        }
        /*
        ** @brief Copy the Data in [first, last) into the allocated memory.
        */
        template <class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last) {
            _start = data_allocator::allocate(last - first);
            _finish = rayn::uninitialized_copy(first, last, _start);
            _endOfStorage = _finish;
        }

        template <class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type) {
            allocateAndCopy(first, last);
        }
        template <class Integer>
        void vector_aux(Integer n, const value_type& value, std::true_type) {
            allocateAndFillN(n, value);
        }
        size_type getNewCapacity(size_type len) {
            size_type oldCapacity = capacity();
            //TODO after <Algorithm.h>
        }
    };
}

#endif
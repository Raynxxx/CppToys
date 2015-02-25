#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>
#include <type_traits>

#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Uninitialized.h"
#include "Algorithm.h"

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
        // The Default Constructor
        vector() : _start(0), _finish(0), _endOfStorage(0) {}
        // explicit Construct vector as Size n.
        explicit vector(const size_type n) {
            allocateAndFillN(n, value_type());
        }
        // Construct vector as multiple elements.
        vector(const size_type n, const value_type& value) {
            allocateAndFillN(n, value);
        }
        // Construct vector from range [first, last)
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
        // Destroy the vector instance.
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
        /*
        ** Returns a read/write reference to the data at the first element of vector
        */
        reference front() { return *(begin()); }
        /*
        ** Returns a read/write reference to the data at the last element of vector
        */
        reference back() { return *(end() - 1); }

        // return address of member
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
            if (this != &v) {
                rayn::swap(_start, v._start);
                rayn::swap(_finish, v._finish);
                rayn::swap(_endOfStorage, v._endOfStorage);
            }
        }
        /*
        ** @brief Add data to the end of vector.
        */
        void push_back(const value_type& value) {
            if (_finish != _endOfStorage) {
                construct(_finish, value);
                ++_finish;
            } else {
                insert(end(), value);
            }
        }
        /*
        ** @brief Erase the last data of vector
        */
        void pop_back() {
            --_finish;
            destroy(_finish);
        }

        /*
        ** @brief Insert @c val at the @c position of vector.
        ** @return The iterator of inserted element.
        */
        iterator insert(iterator position, const value_type& val) {
            difference_type index = position - begin();
            insert(position, 1, val);
            return begin() + index;
        }
        /*
        ** @brief Insert @c val at the range [position, position + n).
        */
        void insert(iterator position, const size_type&n, const value_type& val) {
            insert_aux(position, n, val, typename std::is_integral<size_type>::type());
        }
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last) {
            insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
        }

        /*
        ** @brief Remove element at the given position.
        ** @param position Iterator pointing to element to be erased.
        ** @return An iterator pointing to the next element (or end()).
        */
        iterator erase(iterator position) {
            return erase(position, position + 1);
        }
        /*
        ** @brief Remove a range of elements [first, last).
        ** @param first Iterator pointing to the first element to be erased.
        ** @param last  Iterator pointing to one past the last element to be erased.
        ** @return  An iterator pointing to the element pointed to by @a last prior to erasing (or end()).
        */
        iterator erase(iterator first, iterator last) {
            // 尾部余留的元素
            difference_type lengthOfTail = end() - last;
            difference_type lenthOfRemove = last - first;
            _finish = _finish - lengthOfTail;
            for (; lengthOfTail != 0; --lengthOfTail) {
                iterator temp = last - lenthOfRemove;
                *temp = *last;
                ++last;
            }
            return first;
        }

        //Allocator Function of Container
        Alloc get_allocator() {
            return data_allocator;
        }
    private:
        /*
        ** @brief Destory exist object, deallocate all memory.
        */
        void destoryAndDeallocateAll() {
            if (capacity() != 0) {
                data_allocator::destroy(_start, _finish);
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

        /*
        ** @brief Reallocate memory and Insert(Copy) [first, last) into [position, last - last).
        */
        template <class InputIterator>
        void reallocateAndCopy(iterator position, InputIterator first, InputIterator last) {
            difference_type newCapacity = getNewCapacity(last - first);
            T* newStart = data_allocator::allocate(newCapacity);
            T* newEndOfStorage = newStart + newCapacity;
            T* newFinish = rayn::uninitialized_copy(begin(), position, newStart);
            newFinish = rayn::uninitialized_copy(first, last, newFinish);
            newFinish = rayn::uninitialized_copy(position, end(), newFinish);
            // First to destroy cur Vector.
            destoryAndDeallocateAll();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = newEndOfStorage;
        }
        /*
        ** @brief Reallocate memory and Insert(Copy) n val into [position, position + n).
        */
        void reallocateAndFillN(iterator position, const size_type& n, const value_type& value) {
            difference_type newCapacity = getNewCapacity(n);
            T* newStart = data_allocator::allocate(newCapacity);
            T* newEndOfStorage = newStart + newCapacity;
            T* newFinish = rayn::uninitialized_copy(begin(), position, newStart);
            newFinish = rayn::uninitialized_fill_n(newFinish, n, value);
            newFinish = rayn::uninitialized_copy(position, end(), newFinish);
            // First to destroy cur Vector.
            destoryAndDeallocateAll();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = newEndOfStorage;
        }

        //Public Function的辅助函数
        template <class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type) {
            allocateAndCopy(first, last);
        }
        template <class Integer>
        void vector_aux(Integer n, const value_type& value, std::true_type) {
            allocateAndFillN(n, value);
        }
        template <class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
            // the size of left storage
            difference_type storageLeft = _endOfStorage - _finish;
            difference_type rangeNeed = last - first;
            if (storageLeft >= rangeNeed) {
                iterator lastPtr = end() - 1;
                // Move the range [position, _finish) back
                for (; lastPtr - position >= 0; --lastPtr) {
                    *(lastPtr + rangeNeed) = *lastPtr;
                }
                rayn::uninitialized_copy(first, last, position);
                _finish += rangeNeed;
            } else {
                reallocateAndCopy(position, first, last);
            }
        }
        template <class Integer>
        void insert_aux(iterator position, Integer n, const value_type& value, std::true_type) {
            assert(n != 0);
            // the size of left storage
            difference_type storageLeft = _endOfStorage - _finish;
            difference_type rangeNeed = n;
            if (storageLeft >= rangeNeed) {
                iterator lastPtr = end() - 1;
                // Move the range [position, _finish) back
                for (; lastPtr - position >= 0; --lastPtr) {
                    *(lastPtr + rangeNeed) = *lastPtr;
                }
                rayn::uninitialized_fill_n(position, n, value);
                _finish += rangeNeed;
            } else {
                reallocateAndFillN(position, n, value);
            }
        }

        /*
        ** @brief   如果原大小为0，则配置为len, 否则配置为 旧大小 * 2 or 旧大小 + 增加长度
        ** @param   len (default = 1)  
        */
        size_type getNewCapacity(size_type len = 1) {
            size_type oldCapacity = capacity();
            size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + max(oldCapacity, len)) : len);
            return newCapacity;
        }

    public:
        // 全局重载运算符
        template <class T, class Alloc>
        friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
            return v1.operator==(v2);
        }
        template <class T, class Alloc>
        friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
            return !(v1 == v2);
        }
    };
}

#endif
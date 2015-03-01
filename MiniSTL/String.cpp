/*
** String.cpp
** Created by Rayn on 2015/02/25
** basic_string 具体实现
*/
#include "String.h"

namespace rayn {
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator= (const basic_string& str) {
        if (this != &str) {
            destroy_and_deallocate();
            allocate_and_copy(str._start, str._finish);
        }
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator= (basic_string&& str) {
        if (this != &str) {
            moveData(str);
        }
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator= (const CharT* cstr) {
        destroy_and_deallocate();
        allocate_and_copy(cstr, cstr + strlen(cstr));
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator= (CharT ch) {
        destroy_and_deallocate();
        allocate_and_fill(1, ch);
        return *this;
    }

    template <class CharT>
    void basic_string<CharT>::resize(size_type n) {
        resize(n, CharT());
    }
    template <class CharT>
    void basic_string<CharT>::resize(size_type n, value_type ch) {
        if (n < size()) {
            rayn::destroy(_start + n, _finish);
            _finish = _start + n;
        } else if (n > size() && n <= capacity()) {
            size_type lengthOfAdd = n - size();
            _finish = rayn::uninitialized_fill_n(_finish, lengthOfAdd, ch);
        } else if (n >capacity()) {
            size_type lengthOfAdd = n - size();
            iterator newStart = data_allocator::allocate(getNewCapacity(lengthOfAdd));
            iterator newFinish = rayn::uninitialized_copy(begin(), end(), newStart);
            newFinish = rayn::uninitialized_fill_n(newFinish, lengthOfAdd, ch);

            destroy_and_deallocate();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = _start + n;
        }
    }


}
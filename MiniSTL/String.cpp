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
    void basic_string<CharT>::resize(size_type n, CharT ch) {
        if (n < size()) {
            rayn::destroy(_start + n, _finish);
            _finish = _start + n;
        } else if (n > size() && n <= capacity()) {
            size_type lengthOfAdd = n - size();
            _finish = rayn::uninitialized_fill_n(_finish, lengthOfAdd, ch);
        } else if (n > capacity()) {
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

    template <class CharT>
    void basic_string<CharT>::reserve(size_type n = 0) {
        if (n <= capacity()) return;
        iterator newStart = data_allocator::allocate(n);
        iterator newFinish = rayn::uninitialized_copy(begin(), end(), newStart);
        destroy_and_deallocate();
        _start = newStart;
        _finish = newFinish;
        _endOfStorage = _start + n;
    }

    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator+= (const basic_string& str) {
        insert(size(), str);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator+= (const CharT* cstr) {
        insert(size(), cstr);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::operator+= (CharT ch) {
        insert(end(), ch);
        return *this;
    }

    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::append(const basic_string& str) {
        insert(size(), str);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::append(const basic_string& str, size_type subpos,
        size_type sublen = npos) {
        sublen = fix_npos(sublen, str.length(), subpos);
        insert(size(), str, subpos, sublen);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::append(const CharT* cstr) {
        insert(size(), cstr);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::append(const CharT* cstr, size_type n) {
        insert(size(), cstr, n);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::append(size_type n, CharT ch) {
        insert(end(), n, ch);
        return *this;
    }
    template <class CharT>
    template <class InputIterator>
    basic_string<CharT>& basic_string<CharT>::append(InputIterator first, InputIterator last) {
        insert(end(), first, last);
        return *this;
    }

    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::assign(const basic_string& str) {
        if (this != &str) {
            destroy_and_deallocate();
            allocate_and_copy(str.begin(), str.end());
        }
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::assign(basic_string&& str) {
        if (this != &str) {
            moveData(str);
        }
        return *this;
    }

    template <class CharT>
    typename basic_string<CharT>::iterator
        basic_string<CharT>::insert(iterator p, size_type n, CharT ch) {
        size_type lengthOfLeft = capacity() - size();
        if (n <= lengthOfLeft) {
            for (iterator it = _finish - 1; it >= p; --it) {
                *(it + n) = *it;
            }
            rayn::uninitialized_fill_n(p, n, ch);
            _finish += n;
            return p + n;
        } else {
            return insert_and_fill(p, n, ch);
        }
    }
    template <class CharT>
    typename basic_string<CharT>::iterator
        basic_string<CharT>::insert(iterator p, CharT ch) {
        return insert(p, 1, ch);
    }
    template <class CharT>
    template <class InputIterator>
    typename basic_string<CharT>::iterator
        basic_string<CharT>::insert(iterator p, InputIterator first, InputIterator last) {
        size_type lengthOfLeft = capacity() - size();
        size_type range = last - first;
        if (range <= lengthOfLeft) {
            for (iterator it = _finish - 1; it >= p; --it) {
                *(it + range) = *it;
            }
            rayn::uninitialized_copy(first, last, p);
            _finish += range;
            return p + range;
        } else {
            return insert_and_copy(p, first, last);
        }
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::insert(size_type pos, const basic_string& str) {
        insert(begin() + pos, str.begin(), str.end());
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::insert(size_type pos, const basic_string& str,
        size_type subpos, size_type sublen = npos) {
        sublen = fix_npos(sublen, str.length(), subpos);
        insert(_start + pos, str.begin() + subpos, str.begin() + subpos + sublen);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::insert(size_type pos, const CharT* cstr) {
        insert(begin() + pos, cstr, cstr + strlen(cstr));
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::insert(size_type pos, const CharT* cstr, size_type n) {
        insert(begin() + pos, cstr, cstr + n);
        return *this;
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::insert(size_type pos, size_type n, CharT ch) {
        insert(begin() + pos, n, ch);
        return *this;
    }

    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::erase(size_type pos = 0, size_type n = npos) {
        n = fix_npos(n, this->size(), pos);
        erase(begin() + pos, begin() + pos + n);
        return *this;
    }
    template <class CharT>
    typename basic_string<CharT>::iterator
        basic_string<CharT>::erase(iterator pos) {
        return erase(pos, pos + 1);
    }
    template <class CharT>
    typename basic_string<CharT>::iterator
        basic_string<CharT>::erase(iterator first, iterator last) {
        size_type lengthOfTail = _finish - last;
        for (size_type off = 0; off != lengthOfTail; ++off) {
            *(first + off) = *(last + off);
        }
        rayn::destroy(first + lengthOfTail, _finish);
        _finish = first + lengthOfTail;
        return first;
    }

    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(size_type pos, size_type len, 
        const basic_string& str) {
        return replace(begin() + pos, begin() + pos + len, str.begin(), str.end());
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(size_type pos, size_type len,
        const basic_string& str, size_type subpos, size_type sublen = npos) {
        sublen = fix_npos(sublen, str.length(), subpos);
        return replace(begin() + pos, begin() + pos + len, str.begin() + subpos,
            str.begin() + subpos + sublen);
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(size_type pos, size_type len,
        const CharT* cstr, size_type len2) {
        return replace(begin() + pos, begin() + pos + len, cstr, cstr + len2);
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(size_type pos, size_type len,
        const CharT* cstr) {
        return replace(begin() + pos, begin() + pos + len, cstr, cstr + strlen(cstr));
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(size_type pos, size_type len,
        size_type n, CharT ch) {
        return replace(begin() + pos, begin() + pos + len, n, ch);
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(iterator i1, iterator i2,
        const basic_string& str) {
        return replace(i1, i2, str.begin(), str.end());
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(iterator i1, iterator i2,
        const CharT* cstr, size_type n) {
        return replace(i1, i2, cstr, cstr + n);
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(iterator i1, iterator i2,
        const CharT* cstr) {
        return replace(i1, i2, cstr, cstr + strlen(cstr));
    }
    template <class CharT>
    basic_string<CharT>& basic_string<CharT>::replace(iterator i1, iterator i2,
        size_type n, CharT ch) {
        iterator cur = erase(i1, i2);
        insert(cur, n, c);
        return *this;
    }
    template <class CharT>
    template <class InputIterator>
    basic_string<CharT>& basic_string<CharT>::replace(iterator i1, iterator i2,
        InputIterator first, InputIterator last) {
        iterator cur = erase(i1, i2);
        insert(cur, first, last);
        return *this;
    }
}
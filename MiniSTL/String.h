#ifndef _STRING_H_
#define _STRING_H_

#include <cstring>
#include <type_traits>

#include "Allocator.h"
#include "Uninitialized.h"
#include "ReverseIterator.h"

namespace rayn {
    template <class CharT>
    class basic_string {
    public:
        typedef CharT                               value_type;
        typedef CharT*                              iterator;
        typedef const CharT*                        const_iterator;
        typedef reverse_iterator_t<CharT*>          reverse_iterator;
        typedef reverse_iterator_t<const CharT*>    const_reverse_iterator;
        typedef CharT&                              reference;
        typedef const CharT&                        const_reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

        // Value returned by various member functions when they Fail.
        static const size_type npos = static_cast<size_type>(-1);
    private:
        CharT *_start;
        CharT *_finish;
        CharT *_endOfStorage;

        typedef allocator<value_type> data_allocator;

    public:
        // The Default Constructor
        basic_string() : _start(0), _finish(0), _endOfStorage(0) {}
        // The Copy Constructor
        basic_string(const basic_string& str);
        // The Move Constructor
        basic_string(basic_string&& str);
        // Construct string as copy of a substring.
        basic_string(const basic_string& str, size_type pos, size_type len = npos);
        // Construct string as copy of a C-Style string.
        basic_string(const CharT* cstr);
        // Construct string initialized by a Char Array.
        basic_string(const CharT* cstr, size_type n);
        // Construct string as multiple characters.
        basic_string(size_type n, CharT ch);
        // Construct string as copy of a range [first, last).
        template <class InputIterator>
        basic_string(InputIterator first, InputIterator last);

        // Copy Assign str to this string.
        basic_string& operator= (const basic_string& str);
        // Move Assign str to this string.
        basic_string& operator= (basic_string&& str);
        // Copy cstr into this string.
        basic_string& operator= (const CharT* cstr);
        // Set value to string of length 1.
        basic_string& operator= (CharT ch);

        // Destroy the string instance.
        ~basic_string();

        // The Iterator Functions
        iterator begin() { return _start; }
        const_iterator begin() const { return _start; }
        const_iterator cbegin() const { return _start; }
        iterator end() { return _finish; }
        const_iterator end() const { return _finish; }
        const_iterator cend() const { return _finish; }
        reverse_iterator rbegin() { return reverse_iterator(_finish); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(_finish); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(_finish); }
        reverse_iterator rend() { return reverse_iterator(_start); }
        const_reverse_iterator rend() const { return const_reverse_iterator(_start); }
        const_reverse_iterator crend() const { return const_reverse_iterator(_start); }

        // Capacity
        /*
        ** @brief   Returns the number of characters in the string
        */
        size_type size() const { return _finish - _start; }
        size_type length() const { return _finish - _start; }

        size_type capacity() const { return _endOfStorage - _start; }

        void clear() {
            destroy(_start, _finish);
            _start = _finish;
        }
        bool empty() const { return begin() == end(); }
        void resize(size_type n);
        void resize(size_type n, CharT ch);
        void reserve(size_type n = 0);
        void shrink_to_fit() {
            data_allocator::deallocate(_finish, _endOfStorage - _finish);
            _endOfStorage = _finish;
        }

        // Element access
        reference operator[] (size_type pos) { return *(_start + pos); }
        const_reference operator[] (size_type pos) const { return *(_start + pos); }
        reference front() { return *(_start); }
        const_reference front() const { return *(_start); }
        reference back() { return *(_finish - 1); }
        const_reference back() const { return *(_finish - 1); }

        // Modifiers
        /*
        ** @brief   Append a string to this string.
        ** @return  Reference to this string.
        */
        basic_string& operator+=(const basic_string& str);
        /*
        ** @brief   Append a C-Style string to this string.
        ** @return  Reference to this string.
        */
        basic_string& operator+=(const CharT* cstr);
        /*
        ** @brief   Append a Character.
        ** @return  Reference to this string.
        */
        basic_string& operator+=(CharT ch);

        /*
        ** @brief   Append a string to this string.
        ** @return  Reference to this string.
        */
        basic_string& append(const basic_string& str);
        /*
        ** @brief   Append n char from str starting at pos to this string.
        ** @param   subpos  Index of the first character of str to append.
        ** @param   sublen  The number of characters to append.
        ** @return  Reference to this string.
        */
        basic_string& append(const basic_string& str, size_type subpos, size_type sublen = npos);
        /*
        ** @brief   Append a C-Style string.
        ** @return  Reference to this string.
        */
        basic_string& append(const CharT* cstr);
        /*
        ** @brief   Append a C-Style substring.
        ** @param   n   The number of characters to append.
        ** @return  Reference to this string.
        */
        basic_string& append(const CharT* cstr, size_type n);
        /*
        ** @brief   Append n Characters to this string.
        ** @return  Reference to this string.
        */
        basic_string& append(size_type n, CharT ch);
        /*
        ** @brief   Append a range [first, last) of characters.
        ** @return  Reference to this string.
        */
        template <class InputIterator>
        basic_string& append(InputIterator first, InputIterator last);
        /*
        ** @brief   Append a single character.
        */
        void push_back(CharT ch);

        /*
        ** @brief   Set (Copy) this string as another string.
        ** @return  Reference to this string.
        */
        basic_string& assign(const basic_string& str);
        /*
        ** @brief   Set (Move) this string as another string.
        ** @return  Reference to this string.
        */
        basic_string& assign(basic_string&& str);

        /*
        ** @brief   Insert multiple characters.
        ** @param   p   Iterator refer to location in string to insert to.
        ** @param   n   Number of characters to insert.
        ** @param   ch  The character to insert.
        ** @return  Iterator refer to the first inserted char.
        */
        iterator insert(iterator p, size_type n, CharT ch);
        /*
        ** @brief   Insert one character.
        ** @param   p   Iterator refer to location in string to insert to.
        ** @param   ch  The character to insert.
        ** @return  Iterator refer to the first inserted char.
        */
        iterator insert(iterator p, CharT ch);
        /*
        ** @brief   Insert a range to into this string.
        ** @param   p       Iterator refer to location in string to insert to.
        ** @param   first   Start of range.
        ** @param   last    End of range.
        ** @return  Iterator refer to the first inserted char.
        */
        template <class InputIterator>
        iterator insert(iterator p, InputIterator first, InputIterator last);

        basic_string& insert(size_type pos, const basic_string& str);
        basic_string& insert(size_type pos, const basic_string& str, size_type subpos, size_type sublen = npos);

    };

    typedef basic_string<char>      string;
    typedef basic_string<wchar_t>   wstring;
}

#endif
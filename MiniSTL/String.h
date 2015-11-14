/*
** String.h
** Created by Rayn on 2015/02/25
** basic_string 定义
*/
#ifndef _STRING_H_
#define _STRING_H_

#include "Allocator.h"
#include "Uninitialized.h"
#include "ReverseIterator.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <type_traits>

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
        static const size_type npos = -1;

    private:
        CharT *_start;
        CharT *_finish;
        CharT *_endOfStorage;

        typedef allocator<CharT> data_allocator;

    public:
        // The Default Constructor
        basic_string() : _start(0), _finish(0), _endOfStorage(0) {}
        // The Copy Constructor
        basic_string(const basic_string& str) {
            allocate_and_copy(str._start, str._finish);
        }
        // The Move Constructor
        basic_string(basic_string&& str) {
            moveData(str);
        }
        // Construct string as copy of a substring.
        basic_string(const basic_string& str, size_type pos, size_type len = npos) {
            len = fix_npos(len, str.length(), pos);
            allocate_and_copy(str._start + pos, str._start + pos + len);
        }
        // Construct string as copy of a C-Style string.
        basic_string(const CharT* cstr) {
            allocate_and_copy(cstr, cstr + strlen(cstr));
        }
        // Construct string initialized by a Char Array.
        basic_string(const CharT* cstr, size_type n) {
            allocate_and_copy(cstr, cstr + n);
        }
        // Construct string as multiple characters.
        basic_string(size_type n, CharT ch) {
            allocate_and_fill(n, ch);
        }
        // Construct string as copy of a range [first, last).
        template <class InputIterator>
        basic_string(InputIterator first, InputIterator last) {
            allocate_and_copy(first, last);
        }

        // Destroy the string instance.
        ~basic_string() {
            destroy_and_deallocate();
        }

        // Copy Assign str to this string.
        basic_string& operator= (const basic_string& str);
        // Move Assign str to this string.
        basic_string& operator= (basic_string&& str);
        // Copy cstr into this string.
        basic_string& operator= (const CharT* cstr);
        // Set value to string of length 1.
        basic_string& operator= (CharT ch);

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
        ** @return  Returns the number of characters in the string
        */
        size_type size() const { return _finish - _start; }
        /*
        ** @return  Returns the length of this string.
        */
        size_type length() const { return _finish - _start; }
        /*
        ** @return  Returns the total number of characters that the cur string can hold
        */
        size_type capacity() const { return _endOfStorage - _start; }

        /*
        ** @brief   Erases the string, Make this string empty.
        */
        void clear() {
            destroy(_start, _finish);
            _start = _finish;
        }
        /*
        ** @return  Returns true if this string is empty.
        */
        bool empty() const { return begin() == end(); }
        /*
        ** @brief   Resizes the string to the specified length.
        */
        void resize(size_type n);
        /*
        ** @brief   Resizes the string to the specified length, fill with ch.
        */
        void resize(size_type n, value_type ch);
        /*
        ** @brief   Attempt to preallocate enough memory for specified size.
        */
        void reserve(size_type n = 0);
        /*
        ** @brief   Shrink memory to fit cur string length.
        */
        void shrink_to_fit() {
            data_allocator::deallocate(_finish, _endOfStorage - _finish);
            _endOfStorage = _finish;
        }

        // Element access
        /*
        ** @brief   Access to the data in the string by index.
        ** @param   pos The index of char to access.
        ** @return  Read/Write reference to the character.
        */
        reference operator[] (size_type pos) { return *(_start + pos); }
        /*
        ** @brief   Access to the data in the string by index.
        ** @param   pos The index of char to access.
        ** @return  Read-only reference to the character.
        */
        const_reference operator[] (size_type pos) const { return *(_start + pos); }
        /*
        ** @brief   Access to the data in the string by index.
        ** @param   pos The index of char to access.
        ** @return  Read/Write reference to the character.
        */
        reference at(size_type pos) { return *(_start + pos); }
        /*
        ** @brief   Access to the data in the string by index.
        ** @param   pos The index of char to access.
        ** @return  Read-only reference to the character.
        */
        const_reference at(size_type pos) const { return *(_start + pos); }

        /*
        ** @return  Read/Write reference to the data at the first.
        */
        reference front() { return *(_start); }
        /*
        ** @return  Read-only reference to the data at the first.
        */
        const_reference front() const { return *(_start); }
        /*
        ** @return  Read/Write reference to the data at the last.
        */
        reference back() { return *(_finish - 1); }
        /*
        ** @return  Read-only reference to the data at the last.
        */
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
        ** @param   subpos  Start Index in the inserted string.
        ** @param   sublen  Length of characters to append.
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
        void push_back(CharT ch) { insert(end(), ch); }

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
        /*
        ** @brief   Insert value of string.
        ** @param   pos The Index refer to pos to insert.
        ** @return  Reference to this string.
        */
        basic_string& insert(size_type pos, const basic_string& str);
        /*
        ** @brief   Insert a substring.
        ** @param   pos     Index in this string to insert.
        ** @param   subpos  Start Index in the inserted string.
        ** @param   sublen  Length of characters to insert.
        ** @return  Reference to this string.
        */
        basic_string& insert(size_type pos, const basic_string& str,
            size_type subpos, size_type sublen = npos);
        /*
        ** @brief   Insert a C-Style string.
        ** @param   pos     Index in this string to insert.
        ** @param   cstr    The C-string to insert.
        ** @return  Reference to this string.
        */
        basic_string& insert(size_type pos, const CharT* cstr);
        /*
        ** @brief   Insert a C-Style substring.
        ** @param   pos     Index in this string to insert.
        ** @param   cstr    The C-string to insert.
        ** @param   cstr    Number of characters to insert.
        ** @return  Reference to this string.
        */
        basic_string& insert(size_type pos, const CharT* cstr, size_type n);
        /*
        ** @brief   Insert multiple characters.
        ** @param   pos     Index in this string to insert.
        ** @param   n       Number of characters to insert.
        ** @param   ch      The value of Character to insert.
        ** @return  Reference to this string.
        */
        basic_string& insert(size_type pos, size_type n, CharT ch);

        /*
        ** @brief   Remove characters.
        ** @param   pos Start Index to remove.
        ** @param   n   Number of char to remove. (DEFAULT = npos).
        ** @return  Return the reference of this string.
        */
        basic_string& erase(size_type pos = 0, size_type n = npos);
        /*
        ** @brief   Remove one character.
        ** @param   pos     Iterator refer to the character to remove. 
        ** @return  Return the iterator refer to same location after removal.
        */
        iterator erase(iterator pos);
        /*
        ** @brief   Remove a range of characters [first, last). 
        ** @return  Return the iterator refer to the end of the range.
        */
        iterator erase(iterator first, iterator last);
        /*
        ** @brief   Remove the last character.
        */
        void pop_back() { this->erase(end() - 1, end()); }

        /*
        ** @brief   Replace characters with value from another string.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   str     String to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, const basic_string& str);
        /*
        ** @brief   Replace characters from another substring.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   str     String to insert.
        ** @param   subpos  Index of first char of str to use.
        ** @param   sublen  Number of char from str to use.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, const basic_string& str,
            size_type subpos, size_type sublen = npos);
        /*
        ** @brief   Replace characters from C-Style string.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   cstr    C-String to insert.
        ** @param   len2    Number of char from cstr to use.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, const CharT* cstr, size_type len2);
        /*
        ** @brief   Replace characters from C-Style string.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   cstr    C-String to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, const CharT* cstr);
        /*
        ** @brief   Replace characters from multiple characters.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   n       Number of char to insert.
        ** @param   ch      Characters to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, size_type n, CharT ch);
        /*
        ** @brief   Replace range of characters with string.
        ** @param   i1  Iterator refer to start of range to replace.
        ** @param   i2  Iterator refer to end of range to replace.
        ** @return  Reference to this string.
        */
        basic_string& replace(iterator i1, iterator i2, const basic_string& str);
        /*
        ** @brief   Replace range of characters with C-Style string.
        ** @param   i1      Iterator refer to start of range to replace.
        ** @param   i2      Iterator refer to end of range to replace.
        ** @param   cstr    C-String value to insert.
        ** @param   n       Number of chars from cstr to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(iterator i1, iterator i2, const CharT* cstr, size_type n);
        /*
        ** @brief   Replace range of characters with C-Style string.
        ** @param   i1      Iterator refer to start of range to replace.
        ** @param   i2      Iterator refer to end of range to replace.
        ** @param   cstr    C-String value to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(iterator i1, iterator i2, const CharT* cstr);
        /*
        ** @brief   Replace range of characters with C-Style string.
        ** @param   i1      Iterator refer to start of range to replace.
        ** @param   i2      Iterator refer to end of range to replace.
        ** @param   n       Number of chars to insert.
        ** @param   ch      Character to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(iterator i1, iterator i2, size_type n, CharT ch);
        /*
        ** @brief   Replace range of characters with range.
        ** @param   i1      Iterator refer to start of range to replace.
        ** @param   i2      Iterator refer to end of range to replace.
        ** @param   first   Iterator refer to start of range to insert.
        ** @param   last    Iterator refer to end of range to insert.
        ** @return  Reference to this string.
        */
        template <class InputIterator>
        basic_string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);

        /*
        ** @brief   Copy substring into C-String.
        ** @param   cstr    C-String to copy into.
        ** @param   len     Number of characters to copy.
        ** @param   pos     Index of first character to copy.
        ** @return  Number of characters actually copied.
        */
        size_type copy(const CharT* cstr, size_type len, size_type pos = 0) const;
        /*
        ** @brief   Swap contents with another string.
        */
        void swap(basic_string& str);
        /*
        ** @brief   Return const pointer to null-terminated contents.
        */
        const CharT* c_str() const { 
            CharT* ret = new CharT[size() + 1];
            rayn::uninitialized_copy(begin(), end(), ret);
            ret[size()] = '\0';
            return ret;
        }
        /*
        ** @brief   Return const pointer to contents.
        */
        const CharT* data() const { return _start; }

        /*
        ** @brief   Find position of a String.
        ** @param   cstr    String to locate.
        ** @param   pos     Index of character to search from. (Default = 0)
        ** @return  Index of start of first occurrence.
        */
        size_type find(const basic_string& str, size_type pos = 0) const;
        /*
        ** @brief   Find position of a C substring.
        ** @param   cstr    C-String to locate.
        ** @param   pos     Index of character to search from.
        ** @param   n       Number of characters from cstr to search for.
        ** @return  Index of start of first occurrence.
        */
        size_type find(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find position of a C-String.
        ** @param   cstr    C-String to locate.
        ** @param   pos     Index of character to search from. (Default = 0)
        ** @return  Index of start of first occurrence.
        */
        size_type find(const CharT* cstr, size_type pos = 0) const;
        /*
        ** @brief   Find position of a Character.
        ** @param   ch      Character to locate.
        ** @param   pos     Index of character to search from. (Default = 0)
        ** @return  Index of start of first occurrence.
        */
        size_type find(CharT ch, size_type pos = 0) const;

        /*
        ** @brief   Find last position of a String.
        ** @param   cstr    String to locate.
        ** @param   pos     Index of character to search back from. (Default = npos)
        ** @return  Index of start of first occurrence.
        */
        size_type rfind(const basic_string& str, size_type pos = npos) const;
        /*
        ** @brief   Find last position of a C substring.
        ** @param   cstr    C-String to locate.
        ** @param   pos     Index of character to search back from.
        ** @param   n       Number of characters from cstr to search for.
        ** @return  Index of start of first occurrence.
        */
        size_type rfind(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find last position of a C-String.
        ** @param   cstr    C-String to locate.
        ** @param   pos     Index of character to search back from. (Default = npos)
        ** @return  Index of start of first occurrence.
        */
        size_type rfind(const CharT* cstr, size_type pos = npos) const;   
        /*
        ** @brief   Find last position of a Character.
        ** @param   ch      Character to locate.
        ** @param   pos     Index of character to search back from. (Default = npos)
        ** @return  Index of start of first occurrence.
        */
        size_type rfind(CharT ch, size_type pos = npos) const;

        /*
        ** @brief   Find position of a char of string.
        ** @param   str     The string containing characters to locate.
        ** @param   pos     Index of character to search from.(Default = 0)
        ** @return  Index of first occurrence.
        ** 
        ** Start from pos, searches forward for one of the characters of str
        ** within this string. If not found, returns npos.
        */
        size_type find_first_of(const basic_string& str, size_type pos = 0) const;
        /*
        ** @brief   Find position of a char of C substring.
        ** @param   str     The C-String containing characters to locate.
        ** @param   pos     Index of character to search from
        ** @param   n       Number of characters from cstr to search for.
        ** @return  Index of first occurrence.
        **
        ** Start from pos, searches forward for one of the frist n 
        ** characters of cstr within this string. If not found, returns npos.
        */
        size_type find_first_of(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find position of a char of C-String.
        ** @param   str     The C-String containing characters to locate.
        ** @param   pos     Index of character to search from.(Default = 0)
        ** @return  Index of first occurrence.
        **
        ** Start from pos, searches forward for one of the characters
        ** of cstr within this string. If not found, returns npos.
        */
        size_type find_first_of(const CharT* cstr, size_type pos = 0) const;
        /*
        ** @brief   Find position of a Char.
        ** @param   ch      Character to locate.
        ** @param   pos     Index of character to search from.(Default = 0)
        ** @return  Index of first occurrence.
        **
        ** Start from pos, searches forward for the character ch within
        ** this string. If not found, returns npos.
        */
        size_type find_first_of(CharT ch, size_type pos = 0) const;

        /*
        ** @brief   Find last position of a char of string.
        ** @param   str     The string containing characters to locate.
        ** @param   pos     Index of character to search back from.(Default = npos)
        ** @return  Index of last occurrence.
        **
        ** Start from pos, searches backward for one of the characters of str
        ** within this string. If not found, returns npos.
        */
        size_type find_last_of(const basic_string& str, size_type pos = npos) const;
        /*
        ** @brief   Find last position of a char of C substring.
        ** @param   str     The C-String containing characters to locate.
        ** @param   pos     Index of character to search back from.(Default = npos)
        ** @return  Index of last occurrence.
        **
        ** Start from pos, searches backward for one of the first n characters
        ** of cstr within this string. If not found, returns npos.
        */
        size_type find_last_of(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find last position of a char of C-String.
        ** @param   str     The C-String containing characters to locate.
        ** @param   pos     Index of character to search back from.(Default = npos)
        ** @return  Index of last occurrence.
        **
        ** Start from pos, searches backward for one of the characters of cstr
        ** within this string. If not found, returns npos.
        */
        size_type find_last_of(const CharT* cstr, size_type pos = npos) const;
        /*
        ** @brief   Find last position of a char.
        ** @param   str     Character to locate.
        ** @param   pos     Index of character to search back from.(Default = npos)
        ** @return  Index of last occurrence.
        **
        ** Start from pos, searches backward for the character ch
        ** within this string. If not found, returns npos.
        */
        size_type find_last_of(CharT ch, size_type pos = npos) const;

        /*
        ** @brief   Find position of a character not in string.
        ** @param   str    String containing characters to avoid.
        ** @param   pos    Index of character to search from (Default = 0).
        ** @return  Index of first occurrence.
        **
        ** Starting from pos, searches forward for a character not contained
        ** in str within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_first_not_of(const basic_string& str, size_type pos = 0) const;
        /*
        ** @brief   Find position of a character not in C subtring.
        ** @param   str     C-String containing characters to avoid.
        ** @param   pos     Index of character to search from.
        ** @param   n       Number of character to search from.
        ** @return  Index of first occurrence.
        **
        ** Starting from pos, searches forward for a character not contained
        ** in first n characters of cstr within this string.  If found, returns
        ** the index where it was found.  If not found, returns npos.
        */
        size_type find_first_not_of(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find position of a character not in C-String.
        ** @param   str    C-String containing characters to avoid.
        ** @param   pos    Index of character to search from (Default = 0).
        ** @return  Index of first occurrence.
        **
        ** Starting from pos, searches forward for a character not contained
        ** in cstr within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_first_not_of(const CharT* cstr, size_type pos = 0) const;
        /*
        ** @brief   Find position of a different character.
        ** @param   str    Character to avoid.
        ** @param   pos    Index of character to search from (Default = 0).
        ** @return  Index of first occurrence.
        **
        ** Starting from pos, searches forward for a character different from
        ** ch within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_first_not_of(CharT ch, size_type pos = 0) const;

        /*
        ** @brief   Find last position of a character not in string.
        ** @param   str    String containing characters to avoid.
        ** @param   pos    Index of character to search back from (Default = npos).
        ** @return  Index of last occurrence.
        **
        ** Starting from pos, searches backward for a character not contained
        ** in str within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;
        /*
        ** @brief   Find last position of a character not in C subtring.
        ** @param   str     C-String containing characters to avoid.
        ** @param   pos     Index of character to search back from.
        ** @param   n       Number of character to search back from.
        ** @return  Index of last occurrence.
        **
        ** Starting from pos, searches backward for a character not contained
        ** in first n characters of cstr within this string.  If found, returns
        ** the index where it was found.  If not found, returns npos.
        */
        size_type find_last_not_of(const CharT* cstr, size_type pos, size_type n) const;
        /*
        ** @brief   Find last position of a character not in C-String.
        ** @param   str    C-String containing characters to avoid.
        ** @param   pos    Index of character to search back from (Default = npos).
        ** @return  Index of last occurrence.
        **
        ** Starting from pos, searches backward for a character not contained
        ** in cstr within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_last_not_of(const CharT* cstr, size_type pos = npos) const;
        /*
        ** @brief   Find last position of a different character.
        ** @param   str    Character to avoid.
        ** @param   pos    Index of character to search back from (Default = npos).
        ** @return  Index of last occurrence.
        **
        ** Starting from pos, searches backward for a character different from
        ** ch within this string.  If found, returns the index where it
        ** was found.  If not found, returns npos.
        */
        size_type find_last_not_of(CharT ch, size_type pos = npos) const;

        /*
        ** @brief   Get a substring.
        ** @param   pos     Index of first character.(Default = 0)
        ** @param   n       Number of character in substring.(Default = npos)
        ** @return  The new string.
        */
        basic_string substr(size_type pos = 0, size_type len = npos) const;

        /*
        ** @brief   Compare to a string.
        ** @param   str String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Returns an integer < 0 if this string is ordered before str,
        ** 0 if their values are equivalent, or > 0 if this string is 
        ** ordered after str.
        */
        int compare(const basic_string& str) const;
        /*
        ** @brief   Compare substring to a string.
        ** @param   pos1    Index of first char of substring.
        ** @param   n1      Number of chars in substring.
        ** @param   str     String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n chars starting at pos.
        */
        int compare(size_type pos1, size_type n1, const basic_string& str) const;
        /*
        ** @brief   Compare substring to a substring.
        ** @param   pos1    Index of first char of substring.
        ** @param   n1      Number of chars in substring.
        ** @param   str     String to compare against.
        ** @param   pos2    Index of first char of substring of str.
        ** @param   n2      Number of chars in substring of str.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n1 chars starting at pos1.
        ** Form the substring of str from the n2 characters starting at pos2.
        */
        int compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2) const;
        /*
        ** @brief   Compare to a C-String.
        ** @param   cstr    C-String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Returns an integer < 0 if this string is ordered before cstr,
        ** 0 if their values are equivalent, or > 0 if this string is
        ** ordered after str.
        */
        int compare(const CharT* cstr) const;
        /*
        ** @brief   Compare substring to a C-String.
        ** @param   pos1    Index of first char of substring.
        ** @param   n1      Number of chars in substring.
        ** @param   cstr    C-String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n chars starting at pos.
        */
        int compare(size_type pos1, size_type n1, const CharT* cstr) const;
        /*
        ** @brief   Compare substring to a C substring.
        ** @param   pos1    Index of first char of substring.
        ** @param   n1      Number of chars in substring.
        ** @param   cstr    C-String to compare against.
        ** @param   n2      Number of chars in substring of cstr.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n1 chars starting at pos.
        ** Form a string from the first n2 characters of cstr.
        */
        int compare(size_type pos1, size_type n1, const CharT* cstr, size_type n2) const;

    public:
        // operator+
        friend basic_string operator+ (const basic_string& lhs, const basic_string& rhs);
        friend basic_string operator+ (const CharT* lhs, const basic_string& rhs);
        friend basic_string operator+ (const basic_string& lhs, const CharT* rhs);
        friend basic_string operator+ (const basic_string& lhs, CharT rhs);
        friend basic_string operator+ (CharT lhs, const basic_string& rhs);

        // operator==
        friend bool operator== (const basic_string& lhs, const basic_string& rhs);
        friend bool operator== (const basic_string& lhs, const CharT* rhs);
        friend bool operator== (const CharT* lhs, const basic_string& rhs);

        // operator!=
        friend bool operator!= (const basic_string& lhs, const basic_string& rhs);
        friend bool operator!= (const basic_string& lhs, const CharT* rhs);
        friend bool operator!= (const CharT* lhs, const basic_string& rhs);

        // operator<
        friend bool operator< (const basic_string& lhs, const basic_string& rhs);
        friend bool operator< (const basic_string& lhs, const CharT* rhs);
        friend bool operator< (const CharT* lhs, const basic_string& rhs);

        // operator<=
        friend bool operator<= (const basic_string& lhs, const basic_string& rhs);
        friend bool operator<= (const basic_string& lhs, const CharT* rhs);
        friend bool operator<= (const CharT* lhs, const basic_string& rhs);

        // operator>
        friend bool operator> (const basic_string& lhs, const basic_string& rhs);
        friend bool operator> (const basic_string& lhs, const CharT* rhs);
        friend bool operator> (const CharT* lhs, const basic_string& rhs);

        // operator>=
        friend bool operator>= (const basic_string& lhs, const basic_string& rhs);
        friend bool operator>= (const basic_string& lhs, const CharT* rhs);
        friend bool operator>= (const CharT* lhs, const basic_string& rhs);
        
        friend void swap(basic_string &lhs, basic_string& rhs);
     
        template <CharT>
        friend std::ostream& operator<< (std::ostream& os, const basic_string<CharT>& str);
        template <CharT>
        friend std::istream& operator>> (std::istream& is, basic_string<CharT>& str);
        template <CharT>
        friend std::istream& getline(std::istream& is, basic_string<CharT>& str, char delim);
        template <CharT>
        friend std::istream& getline(std::istream& is, basic_string<CharT>& str);

    private:
        //Aux function
        /*
        ** @brief   Move the data to this string.
        */
        void moveData(basic_string& str) {
            _start = str._start;
            _finish = str._finish;
            _endOfStorage = str._endOfStorage;
            str._start = str._finish = str._endOfStorage = 0;
        }
        /*
        ** @brief   Allocate memory for n elem and fill with same character.
        */
        void allocate_and_fill(size_type n, CharT ch) {
            _start = data_allocator::allocate(n);
            _finish = rayn::uninitialized_fill_n(_start, n, ch);
            _endOfStorage = _finish;
        }
        /*
        ** @brief   Allocate memory and copy data from range [first, last) into.
        */
        template <class InputIterator>
        void allocate_and_copy(InputIterator first, InputIterator last) {
            _start = data_allocator::allocate(last - first);
            _finish = rayn::uninitialized_copy(first, last, _start);
            _endOfStorage = _finish;
        }
        /*
        ** @brief   Destroy data and deallocate memory.
        */
        void destroy_and_deallocate() {
            rayn::destroy(_start, _finish);
            data_allocator::deallocate(_start, _endOfStorage - _start);
        }
        /*
        ** @brief   如果原大小为0，则配置为len, 否则配置为 旧大小 * 2 or 旧大小 + 增加长度
        ** @param   len
        */
        size_type getNewCapacity(size_type len) {
            size_type oldCapacity = _endOfStorage - _start;
            size_type newCapacity = oldCapacity + rayn::max(oldCapacity, len);
            return newCapacity;
        }
        /*
        ** @brief   Fix var when var equal to npos.
        */
        size_type fix_npos(size_type var, size_type length, size_type off) const {
            return var == npos ? (length - off) : var;
        }
        /*
        ** @brief   Insert aux, allocate memory and fill with char.
        */
        iterator insert_and_fill(iterator p, size_type n, value_type ch) {
            size_type newCapacity = getNewCapacity(n);
            iterator newStart = data_allocator::allocate(newCapacity);
            iterator newFinish = rayn::uninitialized_copy(_start, p, newStart);
            newFinish = rayn::uninitialized_fill_n(newFinish, n, ch);
            iterator ret = newFinish;
            newFinish = rayn::uninitialized_copy(p, _finish, newFinish);

            destroy_and_deallocate();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = _start + newCapacity;
            return ret;
        }
        /*
        ** @brief   Insert aux, allocate memory and copy range into.
        */
        template <class InputIterator>
        iterator insert_and_copy(iterator p, InputIterator first, InputIterator last) {
            size_type lengthOfAdd = last - first;
            size_type newCapacity = getNewCapacity(lengthOfAdd);
            iterator newStart = data_allocator::allocate(newCapacity);
            iterator newFinish = rayn::uninitialized_copy(_start, p, newStart);
            newFinish = rayn::uninitialized_copy(first, last, newFinish);
            iterator ret = newFinish;
            newFinish = rayn::uninitialized_copy(p, _finish, newFinish);

            destroy_and_deallocate();
            _start = newStart;
            _finish = newFinish;
            _endOfStorage = _start + newCapacity;
            return ret;
        }
        /*
        ** @brief   Find aux, from range [st, ed) to find substring equal to [it, it + length).
        */
        size_type find_aux(size_type st, size_type ed, const_iterator it, size_type length) const {
            size_type i, j;
            for (i = st; i != ed; ++i) {
                for (j = 0; j != length; ++j) {
                    if (*(begin() + i + j) != *(it + j)) {
                        break;
                    }
                }
                if (j == length) return i;
            }
            return npos;
        }
        /*
        ** @brief   rFind aux, from range [st, ed) to find substring equal to [it, it + length).
        */
        size_type rfind_aux(size_type st, size_type ed, const_iterator it, size_type length) const {
            size_type i, j;
            for (i = st; i >= ed; --i) {
                for (j = 0; j != length; ++j) {
                    if (*(begin() + i + j) != *(it + j)) {
                        break;
                    }
                }
                if (j == length) return i;
            }
            return npos;
        }
        /*
        ** @brief Test whether the char is in range [first, last) or not.
        */
        bool isContained(CharT ch, const_iterator first, const_iterator last) const {
            for (const_iterator it = first; it != last; ++it) {
                if (*it == ch) {
                    return true;
                }
            }
            return false;
        }

        /*
        ** @brief Compare aux.
        */
        int compare_aux(size_type pos1, size_type n1, const_iterator it, size_type pos2, size_type n2) const {
            size_type i, j;
            for (i = 0, j = 0; i != n1 && j != n2; ++i, ++j) {
                if (*(begin() + pos1 + i) < *(it + pos2 + j)) {
                    return -1;
                } else if (*(begin() + pos1 + i) > *(it + pos2 + j)) {
                    return 1;
                }
            }
            if (i == n1 && j == n2) {
                return 0;
            } else if (i == n1) {
                return -1;
            } else {
                return 1;
            }
        }
    };

    typedef basic_string<char>      string;
    typedef basic_string<wchar_t>   wstring;

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
        n = fix_npos(n, size(), pos);
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

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::copy(const CharT* cstr, size_type len, size_type pos = 0) const {
        iterator tail = rayn::uninitialized_copy(begin() + pos, begin() + pos + len, cstr);
        return static_cast<size_type>(tail - cstr);
    }
    template <class CharT>
    void basic_string<CharT>::swap(basic_string& str) {
        rayn::swap(_start, str._start);
        rayn::swap(_finish, str._finish);
        rayn::swap(_endOfStorage, str._endOfStorage);
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find(const basic_string& str, size_type pos = 0) const {
        size_type length = str.length();
        if (size() - pos < length) return npos;
        return find_aux(pos, size(), str.cbegin(), length);
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find(const CharT* cstr, size_type pos, size_type n) const {
        return find_aux(pos, size(), cstr, n);
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find(const CharT* cstr, size_type pos = 0) const {
        return find(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find(CharT ch, size_type pos = 0) const {
        for (const_iterator it = cbegin() + pos; it != cend(); ++it) {
            if (*it == ch) {
                return it - cbegin();
            }
        }
        return npos;
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::rfind(const basic_string& str, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return rfind_aux(pos, 0, str.cbegin(), str.length());
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::rfind(const CharT* cstr, size_type pos, size_type n) const {
        return rfind_aux(pos, 0, cstr, n);
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::rfind(const CharT* cstr, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return rfind(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::rfind(CharT ch, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        for (const_iterator it = cbegin() + pos; it >= cbegin(); --it) {
            if (*it == ch) {
                return it - cbegin();
            }
        }
        return npos;
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_of(const basic_string& str, size_type pos = 0) const {
        return find_first_of(str.begin(), pos, str.size());
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_of(const CharT* cstr, size_type pos, size_type n) const {
        for (size_type i = pos; i != size(); ++i) {
            if (isContained(*(begin() + i), cstr, cstr + n)) {
                return i;
            }
        }
        return npos;
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_of(const CharT* cstr, size_type pos = 0) const {
        return find_first_of(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_of(CharT ch, size_type pos = 0) const {
        return find(ch, pos);
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_of(const basic_string& str, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return find_last_of(str.begin(), pos, str.size());
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_of(const CharT* cstr, size_type pos, size_type n) const {
        pos = fix_npos(pos, size(), 1);
        for (size_type i = pos; i >= 0; --i) {
            if (isContained(*(begin() + i), cstr, cstr + n)) {
                return i;
            }
        }
        return npos;
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_of(const CharT* cstr, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return find_last_of(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_of(CharT ch, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return rfind(ch, pos);
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_not_of(const basic_string& str, size_type pos = 0) const {
        return find_first_not_of(str.begin(), pos, str.size());
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_not_of(const CharT* cstr, size_type pos, size_type n) const {
        for (size_type i = pos; i != size(); ++i) {
            if (!isContained(*(begin() + i), cstr, cstr + n)) {
                return i;
            }
        }
        return npos;
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_not_of(const CharT* cstr, size_type pos = 0) const {
        return find_last_not_of(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_first_not_of(CharT ch, size_type pos = 0) const {
        for (size_type i = pos; i != size(); ++i) {
            if (*(begin() + i) != ch) {
                return i;
            }
        }
        return npos;
    }

    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_not_of(const basic_string& str, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return find_last_not_of(str.begin(), pos, str.size());
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_not_of(const CharT* cstr, size_type pos, size_type n) const {
        pos = fix_npos(pos, size(), 1);
        for (size_type i = pos; i >= 0; --i) {
            if (!isContained(*(begin() + i), cstr, cstr + n)) {
                return i;
            }
        }
        return npos;
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_not_of(const CharT* cstr, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        return find_last_not_of(cstr, pos, strlen(cstr));
    }
    template <class CharT>
    typename basic_string<CharT>::size_type
        basic_string<CharT>::find_last_not_of(CharT ch, size_type pos = npos) const {
        pos = fix_npos(pos, size(), 1);
        for (size_type i = pos; i >= 0; --i) {
            if (*(begin() + i) != ch) {
                return i;
            }
        }
        return npos;
    }

    template <class CharT>
    basic_string<CharT>
        basic_string<CharT>::substr(size_type pos = 0, size_type len = npos) const {
        len = fix_npos(len, size(), pos);
        return basic_string<CharT>(begin() + pos, begin() + pos + len);
    }

    template <class CharT>
    int basic_string<CharT>::compare(const basic_string& str) const {
        return compare(0, size(), str, 0, str.size());
    }
    template <class CharT>
    int basic_string<CharT>::compare(size_type pos1, size_type n1, const basic_string& str) const {
        return compare(pos1, n1, str, 0, str.size());
    }
    template <class CharT>
    int basic_string<CharT>::compare(size_type pos1, size_type n1,
        const basic_string& str, size_type pos2, size_type n2) const {
        return compare_aux(pos1, n1, str.begin(), pos2, n2);
    }
    template <class CharT>
    int basic_string<CharT>::compare(const CharT* cstr) const {
        return compare(0, size(), cstr, strlen(cstr));
    }
    template <class CharT>
    int basic_string<CharT>::compare(size_type pos1, size_type n1, const CharT* cstr) const {
        return compare(pos1, n1, cstr, strlen(cstr));
    }
    template <class CharT>
    int basic_string<CharT>::compare(size_type pos1, size_type n1, const CharT* cstr, size_type n2) const {
        return compare_aux(pos1, n1, cstr, 0, n2);
    }

    // Friend Functions
    // operator +
    template <class CharT>
    basic_string<CharT> operator+ (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        basic_string<CharT> res(lhs);
        return res += rhs;
    }
    template <class CharT>
    basic_string<CharT> operator+ (const CharT* lhs, const basic_string<CharT>& rhs) {
        basic_string<CharT> res(lhs);
        return res += rhs;
    }
    template <class CharT>
    basic_string<CharT> operator+ (const basic_string<CharT>& lhs, const CharT* rhs) {
        basic_string<CharT> res(lhs);
        return res += rhs;
    }
    template <class CharT>
    basic_string<CharT> operator+ (const basic_string<CharT>& lhs, CharT rhs) {
        basic_string<CharT> res(lhs);
        return res += rhs;
    }
    template <class CharT>
    basic_string<CharT> operator+ (CharT lhs, const basic_string<CharT>& rhs) {
        basic_string<CharT> res(lhs);
        return res += rhs;
    }

    //operator==
    template <class CharT>
    bool operator== (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        if (lhs.size() == rhs.size()) {
            for (auto cit1 = lhs.cbegin(), cit2 = rhs.cbegin();
                cit1 != lhs.cend() && cit2 != rhs.cend();
                ++cit1, ++cit2) {
                if (*cit1 != *cit2) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    template <class CharT>
    bool operator== (const basic_string<CharT>& lhs, const CharT* rhs) {
        size_t len1 = lhs.size(), len2 = strlen(rhs);
        if (len1 == len2) {
            auto cit1 = lhs.cbegin();
            auto cit2 = rhs;
            for ( ; cit1 != lhs.cend() && cit2 != rhs + len2;
                ++cit1, ++cit2) {
                if (*cit1 != *cit2) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    template <class CharT>
    bool operator== (const CharT* lhs, const basic_string<CharT>& rhs) {
        return rhs == lhs;
    }

    //operator!=
    template <class CharT>
    bool operator!= (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        return !(lhs == rhs);
    }
    template <class CharT>
    bool operator!= (const basic_string<CharT>& lhs, const CharT* rhs) {
        return !(lhs == rhs);
    }
    template <class CharT>
    bool operator!= (const CharT* lhs, const basic_string<CharT>& rhs) {
        return !(lhs == rhs);
    }

    //operator<
    template <class CharT>
    bool operator< (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        return lhs.compare(rhs) < 0;
    }
    template <class CharT>
    bool operator< (const basic_string<CharT>& lhs, const CharT* rhs) {
        return lhs.compare(rhs) < 0;
    }
    template <class CharT>
    bool operator< (const CharT* lhs, const basic_string<CharT>& rhs) {
        return rhs.compare(lhs) > 0;
    }

    //operator<=
    template <class CharT>
    bool operator<= (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        return lhs.compare(rhs) <= 0;
    }
    template <class CharT>
    bool operator<= (const basic_string<CharT>& lhs, const CharT* rhs) {
        return lhs.compare(rhs) <= 0;
    }
    template <class CharT>
    bool operator<= (const CharT* lhs, const basic_string<CharT>& rhs) {
        return rhs.compare(lhs) >= 0;
    }

    //operator>
    template <class CharT>
    bool operator> (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        return lhs.compare(rhs) > 0;
    }
    template <class CharT>
    bool operator> (const basic_string<CharT>& lhs, const CharT* rhs) {
        return lhs.compare(rhs) > 0;
    }
    template <class CharT>
    bool operator> (const CharT* lhs, const basic_string<CharT>& rhs) {
        return rhs.compare(lhs) < 0;
    }

    //operator>=
    template <class CharT>
    bool operator>= (const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        return lhs.compare(rhs) >= 0;
    }
    template <class CharT>
    bool operator>= (const basic_string<CharT>& lhs, const CharT* rhs) {
        return lhs.compare(rhs) >= 0;
    }
    template <class CharT>
    bool operator>= (const CharT* lhs, const basic_string<CharT>& rhs) {
        return rhs.compare(lhs) <= 0;
    }

    template <class CharT>
    void swap(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        lhs.swap(rhs);
    }

    template <class CharT>
    std::ostream& operator<< (std::ostream& os, const basic_string<CharT>& str) {
        for (auto it = str.begin(); it != str.end(); ++it) {
            os << *it;
        }
        return os;
    }
    template <class CharT>
    std::istream& operator>> (std::istream& is, const basic_string<CharT>& str) {
        CharT ch;
        bool hasPrevBlank = false;
        while (is.get(ch)) {
            if (isblank(ch) || ch == '\n') {
                hasPrevBlank = true;
            } else {
                break;
            }
        }
        is.putback(ch);
        str.clear();
        while (is.get(ch)) {
            if (ch != EOF && !isblank(ch) && ch != '\n') {
                str.push_back(ch);
            } else {
                break;
            }
        }
        return is;
    }
    template <class CharT>
    std::istream& getline(std::istream& is, basic_string<CharT>& str, CharT delim) {
        CharT ch;
        str.clear();
        while (is.get(ch)) {
            if (ch == delim) {
                break;
            } else {
                str.push_back(ch);
            }
        }
        return is;
    }
    template <class CharT>
    std::istream& getline(std::istream& is, basic_string<CharT>& str) {
        return getline(is, str, '\n');
    }

    // Numeric Conversions [string.conversions].
}

#endif
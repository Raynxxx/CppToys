/*
** String.h
** Created by Rayn on 2015/02/25
** basic_string 定义
*/
#ifndef _STRING_H_
#define _STRING_H_

#include <cstring>
#include <iostream>
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
        /*
        ** @brief   Insert value of string.
        ** @param   pos The iterator refer to pos to insert.
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
        basic_string& insert(size_type pos, const basic_string& str, size_type subpos, size_type sublen = npos);
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
        ** @brief   Remove characters.
        ** @param   pos Iterator refer to The start index to remove. 
        ** @return  Return the iterator refer to the location of first char removal.
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
        void pop_back();

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
        ** @brief   Replace characters from C-Style string.
        ** @param   pos     Index of first char to replace.
        ** @param   len     Number of char to be replaced.
        ** @param   len2    Number of char to insert.
        ** @param   ch      Characters to insert.
        ** @return  Reference to this string.
        */
        basic_string& replace(size_type pos, size_type len, size_type len2, CharT ch);
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
        ** @brief   Replace range of characters with C-Style string.
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
        const CharT* c_str() const;
        /*
        ** @brief   Return const pointer to contents.
        */
        const CharT* data() const;

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
        ** @brief   Find position of a String.
        ** @param   cstr    String to locate.
        ** @param   pos     Index of character to search from. (Default = 0)
        ** @return  Index of start of first occurrence.
        */
        size_type find(const basic_string& str, size_type pos = 0) const;
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
        size_type rfind(const CharT* cstr, size_type pos = 0) const;   
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
        ** @param   pos     Index of first char of substring.
        ** @param   n       Number of chars in substring.
        ** @param   str     String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n chars starting at pos.
        */
        int compare(size_type pos, size_type n, const basic_string& str) const;
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
        int compare(const CharT* cstr);
        /*
        ** @brief   Compare substring to a C-String.
        ** @param   pos     Index of first char of substring.
        ** @param   n       Number of chars in substring.
        ** @param   cstr    C-String to compare against.
        ** @return  Integer < 0, 0, or > 0.
        **
        ** Form the substring of this string from the n chars starting at pos.
        */
        int compare(size_type pos, size_type n, const CharT* cstr) const;
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
        int compare(size_type pos, size_type n1, const CharT* cstr, size_type n2) const;

    public:
        // operator+
        /*
        ** @brief   Concatenate two strings.
        ** return   New string with value lhs + rhs.
        */
        friend basic_string operator+ (const basic_string& lhs, const basic_string& rhs);
        /*
        ** @brief   Concatenate a C-String and a string.
        ** return   New string with value lhs + rhs.
        */
        friend basic_string operator+ (const CharT* lhs, const basic_string& rhs);
        /*
        ** @brief   Concatenate a string and a C-String.
        ** return   New string with value lhs + rhs.
        */
        friend basic_string operator+ (const basic_string& lhs, const CharT* rhs);
        /*
        ** @brief   Concatenate a string and a character.
        ** return   New string with value lhs + rhs.
        */
        friend basic_string operator+ (const basic_string& lhs, CharT rhs);
        /*
        ** @brief   Concatenate a character and a string.
        ** return   New string with value lhs + rhs.
        */
        friend basic_string operator+ (CharT lhs, const basic_string& rhs);

        // operator==
        /*
        ** @brief   Test equivalence of two strings.
        ** return   True if lhs.compare(rhs) == 0.  False otherwise.
        */
        friend bool operator== (const basic_string& lhs, const basic_string& rhs);
        /*
        ** @brief   Test equivalence of a string and a C-String.
        ** return   True if lhs.compare(rhs) == 0.  False otherwise.
        */
        friend bool operator== (const basic_string& lhs, const CharT* rhs);
        /*
        ** @brief   Test equivalence of a string and a C-String.
        ** return   True if lhs.compare(rhs) == 0.  False otherwise.
        */
        friend bool operator== (const CharT* lhs, const basic_string& rhs);

        // operator!=
        /*
        ** @brief   Test difference of two strings.
        ** return   True if lhs.compare(rhs) != 0.  False otherwise.
        */
        friend bool operator!= (const basic_string& lhs, const basic_string& rhs);
        /*
        ** @brief   Test difference of a string and a C-String.
        ** return   True if lhs.compare(rhs) != 0.  False otherwise.
        */
        friend bool operator!= (const basic_string& lhs, const CharT* rhs);
        /*
        ** @brief   Test difference of a string and a C-String.
        ** return   True if lhs.compare(rhs) != 0.  False otherwise.
        */
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
     
        friend std::ostream& operator << (std::ostream& os, const basic_string& str);
        friend std::istream& operator >> (std::istream& is, basic_string& str);
        friend std::istream& getline(std::istream& is, basic_string& str, char delim);
        friend std::istream& getline(std::istream& is, basic_string& str);

    private:
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
    };

    typedef basic_string<char>      string;
    typedef basic_string<wchar_t>   wstring;
}

#endif
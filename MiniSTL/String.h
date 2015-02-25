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
        char *_start;
        char *_finish;
        char *_endOfStorage;

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

        //Assign str to this string.
        basic_string& operator= (const basic_string& str);
        //Move Assign str to this string.
        basic_string& operator= (basic_string&& str);
        //Copy cstr into this string.
        basic_string& operator= (const CharT* cstr);
        // Set value to string of length 1.
        basic_string& operator= (CharT ch);

        // Destroy the string instance.
        ~basic_string();

    };

    typedef basic_string<char>      string;
    typedef basic_string<wchar_t>   wstring;
}

#endif
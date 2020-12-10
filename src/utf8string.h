/*
    Original UTF8 Support code by Nemanja Trifunovic, I modified it,
    and  only took the parts that I needed for this class.

    Both his code and mine is under the same license, the BSL 1.0

    Check his work out at: https://github.com/nemtrif/utfcpp
*/

// Copyright 2006 Nemanja Trifunovic
// Copyright 2020 Suhail Alhegry

/*
    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#ifndef RYUK_UTF8_H
#define RYUK_UTF8_H

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <utility>
#include <ostream>

namespace ryuk {
    using u8char_t = unsigned char;
    using u32char_t = char32_t;

    namespace internal {
        constexpr u32char_t CODE_POINT_MAX = 0x0010ffffu;

        inline u8char_t mask(u8char_t c) {
            return (0xFF & c);
        }

        inline bool is_trail(u8char_t c) {
            return ((mask(c) >> 6) == 0x2);
        }

        inline bool is_code_point_valid(u32char_t c) {
            return (c <= CODE_POINT_MAX);
        }

        inline uint8_t sequence_length(u8char_t c) {
            u8char_t lead = mask(c);

            if (lead < 0x80) {
                return 1;
            } else if ((lead >> 5) == 0x6) {
                return 2;
            } else if ((lead >> 4) == 0xE) {
                return 3;
            } else if ((lead >> 3) == 0x1E) {
                return 4;
            }
            
            return 0;
        }

        inline uint8_t sequence_length(u32char_t c) {
            if (c < 0x80) {
                return 1;
            } else if (c < 0x800) {
                return 2;
            } else if (c < 0x10000) {
                return 3;
            } else {
                return 4;
            }

            return 0;
        }

        inline bool is_overlong_sequence(u32char_t c, uint8_t length) {
            if (c < 0x80) {
                if (length != 1) {
                    return true;
                }
            } else if (c < 0x800) {
                if (length != 2) {
                    return true;
                }
            } else if (c < 0x10000) {
                if (length != 3) {
                    return true;
                }
            }

            return false;
        }

        enum UTF8Error {
            UTF8Error_None,
            UTF8Error_NotEnoughRoom,
            UTF8Error_InvalidLead,
            UTF8Error_IncompleteSequence,
            UTF8Error_OverlongSequence,
            UTF8Error_InvalidCodePoint,
        };

        UTF8Error increase_safely(u8char_t *&itr, u8char_t *end) {
            if (++itr == end) {
                return UTF8Error_NotEnoughRoom;
            }
            
            if (!is_trail(*itr)) {
                return UTF8Error_IncompleteSequence;
            }

            return UTF8Error_None;
        }

        UTF8Error get_sequence1(u8char_t *&itr, u8char_t *end, u32char_t &c) {
            if (itr == end) {
                return UTF8Error_NotEnoughRoom;
            }

            c = mask(*itr);
            
            return UTF8Error_None;
        }

        #define increase_safely_and_return_on_error(itr, end)\
                {\
                    UTF8Error result = increase_safely(itr, end);\
                    if (result != UTF8Error_None) {\
                        return result;\
                    }\
                }

        UTF8Error get_sequence2(u8char_t *&itr, u8char_t *end, u32char_t &c) {
            if (itr == end) {
                return UTF8Error_NotEnoughRoom;
            }

            c = mask(*itr);

            increase_safely_and_return_on_error(itr, end);

            c = ((c >> 6) & 0x7FF) + ((*itr) & 0x3F);

            return UTF8Error_None;
        }

        UTF8Error get_sequence3(u8char_t *&itr, u8char_t *end, u32char_t &c) {
            if (itr == end) {
                return UTF8Error_NotEnoughRoom;
            }

            c = mask(*itr);

            increase_safely_and_return_on_error(itr, end);

            c = ((c << 12) & 0xFFFF) + ((mask(*itr) << 6) & 0xFFF);

            increase_safely_and_return_on_error(itr, end);

            c += (*itr) & 0x3F;

            return UTF8Error_None;
        }

        UTF8Error get_sequence4(u8char_t *&itr, u8char_t *end, u32char_t &c) {
            if (itr == end) {
                return UTF8Error_NotEnoughRoom;
            }

            c = mask(*itr);

            increase_safely_and_return_on_error(itr, end);

            c = ((c << 18) & 0x1FFFFF) + ((mask(*itr) << 12) & 0x3FFFF);

            increase_safely_and_return_on_error(itr, end);

            c += (mask(*itr) << 6) & 0xFFF;

            increase_safely_and_return_on_error(itr, end);

            c += (*itr) & 0x3F;

            return UTF8Error_None;
        }

        UTF8Error validate_next(u8char_t *&itr, u8char_t *end, u32char_t &c) {
            if (itr == end) {
                return UTF8Error_NotEnoughRoom;
            }

            u8char_t *original = itr;
            u32char_t cp = 0;

            const uint8_t length = sequence_length(*itr);

            UTF8Error result = UTF8Error_None;

            switch (length) {
                case 0: return UTF8Error_InvalidLead;
                case 1: {
                    result = get_sequence1(itr, end, cp);
                } break;
                case 2: {
                    result = get_sequence2(itr, end, cp);
                } break;
                case 3: {
                    result = get_sequence3(itr, end, cp);
                } break;
                case 4: {
                    result = get_sequence4(itr, end, cp);
                } break;
            }

            if (result == UTF8Error_None) {
                if (is_code_point_valid(cp)) {
                    if (!is_overlong_sequence(cp, length)) {
                        c = cp;
                        ++itr;
                        return UTF8Error_None;
                    }

                    result = UTF8Error_OverlongSequence;
                } else {
                    result = UTF8Error_InvalidCodePoint;
                }
            }

            itr = original;
            return result;
        }

        inline UTF8Error validate_next(u8char_t *&itr, u8char_t* end) {
            u32char_t ignored;
            return validate_next(itr, end, ignored);
        }

        constexpr u8char_t BOM[] = { 0xEF, 0xBB, 0xBF };

        u8char_t *find_invalid(u8char_t *start, u8char_t *end) {
            u8char_t *result = start;

            while (result != end) {
                UTF8Error error = validate_next(result, end);
                if (error != UTF8Error_None) {
                    return result;
                }
            }

            return result;
        }

        inline bool is_valid(u8char_t *start, u8char_t *end) {
            return (find_invalid(start, end) == end);
        }

        inline bool starts_with_bom(u8char_t *itr, u8char_t *end) {
            return (
                ((itr != end) && (mask(*itr++)) == BOM[0]) &&
                ((itr != end) && (mask(*itr++)) == BOM[1]) &&
                ((itr != end) && (mask(*itr)) == BOM[2])
            );
        }

        u8char_t *append(u32char_t cp, u8char_t *result) {
            if (!is_code_point_valid(cp)) {
                return result;
            }

            if (cp < 0x80) {
                *(result++) = static_cast<u8char_t>(  cp);
            } else if (cp < 0x800) {
                *(result++) = static_cast<u8char_t>( (cp  >>    6)          | 0xc0);
                *(result++) = static_cast<u8char_t>( (cp  &  0x3f)          | 0x80);
            } else if (cp < 0x10000) {
                *(result++) = static_cast<u8char_t>( (cp  >>   12)          | 0xe0);
                *(result++) = static_cast<u8char_t>(((cp  >>    6) & 0x3f)  | 0x80);
                *(result++) = static_cast<u8char_t>( (cp  &  0x3f)          | 0x80);
            } else {
                *(result++) = static_cast<u8char_t>( (cp  >>   18)          | 0xf0);
                *(result++) = static_cast<u8char_t>(((cp  >>   12)  & 0x3f) | 0x80);
                *(result++) = static_cast<u8char_t>(((cp  >>    6)  & 0x3f) | 0x80);
                *(result++) = static_cast<u8char_t>( (cp  &  0x3f)          | 0x80);
            }
            
            return result;
        }

        u32char_t next(u8char_t *&itr, u8char_t *end) {
            u32char_t result = 0;
            validate_next(itr, end, result);
            return result;
        }

        u32char_t peek_next(u8char_t *itr, u8char_t *end) {
            return next(itr, end);
        }

        u32char_t previous(u8char_t *&itr, u8char_t *start) {
            if (itr == start) {
                return 0;
            }
            
            u8char_t *end = itr;

            while (is_trail(*(--itr))) {
                if (itr == start) {
                    return 0;
                }
            }

            return peek_next(itr, end);
        }

        void advance(u8char_t *&itr, u8char_t *end, size_t count) {
            for (size_t i = 0; i < count; ++i) {
                next(itr, end);
            }
        }

        void revert(u8char_t *&itr, u8char_t *start, size_t count) {
            for (size_t i = count; i > 0; --i) {
                previous(itr, start);
            }
        }

        size_t distance(u8char_t *first, u8char_t *last) {
            size_t result;

            for (result = 0; first < last; ++result) {
                next(first, last);
            }

            return result;
        }

        u8char_t *utf32_to_utf8(u32char_t *start, u32char_t *end, u8char_t *result) {
            while (start != end) {
                result = append(*(start++), result);
            }

            return result;
        }

        u32char_t *utf8_to_utf32(u8char_t *start, u8char_t *end, u32char_t *result) {
            while (start < end) {
                *(result++) = next(start, end);
            }

            return result;
        }
    };

    class utf8string_iterator {
    private:
        u8char_t *_begin;
        u8char_t *_end;
        u8char_t *_current;

    public:
        utf8string_iterator(u8char_t *begin, u8char_t *end) : _begin(begin), _end(end) {
            assert(begin);
            assert(end);
            _current = begin;
        }

        ~utf8string_iterator() = default;
        utf8string_iterator(const utf8string_iterator &) = default;
        utf8string_iterator(utf8string_iterator &&) = default;
        utf8string_iterator & operator=(const utf8string_iterator &) = default;
        utf8string_iterator & operator=(utf8string_iterator &&) = default;

        bool operator==(const utf8string_iterator &other) {
            return (_current == other._current);
        }

        bool operator!=(const utf8string_iterator &other) {
            return !(operator==(other));
        }

        utf8string_iterator & operator++() {
            internal::next(_current, _end);
            return *this;
        }

        utf8string_iterator operator++(int) {
            utf8string_iterator temp = *this;
            internal::next(_current, _end);
            return temp;
        }

        utf8string_iterator & operator--() {
            internal::previous(_current, _begin);
            return *this;
        }

        utf8string_iterator operator--(int) {
            utf8string_iterator temp = *this;
            internal::previous(_current, _begin);
            return temp;
        }

        u32char_t operator*() {
            return internal::peek_next(_current, _end);
        }

        bool operator<(const utf8string_iterator &other) {
            return (_current < other._current);
        }

        bool operator>(const utf8string_iterator &other) {
            return (_current > other._current);
        }

        bool operator<=(const utf8string_iterator &other) {
            return (_current <= other._current);
        }

        bool operator>=(const utf8string_iterator &other) {
            return (_current >= other._current);
        }

        u8char_t *begin() {
            return _begin;
        }

        u8char_t *end() {
            return _end;
        }
    };

    class utf8string {
    private:
        size_t _capacity;
        size_t _length;
        u8char_t *_data;

        void grow_buffer(size_t amount) {
            size_t totalLength = _length + amount - 1;
            if (_capacity < totalLength) {
                grow(totalLength - _capacity);    
            }
        }

        void init_buffer(size_t capacity) {
            _data = reinterpret_cast<u8char_t *>(malloc(capacity * sizeof(u8char_t)));
            _capacity = capacity;
        }

        void copy_other(const void *other, size_t otherLength) {
            assert(_capacity >= otherLength);
            memcpy_s(_data, _capacity * sizeof(u8char_t), other, otherLength);
            _length = otherLength;
        }

        void move_other(utf8string &&other) {
            _capacity = other._capacity;
            _length = other._length;
            _data = other._data;
            other._capacity = 0;
            other._length = 0;
            other._data = nullptr;
        }

        void append_other(const void* other, size_t otherLength) {
            assert(_capacity >= _length + otherLength);
            memcpy_s(&_data[_length -1], _capacity * sizeof(u8char_t), other, otherLength);
            _length += otherLength;
        }

        void ensure_capacity(size_t capacity) {
            assert(capacity != SIZE_MAX);
            if (capacity > _capacity) {
                grow(capacity - _capacity);
            }
        }

        void resize(size_t newCapacity) {
            if (newCapacity == _capacity) { return; }

            if (_capacity == 1) {
                _data = reinterpret_cast<u8char_t *>(malloc(newCapacity * sizeof(u8char_t)));
                if (_data) {
                    _capacity = newCapacity;
                }
                return;
            }

            assert(newCapacity >= _length);

            _data = reinterpret_cast<u8char_t *>(realloc(_data, newCapacity * sizeof(u8char_t)));

            if (_data) {
                _capacity = newCapacity;
            }
        }

        void release() {
            if (_data) {
                free(_data);
                _data = nullptr;
            }

            _length = 1;
            _capacity = 1;
        }

        friend std::ostream & operator<<(std::ostream& os, const utf8string &str) {
            return os << reinterpret_cast<const char *>(str._data);   
        }

    public:
        void grow(size_t amount) {
            assert(amount != SIZE_MAX);
            size_t newCapacity = _capacity;

            while (newCapacity < _capacity + amount) {
                newCapacity = newCapacity * 5 / 2 + 8;
            }

            resize(newCapacity);
        }

        void shrink_to_fit() {
            resize(_length);
        }

        const u8char_t *get_raw() {
            return _data;
        }

        utf8string_iterator begin() const {
            return utf8string_iterator(_data, _data + _length);
        }

        utf8string_iterator end() const {
            u8char_t *end = _data + _length;
            return utf8string_iterator(end, end);
        }

        size_t size() {
            return _length - 1;
        }

        size_t capacity() {
            return _capacity - 1;
        }

        size_t count() {
            return internal::distance(_data, &_data[_length - 1]);
        }

        void clear() {
            _length = 1;
        }

        void push(u32char_t c) {
            // you should not push a null character!
            assert(c);

            // remove the null terminator
            --_length;

            size_t seqlen = internal::sequence_length(c);
            //                               + 1 for the null terminator
            ensure_capacity(_length + seqlen + 1);

            internal::append(c, &_data[_length]);
            _length += seqlen;
            _data[_length] = '\0';
            ++_length;
        }

        void append(const char *other) {
            size_t length = strlen(other);
            ensure_capacity(_length + length);
            append_other(other, length);
        }

        void append(const utf8string &other) {
            ensure_capacity(_length + other._length);
            append_other(other._data, other._length);
        }
        
        u8char_t octet_at(size_t index) const {
            assert(index < _length);
            return _data[index];
        }

        u32char_t at(size_t index) const {
            assert(index < _length);
            utf8string_iterator itr = begin();
            utf8string_iterator end = this->end();

            for (size_t i = 0; i < index; ++i) {
                if (itr == end) {
                    return 0;
                }

                ++itr;
            }

            return *itr;
        }

        u32char_t operator[](size_t index) const {
            return at(index);
        }

        bool operator==(const char *other) const {
            size_t length = strlen(other) + 1;
            if (length != _length) { return false; }

            int result = memcmp(other, _data, _length * sizeof(u8char_t));

            // a result of 0 means identical
            if (result == 0) {
                return true;
            }

            return false;
        }

        bool operator!=(const char *other) const {
            return !operator==(other);
        }

        bool operator==(const utf8string &other) const {
            if (_length != other._length) { return false; }

            int result = memcmp(_data, other._data, _length * sizeof(u8char_t));

            // a result of 0 means identical
            if (result == 0) {
                return true;
            }

            return false;
        }

        bool operator!=(const utf8string &other) const {
            return !operator==(other);
        }

        utf8string & operator+=(u32char_t c) {
            push(c);
            return *this;
        }

        utf8string & operator+=(u8char_t c) {
            push(static_cast<u32char_t>(c));
            return *this;
        }

        utf8string & operator+=(char c) {
            push(static_cast<u32char_t>(c));
            return *this;
        }

        utf8string & operator+=(const char *other) {
            append(other);
            return *this;            
        }

        utf8string & operator+=(const utf8string &other) {
            append(other);
            return *this;
        }

        utf8string & operator=(const char *other) {
            release();
            size_t length = strlen(other) + 1;
            ensure_capacity(length);
            copy_other(other, length);
            return *this;
        }

        utf8string & operator=(const utf8string &other) {
            release();
            copy_other(other._data, other._length);
            return *this;
        }

        utf8string & operator=(utf8string&& other) {
            release();
            move_other(std::move(other));
            return *this;
        }

        utf8string() {
            _length = 1;
            _capacity = 1;
            _data = nullptr;
        }

        utf8string(const char *other) {
            size_t length = strlen(other) + 1;
            init_buffer(length);
            copy_other(other, length);
        }

        utf8string(const utf8string &other) {
            init_buffer(other._length);
            copy_other(other._data, other._length);
        }

        utf8string(utf8string &&other) {
            move_other(std::move(other));
        }

        ~utf8string() {
            release();
        }
    };
};

#endif

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

#include "../src/utf8string.h"
#include "test_commons.h"

using namespace ryuk;

namespace {
    constexpr const char *hello_world = "Hello, world!";
    constexpr size_t hello_world_length = 13;
    constexpr const char *bye_world = "Goodbye, world.";
    constexpr size_t bye_world_length = 15;
    constexpr const char *hello_world_u8 = "مرحباً بالعالم!";
    constexpr size_t hello_world_u8_length = 28;
    constexpr size_t hello_world_u8_count = 15;
    constexpr const char *bye_world_u8 = "وداعاً أيها العالم.";
    constexpr size_t bye_world_u8_length = 35;
    constexpr size_t bye_world_u8_count = 19;
    constexpr const char *hello_world_long = "Hello, world!, Hello, world!, Hello, world!, Hello, world!, Hello, world!, Hello, world!.";
    constexpr size_t hello_world_long_length = 89;
    constexpr const char *bye_world_long = "Goodbye, world. Goodbye, world. Goodbye, world. Goodbye, world. Goodbye, world. Goodbye, world.";
    constexpr size_t bye_world_long_length = 95;
    constexpr const char *hello_world_long_u8 = "مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!.";
    constexpr size_t hello_world_long_u8_length = 179;
    constexpr size_t hello_world_long_u8_count = 101;
    constexpr const char *bye_world_long_u8 = "وداعاً أيها العالم. وداعاً أيها العالم. وداعاً أيها العالم. وداعاً أيها العالم. وداعاً أيها العالم.";
    constexpr size_t bye_world_long_u8_length = 179;
    constexpr size_t bye_world_long_u8_count = 99;
};

const char *utf8_string_construct_with_constcharp_sso() {
    utf8string string(hello_world);

    test_assert(string.size() == hello_world_length, "invalid size");
    test_assert(string.count() == hello_world_length, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == hello_world, "invalid content");

    utf8string other_equals(hello_world);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_constcharp_sso() {
    utf8string string;
    string += hello_world;

    test_assert(string.size() == hello_world_length, "invalid size");
    test_assert(string.count() == hello_world_length, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == hello_world, "invalid content");

    utf8string other_equals(hello_world);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_char_sso() {
    utf8string string;
    string += '1';

    test_assert(string.size() == 1, "invalid size");
    test_assert(string.count() == 1, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == "1", "invalid content");

    return nullptr;
}

const char *utf8_string_construct_with_u8constcharp_sso() {
    utf8string string(hello_world_u8);

    test_assert(string.size() == hello_world_u8_length, "invalid size");
    test_assert(string.count() == hello_world_u8_count, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == hello_world_u8, "invalid content");

    utf8string other_equals(hello_world_u8);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world_u8);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_u8, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_u8constcharp_sso() {
    utf8string string;
    string += hello_world_u8;

    test_assert(string.size() == hello_world_u8_length, "invalid size");
    test_assert(string.count() == hello_world_u8_count, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == hello_world_u8, "invalid content");

    utf8string other_equals(hello_world_u8);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world_u8);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_u8, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_u8char_sso() {
    utf8string string;
    string += U'\x645';

    test_assert(string.size() == 2, "invalid size");
    test_assert(string.count() == 1, "invalid count");

    // we get the actual capacity - 1 because we always want to keep a space for
    // the null terminator.
    test_assert(string.capacity() == utf8string::sso_capacity - 1, "invalid capacity");

    test_assert(string == "م", "invalid content");

    return nullptr;
}

const char *utf8_string_construct_with_constcharp() {
    utf8string string(hello_world_long);

    test_assert(string.size() == hello_world_long_length, "invalid size");
    test_assert(string.count() == hello_world_long_length, "invalid count");

    test_assert(string == hello_world_long, "invalid content");

    utf8string other_equals(hello_world_long);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_long, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_constcharp() {
    utf8string string;
    string += hello_world_long;

    test_assert(string.size() == hello_world_long_length, "invalid size");
    test_assert(string.count() == hello_world_long_length, "invalid count");

    test_assert(string == hello_world_long, "invalid content");

    utf8string other_equals(hello_world_long);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_long, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_construct_with_u8constcharp() {
    utf8string string(hello_world_long_u8);

    test_assert(string.size() == hello_world_long_u8_length, "invalid size");
    test_assert(string.count() == hello_world_long_u8_count, "invalid count");

    test_assert(string == hello_world_long_u8, "invalid content");

    utf8string other_equals(hello_world_long_u8);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_long_u8, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_append_u8constcharp() {
    utf8string string;
    string += hello_world_long_u8;

    test_assert(string.size() == hello_world_long_u8_length, "invalid size");
    test_assert(string.count() == hello_world_long_u8_count, "invalid count");

    test_assert(string == hello_world_long_u8, "invalid content");

    utf8string other_equals(hello_world_long_u8);
    test_assert(string == other_equals, "equal strings are not equal");

    utf8string other_not_equals(bye_world);
    test_assert(string != other_not_equals, "unequal strings are equal");

    test_assert(string == hello_world_long_u8, "string is not equal to c-string");
    test_assert(string != bye_world, "string is equal to invalid c-string");

    return nullptr;
}

const char *utf8_string_iterate() {
    utf8string string(hello_world_long_u8);

    size_t count = string.count();
    size_t itrCount = 0;
    for (auto itr = string.begin(); itr != string.end(); ++itr) {
        test_assert(*itr == string[itrCount], "iteration character does not match actual character");
        ++itrCount;
    }
    test_assert(itrCount == count, "iteration count does not match actual count");

    return nullptr;
}

const char *utf8_string_iterate_for() {
    utf8string string(hello_world_long_u8);

    size_t count = string.count();
    size_t itrCount = 0;
    for (auto c : string) {
        test_assert(c == string[itrCount], "iteration character does not match actual character");
        ++itrCount;
    }
    test_assert(itrCount == count, "iteration count does not match actual count");

    return nullptr;
}

const char *utf8_string_iterator_operators() {
    utf8string string(hello_world_long_u8);
    using iterator_t = utf8string_iterator;
    iterator_t iter = string.begin();
    u32char_t startChar = *iter;
    iter++;
    u32char_t secondChar = *iter;
    test_assert(iter > string.begin() && string.begin() < iter, "invalid position after iter++");
    test_assert(startChar == *(--iter), "invalid code point after --iter");
    test_assert(iter >= string.begin() && string.begin() <= iter, "invalid position after --iter");
    test_assert(secondChar == *(++iter), "invalid code point after ++iter");
    test_assert(secondChar == *(iter--), "invalid code point after iter--");
    test_assert(startChar == *iter, "invalid code point after advance and revert operators");
    test_assert(iter == string.begin() && iter != string.end(), "invalid iterator position after advance and revert operators");

    return nullptr;
}

const char *utf8_string_iterate_reverse() {
    utf8string string(hello_world_long_u8);

    size_t count = string.count();
    size_t itrCount = count;
    for (auto itr = string.rbegin(); itr != string.rend(); ++itr) {
        test_assert(*itr == string[itrCount - 1], "iteration character does not match actual character");
        --itrCount;
    }

    test_assert(itrCount == 0, "iteration count does not match actual count");

    return nullptr;
}

const char *utf8_string_reverse_iterator_operators() {
    utf8string string(hello_world_long_u8);
    using iterator_t = utf8string_reverse_iterator;
    iterator_t iter = string.rbegin();
    u32char_t startChar = *iter;
    iter++;
    u32char_t secondChar = *iter;
    test_assert(iter > string.rbegin() && string.rbegin() < iter, "invalid position after iter++");
    test_assert(startChar == *(--iter), "invalid code point after --iter");
    test_assert(iter >= string.rbegin() && string.rbegin() <= iter, "invalid position after --iter");
    test_assert(secondChar == *(++iter), "invalid code point after ++iter");
    test_assert(secondChar == *(iter--), "invalid code point after iter--");
    test_assert(startChar == *iter, "invalid code point after advance and revert operators");
    test_assert(iter == string.rbegin() && iter != string.rend(), "invalid iterator position after advance and revert operators");

    return nullptr;
}

const char *utf8_string_pop_short() {
    utf8string string(hello_world);

    size_t count = string.count();
    u32char_t popped = string.pop();
    test_assert(popped == hello_world[hello_world_length - 1], "invalid popped character");
    test_assert(string.count() == count - 1, "invalid count after pop");
    test_assert(string == "Hello, world", "invalid string after pop");

    return nullptr;
}

const char *utf8_string_pop_short_u8() {
    utf8string string(hello_world_u8);

    size_t count = string.count();
    u32char_t popped = string.pop();
    u8char_t *data = reinterpret_cast<u8char_t *>(const_cast<char *>(hello_world_u8));
    u8char_t *pos = &data[hello_world_u8_length];
    test_assert(popped == internal::previous(pos, data), "invalid popped character");
    test_assert(string.count() == count - 1, "invalid count after pop");
    test_assert(string == "مرحباً بالعالم", "invalid string after pop");

    return nullptr;
}

const char *utf8_string_pop_long() {
    utf8string string(hello_world_long);

    size_t count = string.count();
    u32char_t popped = string.pop();
    test_assert(popped == hello_world_long[hello_world_long_length - 1], "invalid popped character");
    test_assert(string.count() == count - 1, "invalid count after pop");
    test_assert(string == "Hello, world!, Hello, world!, Hello, world!, Hello, world!, Hello, world!, Hello, world!", "invalid string after pop");

    return nullptr;
}

const char *utf8_string_pop_long_u8() {
    utf8string string(hello_world_long_u8);

    size_t count = string.count();
    u32char_t popped = string.pop();
    u8char_t *data = reinterpret_cast<u8char_t *>(const_cast<char *>(hello_world_long_u8));
    u8char_t *pos = &data[hello_world_long_u8_length];
    test_assert(popped == internal::previous(pos, data), "invalid popped character");
    test_assert(string.count() == count - 1, "invalid count after pop");
    test_assert(string == "مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!, مرحباً بالعالم!", "invalid string after pop");

    return nullptr;
}

const char *utf8_string_find_cstring() {
    utf8string string(hello_world);
    using iterator_t = basic_utf8string_iterator;


    const char *start = "Hello";
    const char *middle = "llo, wor";
    const char *end = "orld!";
    const char *none = "ld.";
    const char *empty = "";
    iterator_t found = string.find(start);
    test_assert(found == string.begin(), "failed to find substring at the start of string");
    test_assert(*found++ == 'H', "invalid character 0 at the start of string");
    test_assert(*found++ == 'e', "invalid character 1 at the start of string");
    test_assert(*found++ == 'l', "invalid character 2 at the start of string");
    test_assert(*found++ == 'l', "invalid character 3 at the start of string");
    test_assert(*found++ == 'o', "invalid character 4 at the start of string");
    found = string.find(middle);
    test_assert(found != string.end(), "failed to find substring at the middle of string");
    test_assert(*found++ == 'l', "invalid character 0 at the middle of string");
    test_assert(*found++ == 'l', "invalid character 1 at the middle of string");
    test_assert(*found++ == 'o', "invalid character 2 at the middle of string");
    test_assert(*found++ == ',', "invalid character 3 at the middle of string");
    test_assert(*found++ == ' ', "invalid character 4 at the middle of string");
    test_assert(*found++ == 'w', "invalid character 5 at the middle of string");
    test_assert(*found++ == 'o', "invalid character 6 at the middle of string");
    test_assert(*found++ == 'r', "invalid character 7 at the middle of string");
    found = string.find(end);
    test_assert(found != string.end(), "failed to find substring at the end of string");
    test_assert(*found++ == 'o', "invalid character 0 at the end of string");
    test_assert(*found++ == 'r', "invalid character 1 at the end of string");
    test_assert(*found++ == 'l', "invalid character 2 at the end of string");
    test_assert(*found++ == 'd', "invalid character 3 at the end of string");
    test_assert(*found++ == '!', "invalid character 4 at the end of string");
    found = string.find(none);
    test_assert(found == string.end(), "substring should not be found");
    found = string.find(empty);
    test_assert(found == string.end(), "empty substring should not be found");

    return nullptr;
}

const char *utf8_string_find() {
    utf8string string(hello_world);
    using iterator_t = basic_utf8string_iterator;

    utf8string start("Hello");
    utf8string middle("llo, wor");
    utf8string end("orld!");
    utf8string none("ld.");
    utf8string empty;
    iterator_t found = string.find(start);
    test_assert(found == string.begin(), "failed to find substring at the start of string");
    test_assert(*found++ == 'H', "invalid character 0 at the start of string");
    test_assert(*found++ == 'e', "invalid character 1 at the start of string");
    test_assert(*found++ == 'l', "invalid character 2 at the start of string");
    test_assert(*found++ == 'l', "invalid character 3 at the start of string");
    test_assert(*found++ == 'o', "invalid character 4 at the start of string");
    found = string.find(middle);
    test_assert(found != string.end(), "failed to find substring at the middle of string");
    test_assert(*found++ == 'l', "invalid character 0 at the middle of string");
    test_assert(*found++ == 'l', "invalid character 1 at the middle of string");
    test_assert(*found++ == 'o', "invalid character 2 at the middle of string");
    test_assert(*found++ == ',', "invalid character 3 at the middle of string");
    test_assert(*found++ == ' ', "invalid character 4 at the middle of string");
    test_assert(*found++ == 'w', "invalid character 5 at the middle of string");
    test_assert(*found++ == 'o', "invalid character 6 at the middle of string");
    test_assert(*found++ == 'r', "invalid character 7 at the middle of string");
    found = string.find(end);
    test_assert(found != string.end(), "failed to find substring at the end of string");
    test_assert(*found++ == 'o', "invalid character 0 at the end of string");
    test_assert(*found++ == 'r', "invalid character 1 at the end of string");
    test_assert(*found++ == 'l', "invalid character 2 at the end of string");
    test_assert(*found++ == 'd', "invalid character 3 at the end of string");
    test_assert(*found++ == '!', "invalid character 4 at the end of string");
    found = string.find(none);
    test_assert(found == string.end(), "substring should not be found");
    found = string.find(empty);
    test_assert(found == string.end(), "empty substring should not be found");

    return nullptr;
}

#define run_test(func) tests::run_test((#func), (func))

int main() {
    run_test(utf8_string_construct_with_constcharp_sso);
    run_test(utf8_string_append_constcharp_sso);
    run_test(utf8_string_append_char_sso);
    run_test(utf8_string_construct_with_u8constcharp_sso);
    run_test(utf8_string_append_u8constcharp_sso);
    run_test(utf8_string_append_u8char_sso);
    run_test(utf8_string_construct_with_constcharp);
    run_test(utf8_string_append_constcharp);
    run_test(utf8_string_construct_with_u8constcharp);
    run_test(utf8_string_append_u8constcharp);
    run_test(utf8_string_iterate);
    run_test(utf8_string_iterate_for);
    run_test(utf8_string_iterator_operators);
    run_test(utf8_string_iterate_reverse);
    run_test(utf8_string_reverse_iterator_operators);
    run_test(utf8_string_pop_short);
    run_test(utf8_string_pop_short_u8);
    run_test(utf8_string_pop_long);
    run_test(utf8_string_pop_long_u8);
    run_test(utf8_string_find_cstring);
    run_test(utf8_string_find);
}

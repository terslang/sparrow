#pragma once
#include <string>
#include <vector>
#include <assert.h>
#include "checkers.h"


namespace simple_browser {

class BaseParser {
    public:
    std::string source;
    int32_t position;

    public:
    BaseParser(const std::string& source): source(source), position(0) {}

    bool eof() {
        return position >= source.length();
    }

    bool starts_with_string(std::string str) {
        return source.substr(position, str.length()) == str;
    }

    template <typename F>
    bool starts_with_char_predicate(const F& f) {
        return f(source[position]);
    }

    /****************************************************************/
    template <typename F>
    void advance_position_loop(const F& f) {
        while (position < source.length() 
            && f(source[position])) {
            ++position;
        }
    };

    template <typename F>
    std::string consume_position_loop(const F& f) {
        std::string ret;
        while (position < source.length()
            && f(source[position])) {
            ret.push_back(source[position]);
            ++position;
        }
        return ret;
    }

    void advance_position_string(std::string str) {
        assert(str == source.substr(position, str.length()));
        position += str.length();
    }

    /****************************************************************/
    template <typename F>
    std::string skip_blank_and_consume_position(const F& f) {
        advance_position_loop(is_blank);
        return consume_position_loop(f);
    }

    void skip_blank_and_advance_position_string(std::string str) {
        advance_position_loop(is_blank);
        advance_position_string(str);
    }
};

}
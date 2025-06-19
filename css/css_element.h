#pragma once
#include "../utils/checkers.h"
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <cstdint>



namespace simple_browser_css {

struct Selector {
    std::string id;
    std::string tag;
    std::vector<std::string> class_list;

    std::string to_string();
};

struct Keyword {
    std::string keyword;
};

struct Length {
    float data;
    std::string unit;
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;    
};

enum ValueType {
    KEYWORD,
    LENGTH,
    COLOR,
};

struct Value {
    ValueType type;    
    union {
        struct {
            std::string keyword;
        } Keyword;

        struct {
            float data;
            std::string unit;
        } Length;

        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;    
        } Color;
    };

    Value(const std::string& kwd);
    Value(const std::tuple<float, std::string>& length);
    Value(const std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color);
    Value(const Value& v);
    Value();
    ~Value();

    bool operator==(const Value& v);
    float to_px() const;
    std::string to_string();
};

struct Declaration {
    std::string name;
    struct Value value;

    Declaration(const std::string& name, const std::string& kwd);
    Declaration(const std::string& name, const std::tuple<float, std::string>& length);
    Declaration(const std::string& name, const std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color);
    Declaration(const Declaration& de);
    std::string to_string();
};

struct Rule {
    std::vector<Selector> selectors;
    std::vector<Declaration> declarations;
    Rule(std::vector<Selector> selectors, std::vector<Declaration> declarations):
        selectors(selectors), declarations(declarations) {}

    void print(bool is_last_rule);
};

}

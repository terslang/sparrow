#pragma once
#include "../utils/checkers.h"
#include "../utils/exceptions.h"
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <cstdint>
#include <variant>

using namespace std;

namespace simple_browser_css {

struct Selector {
    string id;
    string tag;
    vector<string> class_list;

    string to_string();
};

struct Keyword {
    string keyword;
    bool operator==(const Keyword& other) const { return keyword == other.keyword; }
};

struct Length {
    float data;
    string unit;
    bool operator==(const Length& other) const { return data == other.data && unit == other.unit; }
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;    
    bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
};

enum ValueType {
    KEYWORD,
    LENGTH,
    COLOR,
};

struct Value {
    ValueType type;    
    std::variant<Keyword, Length, Color> data;

    Value(const string& kwd) : data(Keyword{kwd}) {}
    Value(const tuple<float, string>& length) : data(Length{get<0>(length), get<1>(length)}) {}
    Value(const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color) : data(Color{get<0>(color), get<1>(color), get<2>(color), get<3>(color)}) {}
    Value() : data(Keyword{""}) {} // Default constructor

    bool operator==(const Value& other) const;
    float to_px() const;
    string to_string() const;
};

struct Declaration {
    string name;
    struct Value value;

    Declaration(const string& name, const string& kwd);
    Declaration(const string& name, const tuple<float, string>& length);
    Declaration(const string& name, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color);
    Declaration(const Declaration& de);
    string to_string();
};

struct Rule {
    vector<Selector> selectors;
    vector<Declaration> declarations;
    Rule(vector<Selector> selectors, vector<Declaration> declarations):
        selectors(selectors), declarations(declarations) {}

    void print(bool is_last_rule);
};

}

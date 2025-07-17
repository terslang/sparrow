#include "css_element.h"
using namespace std;

namespace simple_browser_css {

string Selector::to_string() {
    string ret;
    if (id.length() > 0) {
        ret += "#" + id + " ";
    }
    if (tag.length() > 0) {
        ret += tag + " ";
    }
    for (int i = 0; i < class_list.size(); ++i) {
        ret += "." + class_list[i] + " ";
    }
    ret.resize(ret.length() - 1);
    return ret;
}



bool Value::operator==(const Value& other) const {
    return data == other.data;
}

float Value::to_px() const {
    if (holds_alternative<Keyword>(data) && get<Keyword>(data).keyword == "auto")
        return 0;
    if (holds_alternative<Length>(data) && get<Length>(data).unit == "px")
        return get<Length>(data).data;
    throw simple_browser::CssParseException("Invalid value type for px conversion");
}

string Value::to_string() const {
    return visit([](auto&& arg) -> string {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, Keyword>) {
            return arg.keyword;
        } else if constexpr (is_same_v<T, Length>) {
            return std::to_string(arg.data) + arg.unit;
        } else if constexpr (is_same_v<T, Color>) {
            return std::to_string(arg.r) + " " +
                   std::to_string(arg.g) + " " +
                   std::to_string(arg.b) + " " +
                   std::to_string(arg.a);
        } else {
            throw std::runtime_error("Unknown Value type");
        }
    }, data);
}


Declaration::Declaration(const string& name, const string& kwd): 
    name(name), value(kwd) {}
Declaration::Declaration(const string& name, const tuple<float, string>& length):
    name(name), value(length) {}
Declaration::Declaration(const string& name, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color):
    name(name), value(color) {}
Declaration::Declaration(const Declaration& de):name(de.name), value(de.value) {}

string Declaration::to_string() {
    return name + ": " + value.to_string();
}

void Rule::print(bool is_last_rule) {
    for (int i = 0; i < selectors.size(); ++i) {
        if (is_last_rule && i ==  selectors.size() - 1) {
            cout << "\xE2\x94\x94\xE2\x94\x80" << selectors[i].to_string() << endl;
        } else {
            cout << "\xE2\x94\x9c\xE2\x94\x80" << selectors[i].to_string() << endl;
        }
        for (int i = 0; i < declarations.size(); ++i) {
            if (i != declarations.size() - 1) 
                cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x9c\xE2\x94\x80" << declarations[i].to_string() << endl;
            else
                cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x94\xE2\x94\x80" << declarations[i].to_string() << endl;
        }
    }
}

}
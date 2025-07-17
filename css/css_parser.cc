#include "css_parser.h"

using namespace simple_browser;
using namespace std;

namespace simple_browser_css {

// Helper function to skip CSS comments
void CssParser::skip_css_comment() {
    if (starts_with_string("/*")) {
        advance_position_string<CssParseException>("/*"); // Consume "/*"
        while (!eof() && !starts_with_string("*/")) {
            position++; // Consume characters until "*/" is found
        }
        if (eof()) {
            throw CssParseException("Unclosed CSS comment");
        }
        advance_position_string<CssParseException>("*/"); // Consume "*/"
    }
}

vector<Rule> CssParser::parse_css_rules() {
    vector<Rule> child_list;
    for(;;) {
        // Loop to skip any combination of whitespace and comments
        for (;;) {
            advance_position_loop(is_blank); // Skip whitespace
            size_t old_position = position;
            skip_css_comment(); // Skip comments
            if (position == old_position) { // No whitespace or comment was skipped
                break;
            }
        }

        if (eof()) {
            break;
        }
        try {
            child_list.push_back(parse_css_rule());
        } catch (const CssParseException& e) {
            cerr << "Warning: Skipping malformed CSS rule. " << e.what() << endl;
            // Skip to the next rule
            while (!eof() && !starts_with_string("}")) {
                position++;
            }
            if (!eof()) {
                position++; // Consume the '}'
            }
        }
    }
    return child_list;
}

Rule CssParser::parse_css_rule() {
    vector<Selector> selectors = parse_selectors();
    skip_blank_and_advance_position_string<CssParseException>("{");
    vector<Declaration> declarations;
    try {
        declarations = parse_declarations();
        skip_blank_and_advance_position_string<CssParseException>("}");
    } catch (const CssParseException& e) {
        cerr << "Warning: Malformed CSS rule. " << e.what() << endl;
        // Attempt to recover by finding the closing brace
        while (!eof() && !starts_with_string("}")) {
            position++;
        }
        if (!eof()) {
            skip_blank_and_advance_position_string<CssParseException>("}");
        }
    }
    return Rule(selectors, declarations);
}

vector<Selector> CssParser::parse_selectors() {
    vector<Selector> list;
    for(;;) {
        // Loop to skip any combination of whitespace and comments
        for (;;) {
            advance_position_loop(is_blank); // Skip whitespace
            size_t old_position = position;
            skip_css_comment(); // Skip comments
            if (position == old_position) { // No whitespace or comment was skipped
                break;
            }
        }

        if (starts_with_string(",")) {
            advance_position_string<CssParseException>(",");
        }
        // Loop to skip any combination of whitespace and comments after comma
        for (;;) {
            advance_position_loop(is_blank); // Skip whitespace
            size_t old_position = position;
            skip_css_comment(); // Skip comments
            if (position == old_position) { // No whitespace or comment was skipped
                break;
            }
        }

        if (eof() || starts_with_string("{"))
            break;
        try {
            list.push_back(parse_selector());
        } catch (const CssParseException& e) {
            cerr << "Warning: Skipping malformed CSS selector. " << e.what() << endl;
            // Skip to the next selector or declaration block
            while (!eof() && !starts_with_string(",") && !starts_with_string("{")) {
                position++;
            }
        }
    }
    return list;
}

Selector CssParser::parse_selector() {
    Selector ret;
    for (;;) {
        // Loop to skip any combination of whitespace and comments
        for (;;) {
            advance_position_loop(is_blank); // Skip whitespace
            size_t old_position = position;
            skip_css_comment(); // Skip comments
            if (position == old_position) { // No whitespace or comment was skipped
                break;
            }
        }

        if (source[position] == '.') {
            advance_position_string<CssParseException>(".");
            ret.class_list.push_back(consume_position_loop(is_identifier));
        } else if (source[position] == '#') {
            advance_position_string<CssParseException>("#");
            ret.id = consume_position_loop(is_identifier);
        } else if (is_char(source[position])) {
            ret.tag = consume_position_loop(is_identifier);
        } else if (source[position] == ',' || source[position] == '{') {
            break;
        } else {
            throw CssParseException("Unsupported selector character: " + string(1, source[position]));
        }
    }
    return ret;
}

vector<Declaration> CssParser::parse_declarations() {
    vector<Declaration> ret;
    for (;;) {
        // Loop to skip any combination of whitespace and comments
        for (;;) {
            advance_position_loop(is_blank); // Skip whitespace
            size_t old_position = position;
            skip_css_comment(); // Skip comments
            if (position == old_position) { // No whitespace or comment was skipped
                break;
            }
        }

        if (starts_with_string("}")) {
            break;
        }
        try {
            ret.push_back(parse_declaration());
            skip_blank_and_advance_position_string<CssParseException>(";");
        } catch (const CssParseException& e) {
            cerr << "Warning: Skipping malformed CSS declaration. " << e.what() << endl;
            // Skip to the next declaration
            while (!eof() && !starts_with_string(";")) {
                position++;
            }
            if (!eof()) {
                position++; // Consume the ';'
            }
        }
    }
    return ret;
}

Declaration CssParser::parse_declaration() {
    string name = skip_blank_and_consume_position(is_identifier);
    advance_position_string<CssParseException>(":");
    // Loop to skip any combination of whitespace and comments
    for (;;) {
        advance_position_loop(is_blank); // Skip whitespace
        size_t old_position = position;
        skip_css_comment(); // Skip comments
        if (position == old_position) { // No whitespace or comment was skipped
            break;
        }
    }

    if (starts_with_string("#")) { // color
        advance_position_string<CssParseException>("#");
        string color = consume_position_loop(is_hex);
        if (color.length() != 3 && color.length() != 6) {
            throw CssParseException("Invalid hex color length");
        }
        return Declaration(name, color_trans(color));
    } else if (starts_with_string("-") || (starts_with_char_predicate(is_num))) { // length
        float data = stof(consume_position_loop([](char c) -> bool {
            return (is_num(c) || c == '.' || c == '-');
        }));
        return Declaration(name, 
            make_tuple(data, skip_blank_and_consume_position(is_char)));
    } else if (starts_with_char_predicate(is_char)) {
        string keyword = consume_position_loop(is_char);
        return Declaration(name, keyword);
    } else {
        throw CssParseException("Unknown declaration starting with: " + string(1, source[position]));
    }
}

tuple<uint8_t, uint8_t, uint8_t, uint8_t> CssParser::color_trans(string& hex_color) {
    string color;
    vector<uint8_t> list;
    if (hex_color.length() == 3 ) {
        for(string::iterator it = hex_color.begin(); it != hex_color.end(); ++it) {
            color.push_back(*it);
            color.push_back(*it);
        }
    } else {
        color = hex_color;
    }
    for (string::iterator it = color.begin(); it != color.end(); ++it) {
        if (!((it - color.begin()) % 2)) {
            uint8_t val = (uint8_t)stoul(color.substr(it - color.begin(), 2), nullptr, 16);
            list.push_back(val);
        }
    }
    return make_tuple(list[0], list[1], list[2], 255);
} 

}
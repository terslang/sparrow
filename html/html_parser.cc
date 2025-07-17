#include "html_parser.h"

using namespace std;
using namespace simple_browser;

namespace simple_browser_html {

vector<DomNode> HtmlParser::parse_dom_nodes(int depth) {
    if (depth > MAX_PARSE_DEPTH) {
        cerr << "Warning: Max parse depth exceeded in parse_dom_nodes. Aborting branch." << endl;
        return {};
    }
    vector<DomNode> child_list;
    for(;;) {
        advance_position_loop(is_blank);
        if (eof() || starts_with_string("</")) {
            break;
        }
        try {
            child_list.push_back(parse_dom_node(depth + 1));
        } catch (const HtmlParseException& e) {
            cerr << "Warning: Skipping malformed HTML node. " << e.what() << endl;
            // Skip to the next node
            while (!eof() && !starts_with_string("<")) {
                consume_position_loop([](char c){ return c != '<'; });
            }
        }
    }
    return child_list;
}

DomNode HtmlParser::parse_dom_node(int depth) {
    if (depth > MAX_PARSE_DEPTH) {
        cerr << "Warning: Max parse depth exceeded in parse_dom_node. Aborting branch." << endl;
        return DomNode("", {}, {}); // Return an empty/invalid node
    }
    // Ignore doctype if present
    if (starts_with_string("<!DOCTYPE html>")) {
        advance_position_string<HtmlParseException>("<!DOCTYPE html>");
    }
    advance_position_loop(is_blank);
    if (starts_with_string("<")) {
        return element_node(depth + 1);
    } else {
        return text_node();
    }
}

DomNode HtmlParser::element_node(int depth) {
    if (depth > MAX_PARSE_DEPTH) {
        cerr << "Warning: Max parse depth exceeded in element_node. Aborting branch." << endl;
        return DomNode("", {}, {}); // Return an empty/invalid node
    }
    advance_position_string<HtmlParseException>("<");
    string tag_name = skip_blank_and_consume_position(is_identifier);
    map<string, string> attributes_map = parse_attributes();
    advance_position_loop(is_blank); // Consume any whitespace after attributes and before closing tag

    if (starts_with_string("/>")) {
        advance_position_string<HtmlParseException>("/>");
        return DomNode(tag_name, attributes_map, {}); // Self-closing tag, no children
    } else if (starts_with_string(">")) {
        advance_position_string<HtmlParseException>(">");
        if (is_self_closing(tag_name)) {
            // HTML5 self-closing tag, no children or closing tag expected
            return DomNode(tag_name, attributes_map, {});
        }
    } else {
        throw HtmlParseException("Malformed tag: expected '>' or '/>'");
    }

    vector<DomNode> child_list;
    try {
        child_list = parse_dom_nodes(depth + 1);
        advance_position_string<HtmlParseException>("</" + tag_name + ">");
    } catch (const HtmlParseException& e) {
        cerr << "Warning: Malformed element '" << tag_name << "'. " << e.what() << endl;
        // Attempt to recover by finding the closing tag
        while (!eof() && !starts_with_string("</" + tag_name + ">")) {
            position++;
        }
        if (!eof()) {
            advance_position_string<HtmlParseException>("</" + tag_name + ">");
        }
    }

    return DomNode(tag_name, attributes_map, child_list);
}

map<string, string> HtmlParser::parse_attributes() {
    map<string, string> ret;
    for (;;) {
        advance_position_loop(is_blank);
        if (eof() || starts_with_string(">") || starts_with_string("/>")) {
            break;
        }
        string attri_name = skip_blank_and_consume_position(is_char);
        if (attri_name.empty()) {
            // If no attribute name was consumed, it's likely a malformed tag or end of attributes
            // Break to avoid infinite loop
            break;
        }

        advance_position_loop(is_blank); // Consume any spaces after attribute name

        if (starts_with_string("=")) {
            // It's a key-value attribute
            advance_position_string<HtmlParseException>("=");
            advance_position_loop(is_blank);
            char quote = source[position];
            if (quote != '\'' && quote != '"') {
                // Malformed attribute value, try to skip to next attribute or tag end
                cerr << "Warning: Malformed attribute value for '" << attri_name << "'. Expected quote, found: " << quote << endl;
                // Skip until next space, quote, or tag end
                consume_position_loop([](char c){ return c != ' ' && c != '\'' && c != '"' && c != '>' && c != '<'; });
                continue; // Try next attribute
            }
            advance_position_string<HtmlParseException>(string(1, quote));
            string value = consume_position_loop([=](char c) -> bool {
                return (c != quote);
            });
            advance_position_string<HtmlParseException>(string(1, quote));
            ret.insert(make_pair(attri_name, value));
        } else {
            // It's a boolean attribute or malformed. Add with empty value.
            ret.insert(make_pair(attri_name, ""));
            // No advance_position_string here, as no '=' or quotes were consumed.
            // The loop will continue and check for '>' or next attribute.
        }
    }
    return ret;
}

DomNode HtmlParser::text_node() {
    string text = consume_position_loop([](char c) -> bool {
        if (c == '>' || c == '<') {
            return false;
        } else {
            return true;
        }
    });
    // Trim leading/trailing whitespace and replace multiple spaces with a single space
    string trimmed_text;
    bool last_char_was_space = false;
    for (char c : text) {
        if (isspace(c)) {
            if (!last_char_was_space) {
                trimmed_text += ' ';
                last_char_was_space = true;
            }
        } else {
            trimmed_text += c;
            last_char_was_space = false;
        }
    }
    if (!trimmed_text.empty() && trimmed_text[0] == ' ') {
        trimmed_text.erase(0, 1);
    }
    if (!trimmed_text.empty() && trimmed_text.back() == ' ') {
        trimmed_text.pop_back();
    }
    return DomNode(trimmed_text);
}

bool HtmlParser::is_self_closing(const string& tag_name) const {
    for (const string& self_closing_tag : self_closing_tags) {
        if (tag_name == self_closing_tag) {
            return true;
        }
    }
    return false;
}
}
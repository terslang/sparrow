#pragma once
#include "../utils/base_parser.h"
#include "css_element.h"
#include <iostream>

using namespace simple_browser;

namespace simple_browser_css {

class CssParser: public BaseParser {

    public:
    CssParser(std::string& source): BaseParser(source) {}

    std::vector<Rule> parse_css_rules();
    Rule parse_css_rule();
    std::vector<Selector> parse_selectors();
    Selector parse_selector();
    std::vector<Declaration> parse_declarations();
    Declaration parse_declaration();
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color_trans(std::string& hex_color);
};
}
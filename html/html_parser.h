#pragma once
#include <string>
#include <vector>
#include "dom.h"
#include "../utils/base_parser.h"

using namespace simple_browser;

namespace simple_browser_html {

class HtmlParser: public BaseParser {

    public:
    HtmlParser(const std::string& source): BaseParser(source) {}

    std::vector<DomNode> parse_dom_nodes();
    DomNode parse_dom_node();
    DomNode element_node();
    std::map<std::string, std::string> parse_attributes();
    DomNode text_node();
};

}
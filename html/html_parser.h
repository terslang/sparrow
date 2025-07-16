#pragma once
#include <string>
#include <vector>
#include "../utils/base_parser.h"
#include "../utils/exceptions.h"
#include "dom.h"

using namespace std;
using namespace simple_browser;

namespace simple_browser_html {

class HtmlParser: public BaseParser {

    public:
    HtmlParser(const string& source): BaseParser(source) {}

    vector<DomNode> parse_dom_nodes();
    DomNode parse_dom_node();
    DomNode element_node();
    map<string, string> parse_attributes();
    DomNode text_node();
};

}
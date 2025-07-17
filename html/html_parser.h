#pragma once
#include <string>
#include <vector>
#include "../utils/base_parser.h"
#include "../utils/exceptions.h"
#include "dom.h"

using namespace std;
using namespace simple_browser;

namespace simple_browser_html {

#define MAX_PARSE_DEPTH 64

class HtmlParser: public BaseParser {

    public:
    HtmlParser(const string& source): BaseParser(source) {}

    vector<DomNode> parse_dom_nodes(int depth = 0);
    DomNode parse_dom_node(int depth = 0);
    DomNode element_node(int depth = 0);
    map<string, string> parse_attributes();
    DomNode text_node();

    private:
    bool is_self_closing(const string& tag_name) const;
    const vector<string> self_closing_tags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr"};
    void skip_html_comment();
};

}

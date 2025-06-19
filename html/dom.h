#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <iostream>

namespace simple_browser_html {

enum DomNodeType {
    ELEMENT,
    TEXT
};

class DomNode {
    public:
    DomNodeType type;
    std::vector<DomNode> child_list;
    std::string text;
    std::string tag_name;
    std::map<std::string, std::string> attributes;

    DomNode(){}
    DomNode(const std::string& text): text(text), type(TEXT) {}
    DomNode(const std::string& tag_name, const std::map<std::string, std::string>& attributes, const std::vector<DomNode>& child_list):
        tag_name(tag_name), attributes(attributes), type(ELEMENT), child_list(child_list) {}
    DomNode(const DomNode& dn) {
        switch (dn.type) {
            case TEXT:
                new(this) DomNode(dn.text);
                return;
            case ELEMENT:
                new(this) DomNode(dn.tag_name, dn.attributes, dn.child_list);
                return;
        }
    }
    ~DomNode() {}
    
    void print(int32_t depth, bool is_last_child, std::vector<int32_t>& list) const;
};

void print_dom_node(const DomNode& root, bool is_last_child);
}
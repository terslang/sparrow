#include "style_node.h"
#include "../utils/dom_elements.h"
#include "../utils/font_manager.h"
#include <assert.h>

using namespace std;
using namespace simple_browser_html;
using namespace simple_browser_css;

namespace simple_browser_style {

bool StyleDomNode::match_selector(const Selector& selector, int &weight) {
    weight = 0;

    // tag of dom node
    if ((selector.tag != "") && (tag_name == selector.tag)) {
        weight += 1;
    }

    vector<char> delimits;
    delimits.push_back(' ');
    delimits.push_back('\t');

    // class of dom node
    if (selector.class_list.size() > 0) {
        int tmp = 0;
        auto it = attributes.find("class");
        if (it != attributes.end()) {
            vector<string> attributes_class = split_string(it->second, delimits);
            for (auto iit = selector.class_list.begin(); iit != selector.class_list.end(); ++iit) {
                if (in_vector(attributes_class, *iit, string_equal)) {
                    tmp += 10;
                } else {
                    goto out;
                }
            }
        }
        weight += tmp;
    out:
        weight += 0;
    }

    //id of dom node
    if (selector.id != "" &&
        exist_in_map<string, string, decltype(string_equal)>
            (attributes, "id", selector.id, string_equal)) {
        weight += 100;
    }

    return weight > 0;
}

void StyleDomNode::trans_style(const vector<Rule>& rules) {
    if (tag_name == "title") {
        property_map.insert_or_assign("display", Value("none"));
    }

    // Store properties with their weights
    map<string, pair<Value, int>> final_properties; // property_name -> {value, weight}

    for (const auto& rule : rules) { // Iterate through rules in order (user-agent then author)
        for (const auto& selector : rule.selectors) {
            int current_weight = 0;
            if (match_selector(selector, current_weight)) {
                for (const auto& declaration : rule.declarations) {
                    auto it = final_properties.find(declaration.name);
                    if (it == final_properties.end() || current_weight >= it->second.second) {
                        // If property not set, or current rule has higher/equal specificity
                        final_properties[declaration.name] = make_pair(declaration.value, current_weight);
                    }
                }
            }
        }
    }

    // Transfer final properties to property_map
    for (const auto& entry : final_properties) {
        property_map.insert_or_assign(entry.first, entry.second.first);
    }
    
/////////for test
    if (type == TEXT) {
        FontManager font_manager;
        if (const Value *tmp = find_in_map(property_map, string("font-size"))) {
            font_manager.set_font_size(tmp->to_px());
        }
        u32string u32text = font_manager.convert_to_u32string(text);
        tuple<int, int> wh_size = font_manager.get_string_width_length(u32text);
        property_map.insert(make_pair("width", Value(make_tuple(get<0>(wh_size), "px"))));
        property_map.insert(make_pair("height", Value(make_tuple(get<1>(wh_size), "px"))));
    }
}

void StyleDomNode::print_property_map(int32_t depth, bool is_last_child, vector<int32_t>& list) {
    for (map<string, Value>::iterator it = property_map.begin(); it != property_map.end(); ++it) {
        for (int32_t i = 0; i < depth; ++i) {
            if (in_vector(list, i, [](int32_t a, int32_t b) -> bool { return a == b; })) {
                std::cout << "\xE2\x94\x82 ";
            }
            else 
                std::cout << "  ";
        }
        cout << it->first << ": " << it->second.to_string() << endl;
    }
}

string StyleDomNode::display() const {
    const Value* value = find_in_map(property_map, string("display"));
    if (value) {
        if (holds_alternative<Keyword>(value->data)) {
            const Keyword& keyword_val = get<Keyword>(value->data);
            if (keyword_val.keyword == "block" || keyword_val.keyword == "none" ||
                keyword_val.keyword == "inline") {
                return keyword_val.keyword;
            }
        }
        assert(false); // wrong display property
    }
    if (in_vector(block_elements, tag_name, string_equal)) {
        return string("block");
    }
    if (in_vector(inline_elements, tag_name, string_equal)) {
        return string("inline");
    }
    return "inline";
}

void style_dom_node_print(StyleDomNode& node, bool is_last_child) {
    static int32_t depth = 0;
    static vector<int32_t> list;

    ++depth;
    node.print(depth, is_last_child, list);
    node.print_property_map(depth, is_last_child, list);

    for (vector<StyleDomNode>::iterator it = node.child_sdn_list.begin();
        it != node.child_sdn_list.end(); ++it) {
        if (it == node.child_sdn_list.begin()) {
            list.push_back(depth);
        }
        if (it == node.child_sdn_list.end() - 1) {
            list.pop_back();
        }
        style_dom_node_print(*it, it == node.child_sdn_list.end() - 1);
    }
    --depth;
    }

}


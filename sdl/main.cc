#include "../html/html_parser.h"
#include "../css/css_parser.h"
#include "../style/style_node_parser.h"
#include "../layout/layout_node.h"
#include "sdl_drawer.h"
#include "sdl_drawer_interface.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;


vector<string> collect_css_links(const simple_browser_html::DomNode& node) {
    vector<string> css_links;
    if (node.type == simple_browser_html::ELEMENT && node.tag_name == "link") {
        auto rel_it = node.attributes.find("rel");
        auto href_it = node.attributes.find("href");
        if (rel_it != node.attributes.end() && rel_it->second == "stylesheet" &&
            href_it != node.attributes.end()) {
            css_links.push_back(href_it->second);
        }
    }
    for (const auto& child : node.child_list) {
        vector<string> child_links = collect_css_links(child);
        css_links.insert(css_links.end(), child_links.begin(), child_links.end());
    }
    return css_links;
}

void update_layout_and_draw(simple_browser_sdldrawer::SdlDrawerInterface& drawer_interface,
                            simple_browser_sdldrawer::SdlDrawer& sdlDrawer,
                            simple_browser_html::DomNode& domNode,
                            vector<simple_browser_css::Rule>& rules,
                            int window_width, int window_height) {
    drawer_interface.clear_drawing_lists();
    simple_browser_style::StyleDomNodeParser styleParser(domNode, rules);
    simple_browser_layout::LayoutNode root = simple_browser_layout::combine_style_dom(
        styleParser.parse_style_dom_node(styleParser.domNode, styleParser.rules, nullptr));

    styleParser.print();

    simple_browser_layout::Box canvas;
    canvas.content.x = 0;
    canvas.content.y = 0;
    canvas.content.width = window_width;
    canvas.content.height = 0;

    Fake_Box fake_box {
        .width = 0,
        .height = 0,
        .pen_x = 0,
        .pen_y = 0,
    };

    float line_height = 0;
    simple_browser_layout::layout_block_node(root, canvas, fake_box, line_height);
    simple_browser_layout::layout_node_print(root, true);

    drawer_interface.iterate_layout_tree(root);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "usage: ./sparrow <html_file>" << endl;
        return -1;
    }
    ifstream file_html(argv[1]);
    if (!file_html) {
        cout << "open html file error" << endl;
        return -1;
    }
    stringstream ss;
    ss << file_html.rdbuf();
    string html_source = ss.str();
    simple_browser_html::HtmlParser htmlParser(html_source);
    simple_browser_html::DomNode domNode;
    try {
        domNode = htmlParser.parse_dom_node(0);
    } catch (const simple_browser::SparrowException& e) {
        cerr << "Failed to parse HTML: " << e.what() << endl;
    }
    vector<simple_browser_css::Rule> rules;

    vector<string> css_links = collect_css_links(domNode);
    for (const string& css_link : css_links) {
        string html_file_path = argv[1];
        string html_dir = html_file_path.substr(0, html_file_path.find_last_of("/") + 1);
        string css_file_path = html_dir + css_link;

        ifstream file_css(css_file_path);
        if (!file_css) {
            cerr << "Failed to open CSS file: " << css_file_path << endl;
            continue;
        }
        stringstream ss_css;
        ss_css << file_css.rdbuf();
        string css_source = ss_css.str();
        simple_browser_css::CssParser cssParser(css_source);
        try {
            vector<simple_browser_css::Rule> new_rules = cssParser.parse_css_rules();
            rules.insert(rules.end(), new_rules.begin(), new_rules.end());
        } catch (const simple_browser::SparrowException& e) {
            cerr << "Failed to parse CSS from " << css_file_path << ": " << e.what() << endl;
        }
    }
    
    simple_browser_sdldrawer::SdlDrawer sdlDrawer(WINDOW_WIDTH, WINDOW_HEIGHT);
    bool status = sdlDrawer.init_sdldrawer();
    if (!status) {
        cout << "init sdldrawer error!" << endl;
        return -1;
    }
    FontManager fontManager;
    simple_browser_sdldrawer::SdlDrawerInterface drawer_interface(fontManager, sdlDrawer.res.render);

    update_layout_and_draw(drawer_interface, sdlDrawer, domNode, rules, WINDOW_WIDTH, WINDOW_HEIGHT);

    sdlDrawer.run_sdldrawer(drawer_interface, [&](int width, int height) {
        update_layout_and_draw(drawer_interface, sdlDrawer, domNode, rules, width, height);
    });

    return 0;
}
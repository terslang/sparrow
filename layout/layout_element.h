#pragma once
#include <cstring>
#include <string>
namespace simple_browser_layout {

struct Rect {
    float x;
    float y;
    float width;
    float height;

    std::string to_string() {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%f %f %f %f", x, y, width, height);
        return std::string(buf);
    }
};

struct Edge {
    float left;
    float right;
    float top;
    float bottom;

    std::string to_string() {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%f %f %f %f", left, right, top, bottom);
        return std::string(buf);
    }
};

struct Box {
    struct Rect content;
    struct Edge padding;
    struct Edge border;
    struct Edge margin;

    std::string get_content() {
        return "content-xywh: " + content.to_string();
    }
    std::string get_padding() {
        return "padding-lrtb: " + padding.to_string();
    }
    std::string get_border() {
        return "border-lrtb: " + border.to_string();
    }
    std::string get_margin() {
        return "margin-lrtb: " + margin.to_string();
    }
};

enum BoxTypeEnum {
    BLOCK,
    INLINE,
    ANONYMOUS,
};


}
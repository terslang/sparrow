
#pragma once
#include <stdexcept>
#include <string>

namespace simple_browser {

class SparrowException : public std::exception {
public:
    SparrowException(const std::string& message) : msg(message) {}
    virtual const char* what() const noexcept override {
        return msg.c_str();
    }
protected:
    std::string msg;
};

class BaseException : public SparrowException {
public:
    BaseException(const std::string& message) : SparrowException(message) {}
};

class HtmlParseException : public SparrowException {
public:
    HtmlParseException(const std::string& message) : SparrowException(message) {}
};

class CssParseException : public SparrowException {
public:
    CssParseException(const std::string& message) : SparrowException(message) {}
};

}

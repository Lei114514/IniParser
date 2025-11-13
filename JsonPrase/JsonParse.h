#pragma once

#include "Json.h"

class JsonParse
{
public: 
    Json parse(const std::string& text);
private:
    std::string text_;
    size_t index_;

    void skip_whitespace();
    char peek() const;
    char advance();

    Json parseValue();
    Json parseNull();
    Json parseBool();
    Json parseNumber();
    Json parseString();
    Json parseArray();
    Json parseObject();
};
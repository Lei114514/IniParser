#include <exception>
#include <stdexcept>
#include <variant>
#include <string>
#include <tuple>

#ifdef DEBUG
#include <stdio.h>
#include <iostream>
#endif

#include "Json.h"
#include "JsonParse.h"

Json JsonParse::parse(const std::string& text)
{
    text_=text;
    index_=0;
    return parseValue();
}

void JsonParse::skip_whitespace()
{
    index_ = text_.find_first_not_of(" \t\r\n",index_);
}

char JsonParse::peek() const
{
    return text_[index_];
}

char JsonParse::advance()
{
    return text_[index_++];
}

Json JsonParse::parseValue()
{
    skip_whitespace();
    char currentChar=peek();
    #ifdef DEBUG
    printf("index=%d, char=%c\n",index_,currentChar);
    #endif

    switch (currentChar)
    {
    case 'n': return parseNull();
    case 't': 
    case 'f': return parseBool();
    case '"': return parseString();
    case '{': return parseObject();
    case '[': return parseArray();
    default:
        if(isdigit(currentChar)||currentChar=='-')
        {
            return parseNumber();
        }
        else 
        {
            throw std::runtime_error("Invaild Json Value");
        }
    }
}

Json JsonParse::parseNull()
{
    std::string str{};
    for(int i=0;i<4;++i) str+=advance();
    if(str=="null"){
        return Json(nullptr);
    }
    else 
    {
        throw std::runtime_error("Invaild Null Value");
    }
}

Json JsonParse::parseBool()
{
    std::string str{};
    char CurrentChar=peek();
    if(CurrentChar=='t')
    {
        for(int i=0;i<4;++i) str+=advance();
    }
    else 
    {
        for(int i=0;i<5;++i) str+=advance();
    }

    if(str=="false") {
        return Json(false);
    }
    else if(str=="true"){
        return Json(true);
    }
    else {
        throw std::runtime_error("Invaild Bool Value");
    }
}

Json JsonParse::parseNumber()
{
    bool isNegative=false;
    if(peek()=='-') {
        isNegative=true;
        advance();
    }

    std::string number{};
    while(isdigit(peek()))
    {
        number+=advance();
    }

    if(number.empty()) 
    {
        throw std::runtime_error("Invaild Number Error");
    }
    
    auto getIntegerValue=[](const std::string& number)->int
    {
        int value=0;
        for(int i=0;i<number.size();++i) {
            value=value*10+number[i]-'0';
        }
        return value;
    };
    auto getDecimalValue=[](const std::string& number)->double
    {
        double value=0;
        for(int i=number.size()-1;i>=0;i--) {
            value=value/10+number[i]-'0';
        }
        return value;
    };

    size_t posIndex=number.find('.');
    if(posIndex!=std::string::npos)
    {
        double value{0};
        value+=getIntegerValue(number.substr(0,posIndex));
        value+=getDecimalValue(number.substr(posIndex+1));
        return Json(value);
    }
    else
    {
        int value{0};
        value+=getIntegerValue(number);
        return Json(value);
    }
}

Json JsonParse::parseString()
{
    advance();
    std::string str;
    while(peek()!='"')
    {
        str+=advance();
    }
    advance();
    return Json(str);
}

Json JsonParse::parseArray()
{
    advance();
    std::vector<Json> result{};
    while(peek()!=']')
    {
        result.push_back(parseValue());
        skip_whitespace();
        if(peek()==',') advance();
        else if(peek()!=']')
        {
            throw std::runtime_error("Expected ',' or ']' in object");
        }
    }
    advance();
    return Json(result);
}

Json JsonParse::parseObject()
{
    advance();
    std::map<std::string,Json> jsons;

    while(peek()!='}')
    {
        skip_whitespace();
        std::string key=std::get<std::string>(parseValue().data());
        #ifdef DEBUG
        std::cout<<key<<std::endl;
        #endif
        skip_whitespace();
        if(peek()!=':')
        {
            throw std::runtime_error("Expected ':' in object");
        }
        advance();
        skip_whitespace();

        Json value=parseValue();
        jsons[key]=value;

        skip_whitespace();
        if(peek()==',') {
            advance();
        }
        else if(peek()!='}'){
            #ifdef DEBUG
            std::cout<<peek()<<std::endl;
            #endif
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }

    advance();
    return Json(jsons);
}
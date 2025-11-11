#include <string.h>
#include <unistd.h>
#include <fstream>
#include <string>

#include "IniParse.h"

IniParse::ParseResult IniParse::parse(const std::string& fileName)
{
    data_.clear();
    std::ifstream file(fileName);
    if(!file.is_open())
    {
        return ParseResult::kErrorFileOpenFailed;
    }

    std::string currentSection{};
    std::string line;
    while(std::getline(file,line))
    {   
        trim(line);
        if(line.empty()||line[0]=='#'||line[0]==';') continue;
        
        if(line[0]=='[')
        {
            size_t index = line.find(']');
            if(index != line.length()-1 )
            {
                return ParseResult::kErrorMalformedSection;
            }

            currentSection=line.substr(1,line.length()-2);
            trim(currentSection);

            if(currentSection.empty())
            {
                return ParseResult::kErrorMalformedSection;
            }

            data_[currentSection];
        }
        else 
        {
            size_t index=line.find('=');
            if(index!=line.rfind('=')) 
            {
                return ParseResult::kErrorUnknowLineFormat;
            }   
            if(currentSection.empty())
            {
                return ParseResult::kErrorMalformedKeyValuePair;
            }

            std::string key=line.substr(0,index);
            std::string value=line.substr(index+1);
            trim(key);
            trim(value);
            if(key.empty()||value.empty())
            {
                return ParseResult::kErrorFileOpenFailed;
            }
            data_[currentSection][key]=value;

        }
    }

    return ParseResult::kSuccess;
}

void IniParse::trim(std::string& line) const 
{
    line.erase(0,line.find_first_not_of(" \t\r\n"));
    line.erase(line.find_last_not_of(" \t\r\n")+1);
}

std::optional<std::string> IniParse::getString(const std::string& section,const std::string& key) const
{
    auto x=data_.find(section);
    if(x==data_.end())
    {
        return std::nullopt;
    }

    auto y=x->second.find(key);
    if(y==x->second.end())
    {
        return std::nullopt;
    }

    return y->second;
}

std::optional<std::map<std::string,std::string>> IniParse::getSection(const std::string& section) const
{
    auto x=data_.find(section);
    if(x==data_.end())
    {
        return std::nullopt;
    }
    return x->second;
}

const std::map<std::string,std::map<std::string,std::string>>& IniParse::getAllSections() const
{
    return data_;
}
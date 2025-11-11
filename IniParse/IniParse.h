#pragma once

#include <map>
#include <string>
#include <optional>

class IniParse
{
public:
    using Section = std::map<std::string,std::string>;

    enum class ParseResult{
        kSuccess,
        kErrorFileOpenFailed,
        kErrorMalformedSection,
        kErrorMalformedKeyValuePair,
        kErrorUnknowLineFormat,
    };

    IniParse()=default;
    ~IniParse()=default;

    ParseResult parse(const std::string& fileName);

    std::optional<std::string> getString(const std::string& section,const std::string& key) const;
    std::optional<Section> getSection(const std::string& section) const;
    const std::map<std::string,Section>& getAllSections() const;

private:
    void trim(std::string& line) const ;
    std::map<std::string,Section> data_;
};
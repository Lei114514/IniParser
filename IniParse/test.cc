#include "IniParse.h"
#include <iostream>

int main()
{
    IniParse iniParse;
    IniParse::ParseResult err=iniParse.parse("sample.ini");
    if(err!=IniParse::ParseResult::kSuccess)
    {
        std::cout<<static_cast<int>(err)<<std::endl;
    }
    else 
    {
        std::cout<<"success\n";
    }
    auto data=iniParse.getAllSections();
    for(auto [x,map_]:data)
    {
        std::cout<<"section:"<<x<<std::endl;
        for(auto [key,value]:map_)
        {
            std::cout<<key<<' '<<value<<std::endl;
        }
    }

    std::cout<<"--------------------------------------------\n";
    auto data2=iniParse.getSection("server");
    if(data2==std::nullopt)
    {
        std::cout<<"data2 is nullopt\n";
    }
    else 
    {
        auto data3=data2.value();
        for(auto [x,y]:data3)
        {
            std::cout<<x<<' '<<y<<std::endl;
        }
    }

    std::cout<<"--------------------------------------------\n";
    auto data4=iniParse.getString("server","port");
    if(data4==std::nullopt)
    {
        std::cout<<"data4 is nullopt\n";
    }
    else 
    {
        std::cout<<data4.value()<<std::endl;
    }
    return 0;
}
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "JsonParse.h"

using JsonObject=std::map<std::string,Json>;
using JsonArray=std::vector<Json>;
using JsonValue=std::variant<
    std::nullptr_t,
    int,
    bool,
    double,
    std::string,
    JsonObject,
    JsonArray
>;

void expandData(JsonValue data, int cnt)
{
    if(std::holds_alternative<std::vector<Json>>(data))
    {
        for(auto x:std::get<std::vector<Json>>(data)) 
        {
            expandData(JsonValue(x.data()),cnt+1);
            std::cout<<",";
        }
    }
    else if(std::holds_alternative<std::map<std::string,Json>>(data))
    {
        for(auto [x,y]:std::get<std::map<std::string,Json>>(data))
        {
            for(int i=0;i<cnt;++i) std::cout<<"  ";
            std::cout<<x<<":";
            expandData(y.data(),cnt+1);
            std::cout<<std::endl;
        }
    }
    else if(std::holds_alternative<int>(data))
    {
        std::cout<<std::get<int>(data);
    }
    else if(std::holds_alternative<bool>(data))
    {
        std::cout<<std::get<bool>(data);
    }
    else if(std::holds_alternative<double>(data))
    {
        std::cout<<std::get<double>(data);
    }
    else if(std::holds_alternative<std::string>(data))
    {
        std::cout<<std::get<std::string>(data);
    }
        else if(std::holds_alternative<std::nullptr_t>(data))
    {
        std::cout<<"null";
    }
}

int main()
{
    char buf[1024];
    int fd=::open("sampleJson.json",O_RDONLY);
    if(fd<0)
    {
        perror("open error");
    }

    if(read(fd,buf,sizeof(buf))<0)
    {
        perror("read error");
    }

    JsonParse parse;
    Json json=parse.parse(buf);

    ::close(fd);

    auto data=json.data();

    expandData(data,0);

    return  0;
}
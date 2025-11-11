#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "IniParse.h"

int IniParse::parse(std::string fileName)
{
    int fd=open(fileName.c_str(),O_RDONLY);
    if(fd==-1)
    {
        return 1;
    }

    char buf[1024];
    
    bool isSection=false,completion=false;
    std::string section;
    int isKey=0;
    std::string key,value;
    bool isNote=false;

    while(true)
    {
        ::memset(buf,0,sizeof(buf));
        int n=::read(fd,buf,sizeof(buf));

        if(n==0) break;
        if(n<0) return 2;

        for(int index=0;index<n;++index)
        {
            char alphabet = buf[index];
            switch (alphabet)
            {
            case '[':
                section.clear();
                isSection=true;
                completion=false;
                break;
            case ']':
                isSection=false;
                completion=true;
                data_[section]=std::map<std::string,std::string>{};
                break;
            case '#':
            case ';':
                isNote=true;
                break;
            case '=':
                ++isKey;
                if(isKey==2) 
                {
                    return 3;
                }
                break;
            case '\n':
                if(isKey==1)
                {
                    data_[section][key]=value;
                    key.clear();
                    value.clear();
                }
                /*else if(!isSection&&!isNote&&value.empty())
                {
                    return 4;
                }*/
                isSection=false;
                completion=false;
                isKey=0;
                isNote=false;
                break;
            case ' ':
                break;
            default:
                if(isNote) continue;
                else if(completion) return 5;
                else if(isSection) section+=alphabet;
                else if(isKey==0) key+=alphabet;
                else if(isKey==1) value+=alphabet;
                break;
            }
        }
    }

    //收尾
    if(!key.empty()&&!value.empty())
    {
        data_[section][key]=value;
    }

    return 0;
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
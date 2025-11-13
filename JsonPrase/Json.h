#pragma once
#include <map>
#include <vector>
#include <string>
#include <variant>

class Json
{
public:
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

    Json():data_(nullptr){};
    Json(JsonValue data):data_(std::move(data)){};
    JsonValue data() { return data_; }
private:
    JsonValue data_;
};
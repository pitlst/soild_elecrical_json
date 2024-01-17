// 以下是对应函数的具体实现
#include "json.hpp"

#include <typeinfo>
#include <type_traits>

using namespace swq;

json::json() {}
json::json(bool input_value)
{
    __value->emplace<bool>(input_value);
}
json::json(int input_value)
{
    __value->emplace<int>(input_value);
}
json::json(double input_value)
{
    __value->emplace<double>(input_value);
}
json::json(const json &input_value)
{
    __value = input_value.__value;
}
json::json(std::string_view input_value)
{
    __value->emplace<std::string>(input_value);
}
json::json(const std::vector<json> &input_value)
{
    __value->emplace<std::vector<json>>(input_value);
}
json::json(const std::unordered_map<std::string, json> &input_value)
{
    __value->emplace<std::unordered_map<std::string, json>>(input_value);
}

json json::operator=(const json &input_value)
{
    this->__value = input_value.__value;
    return *this;
}
json json::operator[](std::size_t index)
{
    return std::visit([index](const auto &val) -> json
                      {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::vector<json>>)
        {
            return val.at(index);
        }
        else
        {
            throw "json type is not array";
        } },
                      *__value);
}
json json::operator[](const std::string &key)
{
    return std::visit([key](const auto &val) -> json
                      {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            return val.at(key);
        }
        else
        {
            throw "json type is not object";
        } },
                      *__value);
}

json json::array(const std::initializer_list<json> &input_value)
{
    json temp(input_value);
    return temp;
}
json json::object(const std::unordered_map<std::string, json> &input_value)
{
    json temp(input_value);
    return temp;
}

bool json::to_bool() const
{
    return std::visit([](auto &val) -> bool
                      {
        bool temp_value = false;
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, bool>)
        {
            temp_value = val;
        }
        else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>)
        {
            temp_value = bool(val);
        }
        return temp_value; },
                      *__value);
}
int json::to_int() const
{
    return std::visit([](auto &val) -> int
                      {
        int temp_value = 0;
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, double>)
        {
            temp_value = int(val);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            temp_value = val;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            temp_value = std::stoi(val);
        }
        return temp_value; },
                      *__value);
}
double json::to_double() const
{
    return std::visit([](auto &val) -> double
                      {
        double temp_value = 0;
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, int>)
        {
            temp_value = double(val);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            temp_value = val;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            temp_value = std::stod(val);
        }
        return temp_value; },
                      *__value);
}

std::string json::to_str() const
{
    return std::visit([](auto &val) -> std::string
                      {
        std::string temp_value;
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::monostate>)
        {
            temp_value = "NULL";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            if (val)
            {
                temp_value = "true";
            }
            else
            {
                temp_value = "false";
            }
        }
        else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>)
        {
            temp_value = std::to_string(val);
        }
        else if constexpr (std::is_same_v<T, std::vector<json>>)
        {
            temp_value = "[";
            bool first = false;
            for (auto &ch : val)
            {
                if(first)
                {
                    temp_value += ",";
                }
                temp_value += ch.to_str();
                first = true;
            }
            temp_value += "]";
        }
        else if constexpr (std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            temp_value = "{";
            bool first = false;
            for (auto &ch : val)
            {
                if(first)
                {
                    temp_value += ",";
                }
                temp_value += (ch.first + ":" + ch.second.to_str());
                first = true;
            }
            temp_value += "}";
        }
        return temp_value; },
                      *__value);
}

bool json::is_null() const
{
    return is_type("null");
}
bool json::is_bool() const
{
    return is_type("bool");
}
bool json::is_int() const
{
    return is_type("int");
}
bool json::is_double() const
{
    return is_type("double");
}
bool json::is_string() const
{
    return is_type("string");
}
bool json::is_array() const
{
    return is_type("array");
}
bool json::is_object() const
{
    return is_type("object");
}

bool json::is_type(std::string_view input_type_name) const
{
    return std::visit([input_type_name](const auto &val) -> bool
                      {
        bool label = false;
        using T = std::decay_t<decltype(val)>;
        if(input_type_name == "null")
        {
            if constexpr (std::is_same_v<T, std::monostate>)
            {
                label = true;
            }
        }
        else if(input_type_name == "bool")
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                label = true;
            }
        }
        else if(input_type_name == "int")
        {
            if constexpr (std::is_same_v<T, int>)
            {
                label = true;
            }
        }
        else if(input_type_name == "double" || input_type_name == "float")
        {
            if constexpr (std::is_same_v<T, double>)
            {
                label = true;
            }
        }
        else if(input_type_name == "string" || input_type_name == "str")
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                label = true;
            }
        }
        else if(input_type_name == "array")
        {
            if constexpr (std::is_same_v<T, std::vector<json>>)
            {
                label = true;
            }
        }
        else if(input_type_name == "object")
        {
            if constexpr (std::is_same_v<T, std::unordered_map<std::string, json>>)
            {
                label = true;
            }
        }
        return label; },
                      *__value);
}

bool json::has(int index) const
{
    return std::visit([index](const auto &val) -> bool
                      {
        bool label = false;
        using T = std::decay_t<decltype(val)>;
        if constexpr(std::is_same_v<T, std::vector<json>>)
        {
            if (index < val.size() && index >= 0)
            {
                label = true;
            }
        }   
        return label; },
                      *__value);
}

bool json::has(const std::string &key) const
{
    return std::visit([key](const auto &val) -> bool
                      {
        bool label = false;
        using T = std::decay_t<decltype(val)>;
        if constexpr(std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            if(val.count(key))
            {
                label = true;
            }
        }   
        return label; },
                      *__value);
}

bool json::empty() const
{
    return std::visit([](const auto &val) -> bool
                      {
        bool label = false;
        using T = std::decay_t<decltype(val)>;
        if constexpr(std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, double>)
        {
            label = true;
        }
        else if constexpr(std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<json>> || std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            label = val.empty();
        }   
        return label; },
                      *__value);
}

std::size_t json::size() const
{
    return std::visit([](const auto &val) -> std::size_t
                      {
        std::size_t temp_size = 0;
        using T = std::decay_t<decltype(val)>;
        if constexpr(std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<json>> || std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            temp_size = val.size();
        }   
        return temp_size; },
                      *__value);
}
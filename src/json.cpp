// 以下是对应函数的具体实现
#include "json.hpp"

#include <typeinfo>
#include <type_traits>

using namespace swq;

json::json() {}
json::json(bool input_value)
{
    __value.emplace<bool>(input_value);
}
json::json(int input_value)
{
    __value.emplace<int>(input_value);
}
json::json(double input_value)
{
    __value.emplace<double>(input_value);
}
json::json(const json &input_value)
{
    __value = input_value.__value;
}
json::json(const char *input_value)
{
    __value.emplace<std::string>(std::string(input_value));
}
json::json(const std::string &input_value)
{
    __value.emplace<std::string>(input_value);
}
json::json(const std::vector<json> &input_value)
{
    __value.emplace<std::vector<json>>(input_value);
}
json::json(const std::map<std::string, json> &input_value)
{
    __value.emplace<std::map<std::string, json>>(input_value);
}


json json::operator=(const json &input_value)
{
    this->__value = input_value.__value;
    return *this;
}

json json::array(const std::initializer_list<json> &input_value)
{
    std::vector<json> temp_(input_value);
    json temp(temp_);
    return temp;
}
json json::object(const std::map<std::string, json> &input_value)
{
    json temp(input_value);
    return temp;
}


bool json::to_bool()
{
    return std::visit([](auto &val)
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
        return temp_value;
    }, __value);
}
int json::to_int()
{
    return std::visit([](auto &val)
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
        return temp_value;
    }, __value);
}
double json::to_double()
{
    return std::visit([](auto &val)
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
        return temp_value;
    }, __value);
}

std::string json::to_str()
{
    return std::visit([](auto &val)
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
        else if constexpr (std::is_same_v<T, std::map<std::string, json>>)
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
        return temp_value;
    }, __value);
}


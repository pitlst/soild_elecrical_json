// 以下是对应函数的具体实现
#include "json.hpp"
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <iostream>

using namespace swq;

json::json()
{
    __value = std::make_unique<m_value>();
}
json::json(bool input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<bool>(input_value);
}
json::json(int input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<int>(input_value);
}
json::json(double input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<double>(input_value);
}
json::json(const json &input_value)
{
    __value = std::make_unique<m_value>();
    *__value = *(input_value.__value);
}
json::json(const char *input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<std::string>(std::string_view(input_value));
}
json::json(std::string_view input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<std::string>(input_value);
}
json::json(const std::vector<json> &input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<std::vector<json>>(input_value);
}
json::json(const std::unordered_map<std::string, json> &input_value)
{
    __value = std::make_unique<m_value>();
    __value->emplace<std::unordered_map<std::string, json>>(input_value);
}

json &json::operator=(const json &input_value)
{
    *__value = *(input_value.__value);
    return *this;
}
json &json::operator[](std::size_t index)
{
    return std::visit([index](auto &val) -> json &
                      {
        if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::vector<json>>)
        {
            return val.at(index);
        }
        else
        {
            throw std::logic_error("json type is not array");
        } },
                      *__value);
}
json &json::operator[](const std::string &key)
{
    return std::visit([key](auto &val) -> json &
                      {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            return val.at(key);
        }
        else
        {
            throw std::logic_error("json type is not object");
        } },
                      *__value);
}

json::operator bool() const
{
    return to_bool();
}
json::operator int() const
{
    return to_int();
}
json::operator float() const
{
    return float(to_double());
}
json::operator double() const
{
    return to_double();
}
json::operator std::string() const
{
    return to_str();
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
        else if constexpr (std::is_same_v<T, std::string>)
        {
            temp_value = ("\"" + val + "\"");
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
                temp_value += ("\"" + ch.first + "\":" + ch.second.to_str());
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

std::string json::get_type_name() const
{
    return std::visit([](const auto &val) -> std::string
                      {
        bool label = false;
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::monostate>)
        {
            return "null";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return "bool";
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            return "int";
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            return "double";
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return "string";
        }
        else if constexpr (std::is_same_v<T, std::vector<json>>)
        {
            return "array";
        }
        else if constexpr (std::is_same_v<T, std::unordered_map<std::string, json>>)
        {
            return "object";
        } },
                      *__value);
}


// 注意，使用该函数时会重置对应类型的数据
void json::set_type(std::string_view input_type_name)
{
    if (input_type_name == "null" && !is_null())
    {
        clear();
        __value.release();
        __value = std::make_unique<m_value>();
    }
    else if (input_type_name == "bool" && !is_bool())
    {
        clear();
        __value.reset(new m_value(false));
    }
    else if (input_type_name == "int" && !is_int())
    {
        clear();
        __value.reset(new m_value(0));
    }
    else if ((input_type_name == "double" || input_type_name == "float")&& !is_double())
    {
        clear();
        __value.reset(new m_value(0.0));
    }
    else if (input_type_name == "string" && !is_string())
    {
        clear();
        __value.reset(new m_value(""));
    }
    else if (input_type_name == "array" && !is_array())
    {
        clear();
        std::vector<json> temp;
        __value.release();
        __value = std::make_unique<m_value>(temp);
    }
    else if (input_type_name == "object" && !is_object())
    {
        clear();
        std::unordered_map<std::string, json> temp;
        __value.release();
        __value = std::make_unique<m_value>(temp);
    }
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

json json::copy() const
{
    return json(*this);
}

void json::copy(const json &input_value)
{
    *(this->__value) = *(input_value.__value);
}

void json::append(const json &input_value)
{
    std::visit([input_value](auto &val)
               {
        if constexpr(std::is_same_v<std::decay_t<decltype(val)>, std::vector<json>>)
        {
            val.emplace_back(input_value);
        } },
               *__value);
}

void json::append(std::string_view input_name, const json &input_value)
{
    std::visit([input_name, input_value](auto &val)
               {
        if constexpr(std::is_same_v<std::decay_t<decltype(val)>, std::unordered_map<std::string, json>>)
        {
            val.emplace(input_name, input_value);
        } },
               *__value);
}

void json::clear()
{
    std::visit([](auto &val)
               {
        using T = std::decay_t<decltype(val)>;
        if constexpr(std::is_same_v<T, std::vector<json>>)
        {
            for (auto & ch : val)
            {
                ch.clear();
            }
        }    
        else if constexpr(std::is_same_v<T, std::unordered_map<std::string, json>>)
        { 
            for (auto & ch : val)
            {
                ch.second.clear();
            }
        } },
               *__value);
    __value.release();
    __value = std::make_unique<m_value>();
}

void json::remove(std::size_t index)
{
    std::visit([index](auto &val)
               {
        if constexpr(std::is_same_v<std::decay_t<decltype(val)>, std::vector<json>>)
        {
            val.erase(val.begin() + index);
        } },
               *__value);
}

void json::remove(const std::string &key)
{
    std::visit([key](auto &val)
               {
        if constexpr(std::is_same_v<std::decay_t<decltype(val)>, std::unordered_map<std::string, json>>)
        {
            auto it = val.find(key);
            if (it != val.end()) 
            {
                val.erase(it);
            }
        } },
               *__value);
}

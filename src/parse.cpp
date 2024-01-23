#include "parse.hpp"

#include <stdexcept>

using namespace swq;

std::string swq::getLevelStr(int level)
{
    std::string levelStr = "";
    for (int i = 0; i < level; i++)
    {
        levelStr += "    "; // 这里可以\t换成你所需要缩进的空格数
    }
    return levelStr;
}

// json的格式化输出函数
std::string swq::format_json(const std::string json)
{
    std::string result = "";
    int level = 0;
    for (std::string::size_type index = 0; index < json.size(); index++)
    {
        char c = json[index];

        if (level > 0 && '\n' == json[json.size() - 1])
        {
            result += getLevelStr(level);
        }

        switch (c)
        {
        case '{':
        case '[':
            result = result + c + "\n";
            level++;
            result += getLevelStr(level);
            break;
        case ',':
            result = result + c + "\n";
            result += getLevelStr(level);
            break;
        case '}':
        case ']':
            result += "\n";
            level--;
            result += getLevelStr(level);
            result += c;
            break;
        default:
            result += c;
            break;
        }
    }
    return result;
}

// json_parse::json_parse(const std::string &input_str) : m_str(input_str)
// {
// }

// char json_parse::get_next_token()
// {
//     if (!end_label)
//     {
//         while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
//         {
//             m_idx++;
//         }
//         if (m_idx == m_str.size())
//         {
//             end_label = true;
//             return m_str[m_str.size() - 1];
//         }
//         else
//         {
//             return m_str[m_idx++];
//         }
//     }
//     else
//     {
//         return m_str[m_str.size() - 1];
//     }
// }

// bool json_parse::in_range(int x, int lower, int upper)
// {
//     return (x >= lower && x <= upper);
// }


// json json_parse::parse()
// {
//     char ch = get_next_token();
//     if (end_label)
//     {
//         return json();
//     }
//     else
//     {
//         switch (ch)
//         {
//         case 'n':
//             m_idx--;
//             return parse_null();
//         case 't':
//         case 'f':
//             m_idx--;
//             return parse_bool();
//         case '-':
//         case '+':
//         case '0':
//         case '1':
//         case '2':
//         case '3':
//         case '4':
//         case '5':
//         case '6':
//         case '7':
//         case '8':
//         case '9':
//             m_idx--;
//             return parse_number();
//         case '"':
//             return parse_string();
//         case '[':
//             return parse_array();
//         case '{':
//             return parse_object();
//         default:
//             break;
//         }
//         throw std::logic_error("unexpected character in parse json");
//     }
// }

// json json_parse::parse_null()
// {
//     if (m_str.compare(m_idx, 4, "null") != 0)
//     {
//         throw std::logic_error("parse null error");
//     }
//     m_idx += 4;
//     return json();
// }

// json json_parse::parse_bool()
// {
//     if (m_str.compare(m_idx, 4, "true") == 0)
//     {
//         m_idx += 4;
//         return json(true);
//     }
//     else if (m_str.compare(m_idx, 5, "false") == 0)
//     {
//         m_idx += 5;
//         return json(false);
//     }
//     else
//     {
//         throw std::logic_error("parse bool error");
//     }
// }

// json json_parse::parse_number()
// {
//     size_t pos = m_idx;
//     // 校验有没有负号
//     if (m_str[m_idx] == '-')
//         m_idx++;
//     // 校验是否是数字
//     if (m_str[m_idx] == '0')
//     {
//         m_idx++;
//     }
//     else if (in_range(m_str[m_idx], '1', '9'))
//     {
//         m_idx++;
//         while (in_range(m_str[m_idx], '0', '9'))
//         {
//             m_idx++;
//         }
//     }
//     else
//     {
//         throw std::logic_error("invalid character in number");
//     }
//     // 校验是不是整数
//     if (m_str[m_idx] != '.')
//     {
//         // atoi在遇到非数字字符时自动停止转换
//         return json(std::atoi((m_str.c_str() + pos)));
//     }
//     else
//     {
//         // 如果是浮点数
//         m_idx++;
//         if (!in_range(m_str[m_idx], '0', '9'))
//         {
//             throw std::logic_error("at least one digit required in fractional part");
//         }
//         while (in_range(m_str[m_idx], '0', '9'))
//         {
//             m_idx++;
//         }
//         return json(std::atof(m_str.c_str() + pos));
//     }
// }

// json json_parse::parse_string()
// {
//     size_t pos = m_idx;
//     while (true)
//     {
//         if (m_idx == m_str.size())
//         {
//             throw std::logic_error("unexpected end of input in string");
//         }

//         char ch = m_str[m_idx++];
//         if (ch == '"')
//         {
//             break;
//         }

//         // The usual case: non-escaped characters
//         if (ch == '\\')
//         {
//             ch = m_str[m_idx++];
//             switch (ch)
//             {
//             case 'b':
//             case 't':
//             case 'n':
//             case 'f':
//             case 'r':
//             case '"':
//             case '\\':
//                 break;
//             case 'u':
//                 m_idx += 4;
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
//     return json(m_str.substr(pos, m_idx - pos - 1));
// }

// json json_parse::parse_array()
// {
//     json arr;
//     char ch = get_next_token();
//     if (ch == ']')
//     {
//         return arr;
//     }
//     m_idx--;
//     while (true)
//     {
//         arr.append(parse());
//         ch = get_next_token();
//         if (ch == ']')
//         {
//             break;
//         }
//         if (ch != ',')
//         {
//             throw std::logic_error("expected ',' in array");
//         }
//     }
//     return arr;
// }

// json json_parse::parse_object()
// {
//     json obj;
//     char ch = get_next_token();
//     if (ch == '}')
//     {
//         return obj;
//     }
//     m_idx--;
//     while (true)
//     {
//         ch = get_next_token();
//         if (ch != '"')
//         {
//             throw std::logic_error("expected '\"' in object");
//         }
//         std::string key = parse_string();
//         ch = get_next_token();
//         if (ch != ':')
//         {
//             throw std::logic_error("expected ':' in object");
//         }
//         obj[key] = parse();
//         ch = get_next_token();
//         if (ch == '}')
//         {
//             break;
//         }
//         if (ch != ',')
//         {
//             throw std::logic_error("expected ',' in object");
//         }
//     }
//     return obj;
// }
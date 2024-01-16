#include "parse.hpp"

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
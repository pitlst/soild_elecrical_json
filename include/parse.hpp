#include "json.hpp"

namespace swq
{
    // json的格式化输出函数
    std::string getLevelStr(int level);
    std::string format_json(const std::string json);

    // 给json用于解析的类
    // 这部分仅在解析文件时使用，所以单独组成一个类，在分析完成后析构，不保留相关数据，提高速度
    class json_parse
    {
    public:
        json_parse();
        json_parse(const std::string &input_str);

        void load(const std::string &input_str);
        json parse();

    private:
        char get_next_token();
        bool in_range(int x, int lower, int upper);

        json parse_null();
        json parse_bool();
        json parse_number();
        json parse_string();
        json parse_array();
        json parse_object();

        std::string m_str = "";
        size_t m_idx = 0;
        bool end_label = false;
    };
}
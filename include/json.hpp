#include <variant>
#include <string>
#include <vector>
#include <initializer_list>
#include <unordered_map>
#include <memory>

namespace swq
{
    class json
    {
    public:
        json();
        json(bool input_value);
        json(int input_value);
        json(double input_value);
        json(const json &input_value);
        json(const char * input_value);
        json(std::string_view input_value);
        json(const std::vector<json> &input_value);
        json(const std::unordered_map<std::string, json> &input_value);
        ~json() = default;

        /*
        对于任何默认的或者为隐式的拷贝方式均为浅拷贝
        所有针对json中的值的修改均会同步到所有指向对应变量的json类
        对于深拷贝需要重新声明一个新的json变量或者使用下面定义的copy函数手动复制
        */
        json &operator=(const json &input_value);
        json &operator[](std::size_t index);
        json &operator[](const std::string &key);

        operator bool() const;
        operator int() const;
        operator float() const;
        operator double() const;
        operator std::string() const;

        static json array(const std::initializer_list<json> &input_value);
        static json object(const std::unordered_map<std::string, json> &input_value);

        bool to_bool() const;
        int to_int() const;
        double to_double() const;
        std::string to_str() const;

        bool is_null() const;
        bool is_bool() const;
        bool is_int() const;
        bool is_double() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;
        bool is_type(std::string_view input_type_name) const;
        std::string get_type_name() const;
        void set_type(std::string_view input_type_name);

        bool has(int index) const;
        bool has(const std::string &key) const;
        bool empty() const;
        std::size_t size() const;
        json copy() const;
        void copy(const json &input_value);
        void append(const json &input_value);
        void append(std::string_view input_name, const json &input_value);
        void clear();
        void remove(std::size_t index);
        void remove(const std::string &key);

    private:
        using m_value = std::variant<
            std::monostate,
            bool,
            int,
            double,
            std::string,
            std::vector<json>,
            std::unordered_map<std::string, json>>;
        /*
            这里不能直接使用std::unordered_map
            会引发不完整类型的错误
            因为指针允许不完整类型的包含，所以使用指针包含所使用的数据
            详见https://stackoverflow.com/questions/13089388/how-to-have-an-unordered-map-where-the-value-type-is-the-class-its-in
        */
        std::unique_ptr<m_value> __value;
    };
}

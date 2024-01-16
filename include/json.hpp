#include <variant>
#include <string>
#include <vector>
#include <initializer_list>
#include <map>

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
        json(const char *input_value);
        json(const std::string &input_value);
        json(const std::vector<json> &input_value);
        json(const std::map<std::string, json> &input_value);
        ~json() = default;

        json operator=(const json& input_value);

        static json array(const std::initializer_list<json> &input_value);
        static json object(const std::map<std::string, json> &input_value);

        bool to_bool();
        int to_int();
        double to_double();
        std::string to_str();


    private:
        using m_value = std::variant<std::monostate, bool, int, double, std::string, std::vector<json>, std::map<std::string, json>>;
        m_value __value;
    };
}

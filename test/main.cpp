#include <iostream>
#include <json.hpp>
#include <gtest/gtest.h>

// bool json_init_test_0()
// {
//     swq::json temp;
//     temp[0];
//     return true;
// }

int main(int argc, char **argv)
{
    // testing::InitGoogleTest(&argc, argv);
    // EXPECT_EQ(json_init_test_0, true);
    swq::json temp;
    std::variant<int, std::string> v = "abc";
    std::cout << std::boolalpha
              << "variant holds int? "
              << std::holds_alternative<int>(v) << '\n'
              << "variant holds string? "
              << std::holds_alternative<std::string>(v) << '\n';
    // temp[0];
    return 0;
    // return RUN_ALL_TESTS();
}
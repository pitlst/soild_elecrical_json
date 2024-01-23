#include <iostream>
#include <json.hpp>
// #include <gtest/gtest.h>

// bool json_init_test_0()
// {
//     swq::json temp;
//     temp[0];
//     return true;
// }

int main()
{
    // testing::InitGoogleTest(&argc, argv);
    // EXPECT_EQ(json_init_test_0, true);
    swq::json temp = swq::json::array({1,2,"124",4,5});
    std::cout << temp[2].get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("null");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("bool");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("int");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("double");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("array");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    temp.set_type("object");
    std::cout << temp.get_type_name() << std::endl;
    std::cout << temp.to_str() << std::endl;
    return 0;
    // return RUN_ALL_TESTS();
}
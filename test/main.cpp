#include <iostream>
#include <json.hpp>
#include <gtest/gtest.h>

bool json_init_test_0()
{
    return true;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    // EXPECT_EQ(json_init_test_0, true);
    return RUN_ALL_TESTS();
}
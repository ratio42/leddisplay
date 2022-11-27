#include "../library.h"

#include <gtest/gtest.h>

TEST(TestSuite, TestName)
{

}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
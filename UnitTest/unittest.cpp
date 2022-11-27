#include "../library.h"

#include <gtest/gtest.h>

TEST(ConnectionTest, WithoutConnectingStateIsNotConnected)
{
    // arrange & act
    bool isConnected{IsConnected()};

    // assert
    ASSERT_FALSE(isConnected);
}

TEST(ConnectionTest, AfterConnectingWithoutDebugStateIsConnected)
{
    // arrange & act
    Connect(false);
    bool isConnected{IsConnected()};

    // assert
    ASSERT_TRUE(isConnected);
}

TEST(ConnectionTest, AfterConnectingWithDebugStateIsConnected)
{
    // arrange & act
    Connect(true);
    bool isConnected{IsConnected()};

    // assert
    ASSERT_TRUE(isConnected);
}

TEST(ConnectionTest, AfterDisconnectingStateIsNotConnected)
{
    // arrange & act
    Connect(true);
    bool isConnectedAfterConnect{IsConnected()};
    Disconnect();
    bool isConnectedAfterDisconnect{IsConnected()};

    // assert
    ASSERT_TRUE(isConnectedAfterConnect);
    ASSERT_FALSE(isConnectedAfterDisconnect);
}


// some led status tests - using fixtures
class LedStatusTests : public testing::Test{
public:
    void SetUp() override;
};

void LedStatusTests::SetUp() {
    // connect with debug output
    Connect(true);
}

TEST_F(LedStatusTests, Test1) {

}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
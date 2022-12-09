#include "../library.h"

#include <gtest/gtest.h>

constexpr int c_SleepTimeAfterLedTestInSeconds = 5;

TEST(ConnectionTest, WithoutConnectingStateIsNotConnected)
{
    // arrange & act
    bool isConnected{IsConnected()};

    // assert
    ASSERT_FALSE(isConnected);
}

TEST(ConnectionTest, AfterConnectingWithoutDebugAndGraphicalOutputStateIsConnected)
{
    // arrange & act
    Connect();
    bool isConnected{IsConnected()};
    Disconnect();

    // assert
    ASSERT_TRUE(isConnected);
}

TEST(ConnectionTest, AfterConnectingWithDebugAndWithoutGraphicalOutputStateIsConnected)
{
    // arrange
    const bool debugState{true};

    // act
    Connect(debugState);
    bool isConnected{IsConnected()};
    Disconnect();

    // assert
    ASSERT_TRUE(isConnected);
}

TEST(ConnectionTest, AfterConnectingWithDebugAndGraphicalOutputStateIsConnected)
{
    // arrange
    const bool debugState{true};
    const bool graphicalOutputState{true};

    // act
    Connect(debugState, graphicalOutputState);
    bool isConnected{IsConnected()};
    Disconnect();

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
    void TearDown() override;
};

void LedStatusTests::SetUp() {
    // connect with debug output and graphical output
    Connect(true, true);
    ClearAll();
}

void LedStatusTests::TearDown() {
    sleep(c_SleepTimeAfterLedTestInSeconds);
    Disconnect();
}

TEST_F(LedStatusTests, TurnSingleLedInUlRedOn) {
    // arrange
    int x{0}, y{0}, r{100}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    LedOn(x, y, r, g, b);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}

TEST_F(LedStatusTests, TurnSingleLedInUrRedOn) {
    // arrange
    int x{63}, y{0}, r{100}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    LedOn(x, y, r, g, b);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}

TEST_F(LedStatusTests, TurnSingleLedInLlRedOn) {
    // arrange
    int x{0}, y{31}, r{100}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    LedOn(x, y, r, g, b);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}

TEST_F(LedStatusTests, TurnSingleLedInLrRedOn) {
    // arrange
    int x{63}, y{31}, r{100}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    LedOn(x, y, r, g, b);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}

TEST_F(LedStatusTests, TurnSingleLedOnAndOffAgain) {
    // arrange
    int x{0}, y{0}, r{100}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    LedOn(x, y, r, g, b);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};
    LedOff(x, y);
    const bool ledIsOnAfterTurningOff{LedIsOn(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
    ASSERT_FALSE(ledIsOnAfterTurningOff);
}


TEST_F(LedStatusTests, TurnSingleLedOnAndSetBlinkingAt1Hz) {
    // arrange
    int x{31}, y{15}, r{200}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    const bool ledIsNotBlinkingBeforeTurningOn{LedIsBlinking(x, y)};
    LedOn(x, y, r, g, b);
    LedAddBlinkingPeriodInMs(x, y, 1000);

    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};
    const bool ledIsBlinkingAfterTurningOn{LedIsBlinking(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_FALSE(ledIsNotBlinkingBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
    ASSERT_TRUE(ledIsBlinkingAfterTurningOn);
}

TEST_F(LedStatusTests, TurnSingleLedOnAndSetBlinkingAt1HzAndTurnBlinkingOffAgain) {
    // arrange
    int x{31}, y{15}, r{200}, g{0}, b{0};

    // act
    const bool ledIsOnBeforeTurningOn{LedIsOn(x, y)};
    const bool ledIsNotBlinkingBeforeTurningOn{LedIsBlinking(x, y)};
    LedOn(x, y, r, g, b);
    LedAddBlinkingPeriodInMs(x, y, 1000);
    const bool ledIsOnAfterTurningOn{LedIsOn(x, y)};
    const bool ledIsBlinkingAfterTurningOn{LedIsBlinking(x, y)};
    LedDisableBlinking(x, y);
    const bool ledIsOnAfterDisableBlinking{LedIsOn(x, y)};
    const bool ledIsBlinkingAfterDisableBlinking{LedIsBlinking(x, y)};

    // assert
    ASSERT_FALSE(ledIsOnBeforeTurningOn);
    ASSERT_FALSE(ledIsNotBlinkingBeforeTurningOn);
    ASSERT_TRUE(ledIsOnAfterTurningOn);
    ASSERT_TRUE(ledIsBlinkingAfterTurningOn);
    ASSERT_TRUE(ledIsOnAfterDisableBlinking);
    ASSERT_FALSE(ledIsBlinkingAfterDisableBlinking);
}


TEST_F(LedStatusTests, TurnEveryLedOnWithColorRamp) {
    // arrange
    int rampMinRed{30}, rampMaxRed{250}, rampStepRed{10};
    int rampMinGreen{0}, rampMaxGreen{250}, rampStepGreen{15};
    int rampMinBlue{50}, rampMaxBlue{250}, rampStepBlue{5};

    // act
    int red{rampMinRed};
    int green{rampMinGreen};
    int blue{rampMinBlue};
    for (int x=0; x < 64; x++) {
        for (int y=0; y < 32; y++) {
            LedOn(x, y, red, green, blue);
            red += rampStepRed;
            green += rampStepGreen;
            blue += rampStepBlue;

            if (red > rampMaxRed) { red = rampMinRed; }
            if (green > rampMaxGreen) { green = rampMinGreen; }
            if (blue > rampMaxBlue) { blue= rampMinBlue; }
        }
    }
    const bool ledIsOnAfterTurningOn{LedIsOn(17, 5)};

    // assert - just check on random led
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}

TEST_F(LedStatusTests, SetEveryLedBlinkingWithColorRamp) {
    // arrange
    int rampMinRed{200}, rampMaxRed{50}, rampStepRed{-10};
    int rampMinGreen{60}, rampMaxGreen{250}, rampStepGreen{5};
    int rampMinBlue{50}, rampMaxBlue{250}, rampStepBlue{5};

    // act
    int red{rampMinRed};
    int green{rampMinGreen};
    int blue{rampMinBlue};
    for (int x=0; x < 64; x++) {
        for (int y=0; y < 32; y++) {
            LedOn(x, y, red, green, blue);
            LedAddBlinkingPeriodInMs(x, y, x*y % 1000);
            red += rampStepRed;
            green += rampStepGreen + x;
            blue += rampStepBlue + 2*x;

            if (red > rampMaxRed) { red = rampMinRed; }
            if (green > rampMaxGreen) { green = rampMinGreen; }
            if (blue > rampMaxBlue) { blue= rampMinBlue; }
        }
    }
    const bool ledIsOnAfterTurningOn{LedIsOn(17, 5)};

    // assert - just check on random led
    ASSERT_TRUE(ledIsOnAfterTurningOn);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
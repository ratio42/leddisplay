#include "library.h"
#include "internal.h"

#include <iostream>
#include <thread>

// global objects
LibraryState g_LibraryState;
Display g_Display(64, 32);
std::unique_ptr<std::thread> g_CyclicLoop;
bool g_StopDisplayLoop = false;

constexpr int loopCycleInMs = 50; // 50ms is 20 fps

static void DebugWrite(const std::string& debugText_p)
{
    if (g_LibraryState.IsDebugOutputEnabled()) {
        std::cout << "LEDDISPLAY LIBRARY DEBUG: " << debugText_p << std::endl;
    }
}

void CyclicLoop() {
    const auto timeWindow = std::chrono::milliseconds(loopCycleInMs);

    while(!g_StopDisplayLoop)
    {
        auto start = std::chrono::steady_clock::now();

        DebugWrite("thread alive ...");

        auto timeToWaitUntil = start + timeWindow;
        std::this_thread::sleep_until(timeToWaitUntil);
    }
}

void Connect(bool enableDebugOutput) {
    if (enableDebugOutput)
    {
        g_LibraryState.EnableDebugOutput();
    }
    g_LibraryState.SetConnected(true);
    DebugWrite("Display connected!");

    // start cyclic loop, for write access to display and refreshing graphical output
    g_CyclicLoop = std::make_unique<std::thread>(CyclicLoop);
}

bool IsConnected() {
    bool isConnected{g_LibraryState.IsConnected()};
    std::string outputString{"Connection status requested! Connected: "};
    if (isConnected) {
        outputString += "true";
    } else {
        outputString += "false";
    }
    DebugWrite(outputString);

    return isConnected;
}

void Disconnect() {
    DebugWrite("Going to disconnect!");
    if (g_CyclicLoop) {
        g_StopDisplayLoop = true;
        g_CyclicLoop->join();
    }
    g_StopDisplayLoop = false;
    g_LibraryState.SetConnected(false);
}

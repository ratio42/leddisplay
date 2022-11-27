#include "library.h"
#include "internal.h"

#include <iostream>

// global objects
LibraryState g_LibraryState;
Display g_Display(64, 32);

constexpr int loopCycleInMs = 50; // 50ms is 20 fps

static void DebugWrite(const std::string& debugText_p)
{
    if (g_LibraryState.IsDebugOutputEnabled()) {
        std::cout << "LEDDISPLAY LIBRARY DEBUG: " << debugText_p << std::endl;
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
    g_LibraryState.SetConnected(false);
}

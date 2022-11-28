#include "library.h"
#include "internal.h"

#include <SDL.h>

#include <iostream>
#include <thread>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// global objects
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LibraryState g_LibraryState;
Display g_Display(64, 32);
std::unique_ptr<std::thread> g_CyclicLoop;
bool g_StopDisplayLoop = false;

constexpr int c_LoopCycleInMs = 50; // 50ms is 20 fps
constexpr int c_WindowWidth = 500;
constexpr int c_WindowHeight = 300;

// SDL objects
SDL_Renderer *g_SdlRenderer = nullptr; // pointer for the renderer
SDL_Window *g_SdlWindow = nullptr; // pointer for the window

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// graphical output and drawing routines
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// TODO: move SDL part (or probably all drawing) into separate file

void GraphicalOutput_Init() {
    SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
    SDL_CreateWindowAndRenderer(c_WindowWidth, c_WindowHeight, 0, &g_SdlWindow, &g_SdlRenderer);
    SDL_SetRenderDrawColor(g_SdlRenderer, 0, 0, 0, 0);      // setting draw color
    SDL_RenderClear(g_SdlRenderer);      // Clear the newly created window
    SDL_RenderPresent(g_SdlRenderer);    // Reflects the changes done in the window.
}

void GraphicalOutput_Stop() {
    SDL_DestroyRenderer(g_SdlRenderer);
    SDL_DestroyWindow(g_SdlWindow);
    SDL_Quit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// (static) helper routines
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void DebugWrite(const std::string& debugText_p)
{
    if (g_LibraryState.IsDebugOutputEnabled()) {
        std::cout << "LEDDISPLAY LIBRARY DEBUG: " << debugText_p << std::endl;
    }
}

void CyclicLoop() {
    const auto timeWindow = std::chrono::milliseconds(c_LoopCycleInMs);

    while(!g_StopDisplayLoop)
    {
        auto start = std::chrono::steady_clock::now();

        DebugWrite("thread alive ...");

        auto timeToWaitUntil = start + timeWindow;
        std::this_thread::sleep_until(timeToWaitUntil);
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// implementation of API routines
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Connect(bool enableDebugOutput, bool enableGraphicalOutput) {
    if (enableDebugOutput)
    {
        g_LibraryState.EnableDebugOutput();
    }
    if (enableGraphicalOutput) {
        g_LibraryState.EnableGraphicalOutput();
    }
    g_LibraryState.SetConnected(true);
    DebugWrite("Display connected!");

    if (g_LibraryState.IsGraphicalOutputEnabled()) {
        // initialize graphic output
        GraphicalOutput_Init();
    }

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

    if (g_LibraryState.IsGraphicalOutputEnabled()) {
        GraphicalOutput_Stop();
    }

    g_StopDisplayLoop = false;
    g_LibraryState.SetConnected(false);
}

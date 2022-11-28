#include "library.h"
#include "internal.h"

#include <SDL.h>

#include <iostream>
#include <thread>
#include <sstream>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// global objects
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
constexpr int c_NumberOfLedsX = 64;
constexpr int c_NumberOfLedsY = 32;

LibraryState g_LibraryState;
Display g_Display(c_NumberOfLedsX, c_NumberOfLedsY);
std::unique_ptr<std::thread> g_CyclicLoop;
bool g_StopDisplayLoop = false;

constexpr int c_LoopCycleInMs = 50; // 50ms is 20 fps

// SDL objects
SDL_Renderer *g_SdlRenderer = nullptr; // pointer for the renderer
SDL_Window *g_SdlWindow = nullptr; // pointer for the window

// some constants for drawing
constexpr int c_WindowWidth = 800;
constexpr int c_WindowHeight = 400;
constexpr int c_WindowOuterBorder = 2;
constexpr int c_LedOuterBorder = 2;

constexpr int c_FrameWidth{c_WindowWidth - 2 * c_WindowOuterBorder};
constexpr int c_FrameHeight{c_WindowHeight - 2 * c_WindowOuterBorder};

// calculate size of leds
constexpr int c_WidthAvailableForLeds{c_FrameWidth - (c_NumberOfLedsX + 1) * c_LedOuterBorder};
constexpr int c_LedWidth{c_WidthAvailableForLeds / c_NumberOfLedsX};
constexpr int c_HeigthAvailableForLeds{c_FrameHeight - (c_NumberOfLedsY + 1) * c_LedOuterBorder};
constexpr int c_LedHeight{c_HeigthAvailableForLeds / c_NumberOfLedsY};

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

void GraphicalOutput_DrawFrame() {
    // Setting the color to be  100% opaque (0% transparent).
    const int red{100};
    const int blue{100};
    const int green{100};
    SDL_SetRenderDrawColor(g_SdlRenderer, red, green, blue, 255);

    SDL_Rect frameRectangle;
    frameRectangle.x = c_WindowOuterBorder;
    frameRectangle.y = c_WindowOuterBorder;
    frameRectangle.w = c_FrameWidth;
    frameRectangle.h = c_FrameHeight;

    SDL_RenderDrawRect(g_SdlRenderer, &frameRectangle);

    // Show the change on the screen
    SDL_RenderPresent(g_SdlRenderer);
}

// draws a led, whereas x and y position is regarding matrix: 0,0 is upper left
// Attention: this does not update the screen
void GraphicalMode_DrawLed(int x, int y, int red_p, int green_p, int blue_p) {
    SDL_SetRenderDrawColor(g_SdlRenderer, red_p, green_p, blue_p, 255);

    SDL_Rect ledRectangle;
    ledRectangle.x = c_WindowOuterBorder + c_LedOuterBorder + (x * (c_LedWidth + c_LedOuterBorder));
    ledRectangle.y = c_WindowOuterBorder + c_LedOuterBorder + (y * (c_LedHeight + c_LedOuterBorder));
    ledRectangle.w = c_LedWidth;
    ledRectangle.h = c_LedHeight;

    SDL_RenderFillRect(g_SdlRenderer, &ledRectangle);
}

void GraphicalOutput_UpdateScreen() {
    SDL_RenderPresent(g_SdlRenderer);
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

    if (g_LibraryState.IsGraphicalOutputEnabled()) {
        GraphicalOutput_DrawFrame();
    }

    while(!g_StopDisplayLoop)
    {
        auto start = std::chrono::steady_clock::now();
        auto duration = start.time_since_epoch();
        auto timeStampInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        // get leds from display variable and update them if needed on real display
        for (int x = 0; x < c_NumberOfLedsX; x++) {
            for (int y = 0; y < c_NumberOfLedsY; y++) {
                const LedColor currentLedColor = g_Display.GetLed(x, y).GetColor(timeStampInMs);

                GraphicalMode_DrawLed(x, y, currentLedColor.GetRed(), currentLedColor.GetGreen(), currentLedColor.GetBlue());
            }
        }
        GraphicalOutput_UpdateScreen();

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

void LedOn(int x, int y, int r, int g, int b) {
    std::stringstream outputStream;
    outputStream << "Turn LED on: (" << x << "," << y << ") with color (" << r << "," << g << "," << b << ")";
    DebugWrite(outputStream.str());

    g_Display.GetLed(x, y).SetColor(LedColor(r, g, b));
}

bool LedIsOn(int x, int y) {
    return g_Display.GetLed(x, y).IsOn();
}

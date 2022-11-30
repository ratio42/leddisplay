#pragma once

#include <vector>
#include <cmath>

// not yet really checked for hw connection
enum ConnectionState {
    eNotConnected = 0,
    eConnected = 1
};

enum DebugState {
    eNoDebug = 0,
    eOutputStatusOnStdOut = 1
};

enum GraphicalMode {
    eNoGraphicalOutput= 0,
    eGraphicalOutput = 1
};

class LedColor {
public:
    LedColor() {};
    LedColor(int red_p, int green_p, int blue_p) : m_Red{red_p}, m_Green{green_p}, m_Blue{blue_p} {}

    int GetRed() const {return m_Red;}
    int GetGreen() const {return m_Green;}
    int GetBlue() const {return m_Blue;}

    bool IsOn() const {return m_Red != 0 || m_Green != 0 || m_Blue != 0;}
    void TurnOff() {
        m_Red = 0;
        m_Green = 0;
        m_Blue = 0;
    }

    void SetColor(int red_p, int green_p, int blue_p) {
        m_Red = red_p;
        m_Green = green_p;
        m_Blue = blue_p;
    }

private:
    int m_Red = 0;
    int m_Green = 0;
    int m_Blue = 0;
};

class Led {
public:
    Led() {};

    bool IsBlinking() const {return !m_BlinkingPeriodInMs.empty();}
    // returns if led is on (either permanently or blinking)
    bool IsOn() const {return m_Color.IsOn();}
    LedColor GetColor(long timeStampInMs) const {
        if (IsBlinking()) {
            for (auto period : m_BlinkingPeriodInMs) {
                if ((timeStampInMs + period/2) % period < period/2) {
                    return m_Color;
                }
            }
            return LedColor();
        } else {
            return m_Color;
        }
    }

    void SetColor(LedColor color_p) {
        m_Color = color_p;
    }

    void AddBlinkingPeriod(int periodInMs_p) {
        m_BlinkingPeriodInMs.push_back(periodInMs_p);
    }

    void DisableBlinking() {m_BlinkingPeriodInMs.clear();}
    // turns led completely off (black and no blinking)
    void TurnOff() {
        DisableBlinking();
        m_Color.TurnOff();
    }

private:
    LedColor m_Color;
    std::vector<int> m_BlinkingPeriodInMs;

    // TODO: along with different periods also different colors should be possible
};

class LibraryState {
public:
    LibraryState() {};

    void SetConnected(bool connected_p = true) {
        m_ConnectionState = connected_p ? eConnected : eNotConnected;
        if (!connected_p) {
            m_DebugState = eNoDebug;
        }
    }
    bool IsConnected() const {return m_ConnectionState == eConnected;}

    void EnableDebugOutput() {m_DebugState = eOutputStatusOnStdOut;}
    bool IsDebugOutputEnabled() {return m_DebugState == eOutputStatusOnStdOut;}

    void EnableGraphicalOutput() {m_GraphicalMode = eGraphicalOutput;}
    bool IsGraphicalOutputEnabled() {return  m_GraphicalMode == eGraphicalOutput;}

private:
    ConnectionState m_ConnectionState = eNotConnected;
    DebugState m_DebugState = eNoDebug;
    GraphicalMode m_GraphicalMode = eNoGraphicalOutput;
};

using RowOfLeds = std::vector<Led>;

class Display {
public:
    Display(int width_p, int height_p) : m_WidthInPixel{width_p}, m_HeightInPixel{height_p} {
        for (int height = 0; height < height_p; height++) {
            RowOfLeds ledRow(width_p);
            m_Leds.emplace_back(ledRow);
        }
    }

    // disables each led
    void Clear() {
        for (RowOfLeds& rowOfLeds : m_Leds) {
            for (Led& led : rowOfLeds) {
                led.TurnOff();
            }
        }
    }

    Led& GetLed(int x_p, int y_p) {
        return m_Leds.at(y_p).at(x_p);
    }

private:
    // resolution
    int m_WidthInPixel;
    int m_HeightInPixel;

    // led colors
    std::vector<RowOfLeds> m_Leds;

    // brightness
    int m_Brightness = 10;
};
#ifndef LEDDISPLAY_LIBRARY_H
#define LEDDISPLAY_LIBRARY_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// display/library connection
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Establish connection to display
void Connect(bool enableDebugOutput = false, bool enableGraphicalOutput = false);
void Disconnect();

// returns true if connected
bool IsConnected();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// clear whole display
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ClearAll();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// simple led on/off calls
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void LedOn(int x, int y, int r, int g, int b);
void LedOff(int x, int y);

void LedAddBlinkingPeriodInMs(int x, int y, int periodInMs);
// disable blinking -> permanent on
void LedDisableBlinking(int x, int y);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// led status calls
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool LedIsBlinking(int x, int y);
bool LedIsOn(int x, int y);

void LedGetColor(int x, int y, int &r, int &g, int &b);

#endif //LEDDISPLAY_LIBRARY_H

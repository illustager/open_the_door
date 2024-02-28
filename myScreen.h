#ifndef MYSCREEN_H
#define MYSCREEN_H

#include <Arduino.h>

#include <SSD1306Wire.h>

class myScreen {
public:
  myScreen(byte addr, byte sda, byte scl);
  
  ~myScreen();

  void debug(uint32_t, bool);

private:
  SSD1306Wire *myscreen;
  byte addr, sda, scl;
}; // class myScreen

#endif // MYSCREEN_H

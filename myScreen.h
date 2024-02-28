#ifndef MYSCREEN_H
#define MYSCREEN_H

#include <SSD1306Wire.h>

class myScreen {
public:
  myScreen(byte addr, byte sda, byte scl) 
    : addr(addr), sda(sda), scl(scl) {
      myscreen = new SSD1306Wire(addr, sda, scl);
      myscreen->init();
      myscreen->setFont(ArialMT_Plain_16);
      myscreen->setFont(ArialMT_Plain_16);
  };
  
  ~myScreen() {
    delete myscreen;
  }

  void debug(uint32_t, bool);

private:
  SSD1306Wire *myscreen;
  byte addr, sda, scl;
}; // class myScreen

#endif // MYSCREEN_H

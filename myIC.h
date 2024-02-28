#ifndef MYIC_H
#define MYIC_H

#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

class myIC {
public:
  myIC(byte ss,byte rst);
  myIC(myIC & obj);
  ~myIC();

  bool readyet();
  uint32_t read();

private:
  MFRC522 *my_rfid;
  byte ss, rst;
}; // class myIC

#endif // MYIC_H
 
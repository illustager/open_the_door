#include "myIC.h"

myIC::myIC(byte ss, byte rst) {
  this->ss = ss;
  this->rst = rst;

  this->my_rfid = new MFRC522( ss, rst );
  SPI.begin();                 // 初始化 SPI 总线
  this->my_rfid->PCD_Init();   // 初始化 MFRC522
}

myIC::myIC(myIC & obj) {
  this->ss = obj.ss;
  this->rst = obj.rst;

  this->my_rfid = new MFRC522( obj.ss, obj.rst );
  SPI.begin();                // 初始化 SPI 总线
  this->my_rfid->PCD_Init();
}

myIC::~myIC() {
  if( this->my_rfid )
    delete this->my_rfid;
}

bool myIC::readyet() {
  return (my_rfid->PICC_IsNewCardPresent() && my_rfid->PICC_ReadCardSerial());
}

uint32_t myIC::read() {
  uint32_t data = 0;
  for (byte i = 0; i < 4; i++) {
    data <<= 8;
    data |= my_rfid->uid.uidByte[i];
  }
  return data;
}

  // 使放置在读卡区的IC卡进入休眠状态，不再重复读卡
  //rfid.PICC_HaltA();
  // 停止读卡模块编码
  //rfid.PCD_StopCrypto1();

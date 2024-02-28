#include "myServo.h"

myServo::myServo(byte pin, unsigned minUs = 500, unsigned maxUs = 2500) : servoPin(pin), minUs(minUs), maxUs(maxUs) {
  ESP32PWM::allocateTimer(0); // 如有需要可以将四个计时器全部开启 此处仅开启 0 计时器 在 ESP32Servo 中的 ESP32PWM.cpp 中可以查看相关事项
  this->myservo.setPeriodHertz(50);
  this->myservo.attach(this->servoPin, this->minUs, this->maxUs); // 舵机连接到指定引脚
};

myServo::~myServo() {
  // this->myservo.detach();
}

void myServo::work(int angle) {
  this->myservo.write(angle);
}

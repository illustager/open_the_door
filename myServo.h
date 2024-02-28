#ifndef MYSERVO_H
#define MYSERVO_H

#include <ESP32Servo.h>

class myServo {
public:
  myServo(byte pin, unsigned minUs = 500, unsigned maxUs = 2500);
  
  ~myServo();

  void work(int);

private:
  Servo myservo;
  byte servoPin;
  unsigned minUs;
  unsigned maxUs;

  void init();
}; // class myServo

#endif // MYSERVO_H

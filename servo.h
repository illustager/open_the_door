
class mc_Servo {
public:
  void m_servo_init();
  void m_servo_work(int);
  // Servo &m_get_myservo();
private:
  Servo myservo;
};


void mc_Servo::m_servo_init() {
  ESP32PWM::allocateTimer(0);//如有需要可以将四个计时器全部开启，此处仅开启0计时器。在ESP32Servo中的ESP32PWM.cpp中可以查看相关事项
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, minUs, maxUs); // 舵机连接到指定引脚
}

void mc_Servo::m_servo_work(int angle) {
  myservo.write(angle);
}













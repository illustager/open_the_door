#include "statusinfo.h"

#include <Arduino.h>

// ------------------------------ LED config ------------------------------
#define LEDPin 	 2 	 // 指示灯引脚

#define LEDoff   0	 // 关闭时的 PWM 值
#define LEDinfo1 25  // 一级信号 唤醒时
#define LEDinfo2 255 // 二级信号 开门时 
// ------------------------------ LED config ------------------------------

namespace statusinfo {
	void init() {
	#ifdef DEBUG
		Serial.begin(115200);
	#endif
		pinMode(LEDPin, OUTPUT);
		analogWrite(LEDPin, LEDoff);
	}

	void wakeup() {
	#ifdef DEBUG
		Serial.println("Wakeup");
	#endif
		analogWrite(LEDPin, LEDinfo1);
	}

	void working() {
		analogWrite(LEDPin, LEDinfo2);
	}

	void sleep() {
	#ifdef DEBUG
		Serial.println("ESP32 will sleep now!");
	#endif
		analogWrite(LEDPin, LEDoff);
	}
} // namespace statusinfo

#include "statusinfo.h"

#include <Arduino.h>

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

#include "lowpower.h"

#include <Arduino.h>
#include <esp_sleep.h>

// ------------------------------ Touch config ------------------------------
#define touchPin  		33 		// 触摸引脚
#define touchThreshold	5 		// 触摸阈值
// ------------------------------ Touch config ------------------------------

namespace lowpower {
	void init() {
		esp_sleep_enable_touchpad_wakeup();
	}

	void start() {
		touchAttachInterrupt(touchPin, callback, touchThreshold);
		esp_deep_sleep_start();
	}

	void callback() {
		// Serial.println("good");
	}
} // namespace sleep

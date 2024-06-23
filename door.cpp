#include "door.h"

#include <myServo.h>

// ------------------------------ Servo Config ------------------------------
#define servoPin 		4       // 舵机引脚

#define servoMinPulse 	500 	// 脉冲宽度范围 一般不用改
#define servoMaxPulse 	2500

#define servoDelayTime 	3 		// 秒

#define servoOpenAngle  130		// 舵机开关门的角度
#define servoCloseAngle 0
// ------------------------------ Servo Config ------------------------------

static myServo my_servo(servoPin, servoMinPulse, servoMaxPulse);

namespace door {
	void open() {
		my_servo.work(servoOpenAngle);
	}

	void close() {
		my_servo.work(servoCloseAngle);
	}
} // namespace door

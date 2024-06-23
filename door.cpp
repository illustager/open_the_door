#include "door.h"

#include <myServo.h>

static myServo my_servo(servoPin, servoMinPulse, servoMaxPulse);

namespace door {
	void open() {
		my_servo.work(servoOpenAngle);
	}

	void close() {
		my_servo.work(servoCloseAngle);
	}
} // namespace door

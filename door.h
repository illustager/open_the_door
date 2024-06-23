#pragma once

namespace door {
	void open();
	void close();
} // namespace door

// ------------------------------ Servo Config ------------------------------
#define servoPin 		4       // 舵机引脚

#define servoMinPulse 	500 	// 脉冲宽度范围 一般不用改
#define servoMaxPulse 	2500

#define servoDelayTime 	3 		// 秒

#define servoOpenAngle  130		// 舵机开关门的角度
#define servoCloseAngle 0
// ------------------------------ Servo Config ------------------------------

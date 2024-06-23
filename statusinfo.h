#pragma once

namespace statusinfo {
	void init();
	void wakeup();
	void working();
	void sleep();
} // namespace statusinfo

// ------------------------------ LED config ------------------------------
#define LEDPin 	 2 	 // 指示灯引脚

#define LEDoff   0	 // 关闭时的 PWM 值
#define LEDinfo1 25  // 一级信号 唤醒时
#define LEDinfo2 255 // 二级信号 开门时 
// ------------------------------ LED config ------------------------------

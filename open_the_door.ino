//------------------------------------------------------------配置
#include "config.h"

//------------------------------------------------------------舵机
#include <myServo.h>
myServo my_servo(servoPin, servoMinPulse, servoMaxPulse);

//------------------------------------------------------------读卡器
#include <myIC.h>
myIC my_ic(rc522_SS_PIN,rc522_RST_PIN);

//------------------------------------------------------------音频
#include <myAudio.h>
myAudio my_audio(audioPin, audioNum, audioDatas);

volatile bool is2play; 			// volatile !!!!!!!!!! 用于多线程之间的通信
TaskHandle_t playHandle = NULL; // 多线程句柄

void playTask(void*) { // 多线程任务函数 - 播放音频
	static RTC_DATA_ATTR byte select = 0;
	while(true) {
		if( is2play ) {
			my_audio.play(select);
			is2play = false;
			++select;
		}
	}
#ifdef DEBUG
	Serial.println("sound");
#endif
}

//------------------------------------------------------------休眠
#include <esp_sleep.h>

void callbackFunc() { // 回调函数
	// Serial.println("good");
}

//------------------------------------------------------------键盘和管理系统
#include <Keypad.h>
#include "manage.h"

unsigned times4wake PROGMEM = 0;

Keypad kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//------------------------------------------------------------


void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif

	// while(true) {
	// 	Serial.println(touchRead(touchPin));
	// 	delay(500);
	// }

	esp_sleep_enable_touchpad_wakeup();
	// 在 esp32 中 断分为两种 写入不同位置
	// 一种是 RTC 中断 如定时器中断 触摸中断
	// 另一种是 GPIO 中断
	touchAttachInterrupt(touchPin, callbackFunc, threshold); // 引脚 回调函数 阈值
	touchDetachInterrupt(touchPin); // 先解除中断 而后在 loop 中重新启用中断 以免多次触发中断

	pinMode(LEDPin, OUTPUT);
	analogWrite(LEDPin, LEDoff); // digitalWrite(LEDPin, LOW);

	my_servo.work(servoCloseAngle); // 初始化舵机

	// xTaskCreate(  playTask,         // 任务函数
	// 			  "play",           // 任务名称
	// 			  8*1024,           // 任务栈大小，根据需要自行设置
	// 			  NULL,             // 参数 入参为空
	// 			  1,                // 优先级
	// 			  &playHandle );    // 任务句柄

	++times4wake;
	if( times4wake == 1 ) {
		loadUserData();
	}

} // setup

void loop() {

// 实现 esp32 在唤醒后维持一段时间的运行
#ifdef DEBUG
	Serial.println("Working...");
#endif
	analogWrite(LEDPin, LEDinfo1);

	uint64_t startTime = millis();
	while( millis() - startTime < wakeupTime * 1000 ) {
		if( kpd.getKey() == 'D' ) {
			manage(&kpd, &my_ic);
		}

		if( my_ic.readyet() ) {
			uint32_t uid = my_ic.read();
			int check_flag = checkUserData(uid);

			#ifdef DEBUG
				Serial.println(uid);
				Serial.println(check_flag);
			#endif

			if( check_flag >= 0 ) {
			#ifdef DEBUG
				Serial.println("Welcome!");
			#endif
				analogWrite(LEDPin, LEDinfo2);
				is2play = true;
			//---------------------------------------------------
				my_servo.work(servoOpenAngle);
				delay(servoDelayTime * 1000);
			//-----------------------------------------
				break;
			}
		}
	}

	
#ifdef DEBUG
	Serial.println("ESP32 will sleep now!");
#endif

	is2play = false;
	my_servo.work(servoCloseAngle);

	delay(delayTime);

	analogWrite(LEDPin, LEDoff);
	
	touchAttachInterrupt(touchPin, callbackFunc, threshold);
	esp_deep_sleep_start();
} // loop

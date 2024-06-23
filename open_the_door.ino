//------------------------------------------------------------配置
#include "config.h"

//------------------------------------------------------------舵机
#include "door.h"
#include "statusinfo.h"
//------------------------------------------------------------读卡器
#include <myIC.h>
myIC my_ic(rc522_SS_PIN,rc522_RST_PIN);

//------------------------------------------------------------音频
#include "play.h"
volatile bool working; 			// volatile !!!!!!!!!! 用于多线程之间的通信
TaskHandle_t taskHandle = NULL; // 多线程句柄

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
	// 设置睡眠和触摸唤醒
	esp_sleep_enable_touchpad_wakeup();
	touchAttachInterrupt(touchPin, callbackFunc, touchThreshold);
	touchDetachInterrupt(touchPin);

	// 设置指示灯
	statusinfo::init();

	// 关门（复位）
	door::close();

	working = false;

	// 开门时播放音频
	xTaskCreate(  play::task,       // 任务函数
				  "play",           // 任务名称
				  8*1024,           // 任务栈大小，根据需要自行设置
				  (void*)&working,  // 参数 入参为空
				  1,                // 优先级
				  &taskHandle );    // 任务句柄

	// 第一次启动时加载用户数据
	++times4wake;
	if( times4wake == 1 ) {
		manage::loadUserData();
	}

} // setup

void loop() {
	// 指示灯提示 ESP32 唤醒
	statusinfo::wakeup();

	uint64_t startTime = millis();
	while( millis() - startTime < wakeupTime * 1000 ) { // 工作 wakeupTime 秒
		if( kpd.getKey() == 'D' ) {
			manage::manage(&kpd, &my_ic); // 进入用户管理系统
		}

		if( my_ic.readyet() ) {
			uint32_t uid = my_ic.read();
			int check_flag = manage::checkUserData(uid, NULL);

			#ifdef DEBUG
				Serial.println(uid);
				Serial.println(check_flag);
			#endif

			if( check_flag >= 0 ) {
			#ifdef DEBUG
				Serial.println("Welcome!");
			#endif

				// 开门 随后退出
				statusinfo::working();
				working = true;
				door::open();
				delay(servoDelayTime * 1000);

				break;
			}
		}
	}

	working = false;
	door::close();
	delay(delayTime);
	
	statusinfo::sleep();

	touchAttachInterrupt(touchPin, callbackFunc, touchThreshold);
	esp_deep_sleep_start();
} // loop

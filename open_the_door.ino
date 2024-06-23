//------------------------------------------------------------配置
#include "config.h"

//------------------------------------------------------------舵机
#include "door.h"

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
	// 设置睡眠和触摸唤醒
	esp_sleep_enable_touchpad_wakeup();
	touchAttachInterrupt(touchPin, callbackFunc, touchThreshold);
	touchDetachInterrupt(touchPin);

	// 设置指示灯
	pinMode(LEDPin, OUTPUT);
	analogWrite(LEDPin, LEDoff); // digitalWrite(LEDPin, LOW);

	// 关门（复位）
	door::close();

	// 开门时播放音频
	xTaskCreate(  playTask,         // 任务函数
				  "play",           // 任务名称
				  8*1024,           // 任务栈大小，根据需要自行设置
				  NULL,             // 参数 入参为空
				  1,                // 优先级
				  &playHandle );    // 任务句柄

	// 第一次启动时加载用户数据
	++times4wake;
	if( times4wake == 1 ) {
		manage::loadUserData();
	}

} // setup

void loop() {
#ifdef DEBUG
	Serial.println("Working...");
#endif
	// 指示灯提示 ESP32 唤醒
	analogWrite(LEDPin, LEDinfo1);

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
				analogWrite(LEDPin, LEDinfo2);
				is2play = true;
				door::open();
				delay(servoDelayTime * 1000);

				break;
			}
		}
	}

	is2play = false;
	door::close();
	delay(delayTime);
	analogWrite(LEDPin, LEDoff);
	
#ifdef DEBUG
	Serial.println("ESP32 will sleep now!");
#endif
	touchAttachInterrupt(touchPin, callbackFunc, touchThreshold);
	esp_deep_sleep_start();
} // loop

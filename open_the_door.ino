//------------------------------------------------------------舵机
#include "door.h"

#define openDelayTime 	1 	// s 开门所需时间

//------------------------------------------------------------指示灯
#include "statusinfo.h"

//------------------------------------------------------------音频
#include "play.h"
volatile bool working;			// 开门时为 true
TaskHandle_t taskHandle = NULL; // 多线程句柄

//------------------------------------------------------------休眠
#include "lowpower.h"
#define wakeupTime 		10 		// s 醒来之后的工作时间
#define sleepDelayTime	1500  	// ms 开完门之后的延时时间 时间过短则舵机来不及恢复到初始位置 

//------------------------------------------------------------键盘和读卡器
#include <myIC.h>
#define rc522_SS_PIN  	12
#define rc522_RST_PIN 	27
myIC my_ic(rc522_SS_PIN,rc522_RST_PIN);

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {32, 15, 17, 5};
byte colPins[COLS] = {13, 14, 26, 16};
char keys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

Keypad kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//------------------------------------------------------------管理系统
#include "manage.h"
unsigned RTC_DATA_ATTR times4wake = 0; // 记录唤醒次数

//------------------------------------------------------------

void setup() {
	// 设置睡眠和触摸唤醒
	lowpower::init();

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
	while( millis() - startTime < wakeupTime * 1000 )
	{ // 工作 wakeupTime 秒
		if( kpd.getKey() == 'D' ) {
			manage::manage(&kpd, &my_ic); // 进入用户管理系统
		}

		if( my_ic.readyet() ) {
			uint32_t uid = my_ic.read();
			int check_flag = manage::checkUserData(uid, NULL);

			if( check_flag >= 0 ) {
				// 开门 随后退出
				statusinfo::working();
				working = true;
				door::open();
				delay(openDelayTime * 1000);

				break;
			}
		}
	}

	working = false;
	door::close();
	delay(sleepDelayTime);
	
	statusinfo::sleep();

	lowpower::start();
} // loop

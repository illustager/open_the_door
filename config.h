#ifndef CONFIG_H
#define CONFIG_H

//------------------------------------------------------------DEBUG 模式
#define DEBUG

//------------------------------------------------------------舵机
#define servoPin 		4       // 舵机引脚

#define servoMinPulse 	500 	// 脉冲宽度范围 一般不用改
#define servoMaxPulse 	2500

#define servoDelayTime 	3 		// 秒

#define servoOpenAngle  100		// 舵机开关门的角度
#define servoCloseAngle 0

//------------------------------------------------------------读卡器与用户数据
// SS 和 RST 已固定 不要随便更改
// MISO 和 MOSI 以及其它引脚只需按照引脚参考图正确连接即可 这些引脚是自动工作的
#define rc522_SS_PIN  	12
#define rc522_RST_PIN 	27

// 用户数据见 user_data.h

//------------------------------------------------------------音频
#include "sound_data.h" // 音频数据
const byte *sound_datas[] = {	sound_data::cn,
							  	sound_data::en,
							  	sound_data::jp,
							  	sound_data::russian,
							  	sound_data::guandong,
							  	sound_data::sn,
							  	sound_data::shanghai,
							  	sound_data::eg	};

#define audioPin 	25 			// 音频引脚
#define audioNum 	8  			// 音频文件数量
#define audioDatas 	sound_datas	// 音频数据

//------------------------------------------------------------睡眠
#define touchPin  	33 		// 触摸引脚
#define threshold 	30 		// 触摸阈值
#define wakeupTime 	10 		// 秒 醒来之后的工作时间
#define delayTime 	1500  	// 毫秒 开完门之后的延时时间 时间过短则舵机来不及恢复到初始位置 

//------------------------------------------------------------指示灯
#define LEDPin 	 2 	 // 指示灯引脚

#define LEDoff   0	 // 关闭时的 PWM 值
#define LEDinfo1 25  // 一级信号 唤醒时
#define LEDinfo2 255 // 二级信号 开门时 

//------------------------------------------------------------

#endif // CONFIG_H

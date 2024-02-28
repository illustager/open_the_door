#define DEBUG

//-----------------------------------------------------------舵机
#include "servo.h"
myServoClass my_servo(4, 500, 2500); // 舵机引脚 以及最小最大脉冲宽度

#define servo_delay_time 3 // 此处单位为 s

//------------------------------------------------------------读卡器与用户数据
// SS 和 RST 已固定 不要随便更改
// MISO 和 MOSI 以及其它引脚只需按照引脚参考图正确连接即可 这些引脚是自动工作的
#define rc522_SS_PIN  12
#define rc522_RST_PIN 27 // 重置引脚

#include "IC_read.h"
myIC my_ic(rc522_SS_PIN,rc522_RST_PIN);

#include "user_data.h"

//-----------------------------------------------------------------------4脚 OLED
// #define SDA   21
// #define SCL   22

// #include "4_pin_screen.h"
// myScreenClass my_screen(0x3c, SDA, SCL);

//--------------------------------------------------------------------音频
#include "play.h"
myAudioClass my_audio(25); // 音频引脚

volatile bool is2play; // volatile !!!!!!!!!! 用于多线程之间的通信
TaskHandle_t playHandle = NULL; // 多线程句柄

void playTask(void*) { // 多线程任务函数 - 播放音频
  while(true) {
    if( is2play ) {
      my_audio.play();
      is2play = false;
    }
  }
}

//------------------------------------------------------------休眠
#include <esp_sleep.h>
// RTC_DATA_ATTR int bootCount = 0;
#define touchPin  33
#define threshold 30
#define wakeup_work_time 10 // 此处单位为 s

void callbackFun() { // 回调函数 必须实现
  // Serial.println("good");
}

//------------------------------------------------------------指示灯
#define LEDPin 2

//-------------------------------------------------------------------


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif

  esp_sleep_enable_touchpad_wakeup();
  // 在 esp32 中 断分为两种 写入不同位置
  // 一种是 RTC 中断 如定时器中断 触摸中断
  // 另一种是 GPIO 中断
  touchAttachInterrupt(touchPin, callbackFun, threshold); // 引脚 回调函数 阈值
  touchDetachInterrupt(touchPin); // 先解除中断 而后在 loop 中重新启用中断 以免多次触发中断

  pinMode(LEDPin, OUTPUT);

  xTaskCreate(  playTask,         /* 任务函数 */
                "play",           /* 任务名 */
                8*1024,           /* 任务栈大小，根据需要自行设置*/
                NULL,             /* 参数，入参为空 */
                1,                /* 优先级 */
                &playHandle );    /* 任务句柄 */

}

void loop() {

  // 实现 esp32 在唤醒后维持一段时间的运行

  Serial.println("Running...");

  uint64_t startTime = millis();
  while( millis() - startTime < wakeup_work_time * 1000 ) {
    if( my_ic.readyet() ) {
      uint32_t uid = my_ic.read();
      bool check_flag = user_data::check(uid);
      // my_screen.debug(uid, check_flag);
      if(check_flag) {
        Serial.println("Welcome!");
        digitalWrite(LEDPin, HIGH);
        is2play = true;
        //---------------------------------------------------
        my_servo.work(135);
        delay(servo_delay_time * 1000);
        //-----------------------------------------------------
        my_servo.work(0);
    
        //-----------------------------------------
        digitalWrite(LEDPin, LOW);
        break;
      }
    }
  }

  is2play = false;
#ifdef DEBUG
  Serial.println("ESP32 will sleep now!");
#endif
  delay(100);
  touchAttachInterrupt(touchPin, callbackFun, threshold);
  esp_deep_sleep_start();
}






//-----------------------------------------------------------舵机
//指定PWM脉冲宽度，最小为0.5ms，最大为2.5ms
#define minUs 500
#define maxUs 2500
#define servo_delay_time 3//此处单位为秒(s)
const byte servoPin = 4;
#include <ESP32Servo.h>
#include "servo.h"
mc_Servo my_servo;
//------------------------------------------------------------读卡器
#include <SPI.h>
#include <MFRC522.h>
//SS和RST已固定，不要随便更改。MISO和MOSI以及其它引脚只需按照引脚参考图正确连接即可，这些引脚是自动工作的
#define rc522_SS_PIN 12
#define rc522_RST_PIN 27//重置引脚
#include "user_data.h"
#include "IC_read_check.h"
mc_IC my_ic(rc522_SS_PIN,rc522_RST_PIN);
//-----------------------------------------------------------------------4脚OLED
#define SDA   21
#define SCL   22
#include <SSD1306Wire.h>
#include "4_pin_screen.h"
RTC_DATA_ATTR byte screen_work_cont = 0;
//--------------------------------------------------------------------
volatile bool is2play;
TaskHandle_t play_Handle = NULL;//多线程句柄
#define play_PIN 25
#include <XT_DAC_Audio.h>
#include "play.h"

//------------------------------------------------------------休眠
#include <esp_sleep.h>
// RTC_DATA_ATTR int bootCount = 0;//写入RTC的数据修饰为RTC_DATA_ATTR
#define touchPin 33
#define threshold 30
#define wakeup_work_time 10//此处单位为秒(s)
//回调函数，必须实现
void callbackFun(){
  // Serial.println("good");
}
//-------------------------------------------------------------------


void setup() {
  Serial.begin(115200);
  esp_sleep_enable_touchpad_wakeup();
  //在esp32中，中断分为两种，写入不同位置。一种是RTC中断，如定时器中断、触摸中断；另一种是GPIO中断。
  touchAttachInterrupt(touchPin, callbackFun, threshold);//引脚，回调函数，阈值
  touchDetachInterrupt(touchPin);//先解除中断，而后在loop中重新启用中断，以免多次触发中断

  pinMode(2, OUTPUT);

  my_servo.m_servo_init();
  myscreen_init();

  initPlay();
  xTaskCreate(  play,              /* 任务函数 */
                "play",            /* 任务名 */
                8*1024,            /* 任务栈大小，根据需要自行设置*/
                NULL,              /* 参数，入参为空 */
                1,                 /* 优先级 */
                &play_Handle );    /* 任务句柄 */

}

void loop() {

  //实现esp32在唤醒后维持一段时间的运行
  uint64_t startTime = millis();
  while(millis() - startTime < wakeup_work_time * 1000){
    if( my_ic.m_IC_readyet() ) {
      uint32_t uid = my_ic.m_IC_read();
      bool check_flag = my_ic.m_IC_check(uid, all_user);
      myscreen_print(uid, check_flag, screen_work_cont);
      if(check_flag) {
        digitalWrite(2, HIGH);
        is2play = true;
        //---------------------------------------------------
        my_servo.m_servo_work(135);
        delay(servo_delay_time * 1000);
        //-----------------------------------------------------
        my_servo.m_servo_work(0);
    
        //-----------------------------------------
        digitalWrite(2, LOW);
        break;
      }
    }
  }

  is2play = false;
  Serial.println("ESP32 will sleep now!");
  delay(100);
  touchAttachInterrupt(touchPin, callbackFun, threshold);
  esp_deep_sleep_start();
}





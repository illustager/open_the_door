// #include "HardwareSerial.h"

SSD1306Wire myscreen(0x3c, SDA, SCL);

void myscreen_init(){
  // 初始化OLED显示
  myscreen.init();
  // myscreen.flipScreenVertically(); // 垂直翻转屏幕设置
  myscreen.setFont(ArialMT_Plain_16); // 设置字体大小

}

void myscreen_print(uint32_t uid, bool flag, byte cont){
  // RTC_DATA_ATTR byte cont = 0;
  char buff[100] = {};
  sprintf( buff, "%d", flag);
  sprintf( buff+2, "%d", cont);
  sprintf( buff+20, "%u", uid);

  // 在OLED屏幕上显示天气信息
  myscreen.clear();
  myscreen.drawString(0, 10, buff);
  myscreen.drawString(0, 30, buff+2);
  myscreen.drawString(0, 50, buff+20);
  myscreen.display();

  cont++;
}


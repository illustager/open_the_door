#include "myScreen.h"

void myScreen::debug(uint32_t uid, bool flag){
  static RTC_DATA_ATTR byte cnt = 0;

  char buff[100] = {};
  sprintf( buff, "%d", flag);
  sprintf( buff+2, "%d", cnt);
  sprintf( buff+20, "%u", uid);

  // 在 OLED 屏幕上显示天气信息
  myscreen->clear();
  myscreen->drawString(0, 10, buff);
  myscreen->drawString(0, 30, buff+2);
  myscreen->drawString(0, 50, buff+20);
  myscreen->display();

  ++cnt;
}

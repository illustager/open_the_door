#include "myScreen.h"

myScreen::myScreen(byte addr, byte sda, byte scl) : addr(addr), sda(sda), scl(scl) {
	myscreen = new SSD1306Wire(addr, sda, scl);
	myscreen->init();
	myscreen->setFont(ArialMT_Plain_16);
	myscreen->setFont(ArialMT_Plain_16);
};

myScreen::~myScreen() {
	delete myscreen;
}

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

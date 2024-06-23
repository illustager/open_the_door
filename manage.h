#include <Keypad.h>
#include <myIC.h>

namespace manage {
	void manage(Keypad* kpd, myIC* my_ic);
	void loadUserData();
	int checkUserData(uint32_t uid, uint32_t* data);
} // manage


#include <Keypad.h>
#include <myIC.h>

void manage(Keypad* kpd, myIC* my_ic);
void loadUserData();
int checkUserData(uint32_t uid);

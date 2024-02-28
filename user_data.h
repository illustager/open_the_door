// #include "HardwareSerial.h"
#define user_num 3

struct user{
  char name[10];
  uint32_t id;
};

struct user all_user[user_num] = {
  {"唐宇坤", 0xDED0135F},
  {"杨东霖", 0xAE2B175F},
  {"刘冰洋", 0x2EB7115F}
};

// byte check_user(uint32_t scan_info, struct user* all_user){
//   for(byte i =0;i < user_num;i++){
//     if(all_user[i].id == scan_info)  return i;
//   }
//   return user_num+1;
// }








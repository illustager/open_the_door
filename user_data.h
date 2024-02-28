namespace user_data{

  #define user_num 3

  struct user {
    char name[10];
    uint32_t id;
  };

  struct user all_user[user_num] = {
    {"唐宇坤", 0xDED0135F},
    {"杨东霖", 0xAE2B175F},
    {"刘冰洋", 0x2EB7115F}
  };

  bool check(uint32_t uid){
  for(byte i =0;i < user_num;i++){
    if(all_user[i].id == uid)  return true;
  }
  return false;
}

} // namespace user_data


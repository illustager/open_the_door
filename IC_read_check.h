class mc_IC {
public:
  mc_IC(byte ss,byte rst);
  mc_IC(mc_IC & obj);
  ~mc_IC();
  //--------------------------
  // void m_IC_init();
  bool m_IC_readyet();
  uint32_t m_IC_read();
  bool m_IC_check(uint32_t, struct user*);

private:
  MFRC522 *my_rfid;
  byte ss, rst;
  //存储读取的NUID
  // uint32_t uid = 0;

};

mc_IC::mc_IC(byte ss, byte rst) {
  this->ss = ss;
  this->rst = rst;

  this->my_rfid = new MFRC522( ss, rst );
  SPI.begin();                // 初始化SPI总线
  this->my_rfid->PCD_Init();   // 初始化 MFRC522
}

mc_IC::mc_IC(mc_IC & obj) {
  this->ss = obj.ss;
  this->rst = obj.rst;

  this->my_rfid = new MFRC522( obj.ss, obj.rst );
  SPI.begin();                // 初始化SPI总线
  this->my_rfid->PCD_Init();
}

mc_IC::~mc_IC() {
  if( this->my_rfid )
    delete this->my_rfid;
}

// void mc_IC::m_IC_init() {
// }

bool mc_IC::m_IC_readyet() {
  return (my_rfid->PICC_IsNewCardPresent() && my_rfid->PICC_ReadCardSerial());
}

uint32_t mc_IC::m_IC_read() {
  uint32_t data = 0;
  for (byte i = 0; i < 4; i++) {
    data <<= 8;
    data |= my_rfid->uid.uidByte[i];//唯一重要代码！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
  }
  return data;
}


bool mc_IC::m_IC_check(uint32_t uid, struct user* all_user){
  for(byte i =0;i < user_num;i++){
    if(all_user[i].id == uid)  return true;
  }
  return false;
}



  // 使放置在读卡区的IC卡进入休眠状态，不再重复读卡
  //rfid.PICC_HaltA();
  // 停止读卡模块编码
  //rfid.PCD_StopCrypto1();

 
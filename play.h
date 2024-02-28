#include <XT_DAC_Audio.h>

#include "soundData.h"

class myAudioClass {
public:
  myAudioClass(byte pin, volatile bool *is2play) 
    : playPIN(pin), ptr_is2play(ptr_is2play) {
      DacAudio = new XT_DAC_Audio_Class(playPIN,0);
      this->init();
  };
  
  ~myAudioClass() {
    for( int i = 0; i < 8; i++ ) {
      delete pSound[i];
    }
    delete DacAudio;
  }

  void play(void*);

private:
  byte playPIN;
  volatile bool *ptr_is2play;
  XT_Wav_Class *pSound[8];
  XT_DAC_Audio_Class *DacAudio;

  void init();
};

void myAudioClass::init() {
  digitalWrite( playPIN, LOW );

  randomSeed( analogRead(0) );
  // 裸机程序 应该不需要 delete 吧
  pSound[0] = new XT_Wav_Class(russian);
  pSound[1] = new XT_Wav_Class(guandong);
  pSound[2] = new XT_Wav_Class(cn);
  pSound[3] = new XT_Wav_Class(en);
  pSound[4] = new XT_Wav_Class(jp);
  pSound[5] = new XT_Wav_Class(sn);
  pSound[6] = new XT_Wav_Class(shanghai);
  pSound[7] = new XT_Wav_Class(eg);
}

void myAudioClass::play(void*) {
  byte select = 0;

  while( true ) {
    
    if( !*ptr_is2play ) continue;

    *ptr_is2play = false;
    
    select = random(8);
    DacAudio->Play( pSound[select] );

    while( pSound[select]->Playing ) {
      DacAudio->FillBuffer();
    }

#ifdef DEBUG
    Serial.println("voice");
#endif

    break;
  }

  return;
}

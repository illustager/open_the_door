#include <XT_DAC_Audio.h>

#include "soundData.h"

class myAudioClass {
public:
  myAudioClass(byte pin) : playPIN(pin) {
      DacAudio = new XT_DAC_Audio_Class(playPIN,0);
      this->init();
  };
  
  ~myAudioClass() {
    for( int i = 0; i < 8; i++ ) {
      delete pSound[i];
    }
    delete DacAudio;
  }

  void play();

private:
  byte playPIN;
  XT_Wav_Class *pSound[8];
  XT_DAC_Audio_Class *DacAudio;

  void init();
};

void myAudioClass::init() {
  digitalWrite( playPIN, LOW );

  pSound[0] = new XT_Wav_Class(russian);
  pSound[1] = new XT_Wav_Class(guandong);
  pSound[2] = new XT_Wav_Class(cn);
  pSound[3] = new XT_Wav_Class(en);
  pSound[4] = new XT_Wav_Class(jp);
  pSound[5] = new XT_Wav_Class(sn);
  pSound[6] = new XT_Wav_Class(shanghai);
  pSound[7] = new XT_Wav_Class(eg);
}

void myAudioClass::play() {
  static RTC_DATA_ATTR byte select = 0;

  select = (select + 1) % 8;

  DacAudio->Play( pSound[select] );

  while( pSound[select]->Playing ) {
    DacAudio->FillBuffer();
  }

#ifdef DEBUG
  Serial.println("voice");
#endif

  return;
}

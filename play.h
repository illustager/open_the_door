// #include "esp32-hal-gpio.h"
#include "soundData.h"

XT_Wav_Class *pSound[8];
XT_DAC_Audio_Class DacAudio(play_PIN,0);

void initPlay() {
  digitalWrite( play_PIN, LOW );

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

void play(void*) {
  byte select = 0;

  while( true ) {
    
    if( !is2play ) continue;

    is2play = false;
    
    select = random(8);
    DacAudio.Play( pSound[select] );

    while( pSound[select]->Playing ) {
      DacAudio.FillBuffer();
    }

    Serial.println("voice");

    break;
  }
  vTaskDelete( play_Handle );
  return;
}

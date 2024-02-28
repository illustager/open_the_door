#ifndef MYAUDIO_H
#define MYAUDIO_H

#include <XT_DAC_Audio.h>

class myAudio {
public:
  myAudio(byte pin);
  ~myAudio();

  void play();

private:
  byte playPIN;
  XT_Wav_Class *pSound[8];
  XT_DAC_Audio_Class *DacAudio;
}; // class myAudio

#endif // MYAUDIO_H

#include "play.h"

#include <myAudio.h>

// ------------------------------ Audio config ------------------------------
#include "sound_data.h" // 音频数据
const byte *sound_datas[] = {	sound_data::cn,
							  	sound_data::en,
							  	sound_data::jp,
							  	sound_data::russian,
							  	sound_data::guandong,
							  	sound_data::sn,
							  	sound_data::shanghai,
							  	sound_data::eg	};

#define audioPin 	25 			// 音频引脚
#define audioNum 	8  			// 音频文件数量
#define audioDatas 	sound_datas	// 音频数据
// ------------------------------ Audio config ------------------------------

static myAudio my_audio(audioPin, audioNum, audioDatas);

namespace play {
	void task(void* pvParameters) {
		static RTC_DATA_ATTR byte select = 0;
		while(true) {
			if( *(bool*)pvParameters ) {
				my_audio.play(select);
				*(bool*)pvParameters = false;
				++select;
			}
		}
	#ifdef DEBUG
		Serial.println("sound");
	#endif
	}
} // namespace play

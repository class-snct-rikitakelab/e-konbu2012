
#include "Sound.h"

void Sound_init(Sound *this_Sound){
	//none
}
void Sound_soundTone(Sound *this_Sound, U32 freq, U32 ms, U32 vol){
	ecrobot_sound_tone(freq,ms,vol);
}
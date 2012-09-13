#ifndef __Sound
#define __Sound

#include "ecrobot_interface.h"

typedef struct {
}Sound;

void Sound_init(Sound *this_Sound);
void Sound_soundTone(Sound *this_Sound, U32 freq, U32 ms, U32 vol);

#endif
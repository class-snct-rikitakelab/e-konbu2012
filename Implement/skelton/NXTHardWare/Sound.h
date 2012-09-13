
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "U32.h"

namespace NXTHardWare
{
class Sound
{
public:
	void Sound_init(Sound *this_Sound);

	void Sound_soundTone(Sound *this_Sound, U32 freq, U32 ms, U32 vol);

};

}  // namespace NXTHardWare

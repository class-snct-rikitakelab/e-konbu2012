
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace StrategyPart
{
typedef enum 
{
	IDLE,
	FST_STRAIGHT,
	UP_SLOPE,
	DOWN_SLOPE,
	FST_CORNER,
	SND_STRIGHT,
	SND_CORNER,
	TRD_STRIGHT,
	TRD_CORNER,
	FORTH_STRAIGHT,
	FIN_APPROACH,
	MARKER,
	LOOKUP

} SectName;

}  // namespace StrategyPart

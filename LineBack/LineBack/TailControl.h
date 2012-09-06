#ifndef __TailControl
#define __TailControl

#include "config.h"

//K”ö‚Ìó‘Ô
typedef enum{
	TAIL_DOWN,		//K”ö~‰º
	TAIL_UP,		//K”öã¸
} TailState;

typedef struct{
	float Tail_Kp;	//”ä—áŒW”
	float Tail_Ki;	//Ï•ª’è”
	float Tail_Kd;	//”÷•ªŒW”
	TailState State;	//K”öó‘Ô
}TailControl;

#define ANGLEOFDOWN		95	//K”ö‘–s
#define ANGLEOFUP		5	//K”ö•sg—p
#define ANGLEOFLOOKUP	56	//ƒ‹ƒbƒNƒAƒbƒvƒQ[ƒg’Ê‰ß
#define ANGLEOFSTAND	100	//’¼—§ó‘Ô
#define ANGLEOFPUSH		120	//‘–s‘Ì’µ‚Ëã‚°

extern void TailControl_init(TailControl * this_TailControl);
extern void TailControl_PIDTailControl(TailControl * this_TailControl);
extern void TailControl_TailStateChange(TailControl * this_TailControl,TailState state);

#endif

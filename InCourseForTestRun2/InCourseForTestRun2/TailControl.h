#include "MainRunningInCourse.h"

#define ANGLEOFDOWN		95	//Kös
#define ANGLEOFUP		5	//Kösgp
#define ANGLEOFLOOKUP	56	//bNAbvQ[gÊß
#define ANGLEOFSTAND	100	//¼§óÔ
#define ANGLEOFPUSH		120	//sÌµËã°

//KöÌóÔ
typedef enum{
	RN_TAILDOWN,				//Kö~º
	RN_TAILUP,					//Köã¸
} RN_TAILMODE;

//KöPI§äpW
static float t_Kp = 1.85;			//P§äp
static float t_Ki = 0;				//I§äp

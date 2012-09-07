#include "MainRunningInCourse.h"

#define ANGLEOFDOWN		95	//尻尾走行
#define ANGLEOFUP		5	//尻尾不使用
#define ANGLEOFLOOKUP	56	//ルックアップゲート通過
#define ANGLEOFSTAND	100	//直立状態
#define ANGLEOFPUSH		120	//走行体跳ね上げ

//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;

//尻尾PI制御用係数
static float t_Kp = 1.85;			//P制御用
static float t_Ki = 0;				//I制御用

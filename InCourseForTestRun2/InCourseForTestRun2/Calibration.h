//#include "MainRunningInCourse.h"
#include "TailControl.h"
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
static unsigned int LOOKUP_BLACK_VALUE;		//角度がANGLEOFLOOKUP時の黒値
static unsigned int LOOKUP_WHITE_VALUE;		//角度がANGLEOFLOOKUP時の白値
static unsigned int LOOKUP_GRAY_VALUE;		//角度がANGLEOFLOOKUP地の灰色値（現在は黒と白の平均値）

extern void RN_calibrate(void);

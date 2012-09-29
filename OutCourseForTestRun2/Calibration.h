#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "TailControl.h"
#include "ecrobot_interface.h"
#include "RemoteStart.h"
#include "BatteryCheck.h"

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
static unsigned int LOOKUP_BLACK_VALUE;		//角度がANGLEOFLOOKUP時の黒値
static unsigned int LOOKUP_WHITE_VALUE;		//角度がANGLEOFLOOKUP時の白値
static unsigned int LOOKUP_GRAY_VALUE;		//角度がANGLEOFLOOKUP地の灰色値（現在は黒と白の平均値）

static unsigned int INIT_GYRO_OFFSET;	//キャリブレーション時ののジャイロオフセット値

//ジャイロセンサオフセット計算用変数
static U32 gyro_offset;    /* gyro sensor offset value */

extern int RN_calibrate(void);
extern U32 getGyroOffset();
extern U32 getInitGyroOffset();
extern void setGyroOffset(int setvalue);
extern unsigned int getGrayValue();
extern unsigned int getLookUpGrayValue();

#endif

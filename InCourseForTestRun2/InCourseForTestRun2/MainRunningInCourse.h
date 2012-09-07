
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

#include "math.h"
#include "DriftTurn.h"
#include "LightPIDControl.h"
#include "LogSend.h"
#include "LookUpGate.h"
#include "TailControl.h"

//尻尾設定角度
#define ANGLEOFDOWN 90 				//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度
#define ANGLEOFPUSH ANGLEOFDOWN+5				//押上目標角度（未使用）
#define ANGLEOFLOOKUP 56

#define PI 3.141592

//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)

/* 車輪半径、走行体幅*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
static unsigned int LOOKUP_BLACK_VALUE;		//角度がANGLEOFLOOKUP時の黒値
static unsigned int LOOKUP_WHITE_VALUE;		//角度がANGLEOFLOOKUP時の白値
static unsigned int LOOKUP_GRAY_VALUE;		//角度がANGLEOFLOOKUP地の灰色値（現在は黒と白の平均値）




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

//Kφέθpx
#define ANGLEOFDOWN 90 				//~ΊΪWpx
#define ANGLEOFUP 0					//γΈΪWpx
#define ANGLEOFPUSH ANGLEOFDOWN+5				//γΪWpxi’gpj
#define ANGLEOFLOOKUP 56

#define PI 3.141592

//WCUl
#define PM_GYRO 65

//ΤΦΜ~ό[cm]
#define CIRCUMFERENCE 25.8			//ΤΦΜ~ό

#define CMD_START '1'    			//[gX^[gR}h(ΟXΦ~)

/* ΤΦΌaAsΜ*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

//Cg[XpΪWl
static unsigned int BLACK_VALUE;	//l
static unsigned int WHITE_VALUE;	//l
static unsigned int GRAY_VALUE;		//DFli»έΝΖΜ½Οlj
static unsigned int LOOKUP_BLACK_VALUE;		//pxͺANGLEOFLOOKUPΜl
static unsigned int LOOKUP_WHITE_VALUE;		//pxͺANGLEOFLOOKUPΜl
static unsigned int LOOKUP_GRAY_VALUE;		//pxͺANGLEOFLOOKUPnΜDFli»έΝΖΜ½Οlj



#include "Factory.h"


void factory(){



	/****start Director part init****/
	
RunnerRobot_init(&mRunnerRobot);
Running_init(&mRunning);

	/****end Director part init****/

	


//Sector_init(&mSector);

/****start detectionPart part objects****/
Notice_init(&mNotice);
LocationInfo_init(&mLocationInfo);

Battery_init(&mBattery);
Coordinates_init(&mCoordinates);
DetcMarker_init(&mDetcMarker);
DetcObst_init(&mDetcObst);
Gradient_init(&mGradient);
LineEdge_init(&mLineEdge);
RunDist_init(&mRunDist);
RunStartSigRecv_init(&mRunStartSigRecv);
Timer_init(&mTimer);
TurnAngle_init(&mTurnAngle);

/****end dectionPart part objects****/



/****start Driven part init****/
CurvatureCtrl_init(&mCurvatureCtrl);
ForwardValRevise_init(&mForwardValRevise);
LightValCtrl_init(&mLightValCtrl);
RobotDrivenDirect_init(&mRobotDrivenDirect);
TailAngleCtrl_init(&mTailAngleCtrl);
WheelMotor_init(&rightWheelMotor,NXT_PORT_B);
WheelMotor_init(&leftWheelMotor,NXT_PORT_C);
WheelMotorCtrl_init(&mWheelMotorCtrl);
/****end Driven part init****/




/****start Strategy part init****/


//test object

/****end Strategy part init****/



/****start create NXT HarDware init****/
LightSensor_init(&mLightSensor);
TouchSensor_init(&mTouchSensor);
GyroSensor_init(&mGyroSensor);
Sound_init(&mSound);
MotorEncoder_init(&rightMotorEncoder,NXT_PORT_B);
MotorEncoder_init(&leftMotorEncoder,NXT_PORT_C);
SonorSensor_init(&mSonorSensor,NXT_PORT_S2);
Timer_init(&mTimer);

/****end create NXT HarDware init****/
}



void makeSectors(){
		//Sector idleSector;
//Sector fstStraightSector;

TargetDrivenParm mTargetDrivenParm;
CngSectTerm mCngSecTerm[CNG_TERM_MAX_NUM];

/*___________________MAKE IDLE ___________________________*/

/* data format
 void TargetDrivenParm_init(TargetDrivenParm *this_TargetDrivenParm,
	S8 targForwardVal,float curvature,RUN_MODE runMode,int tailAngle,U16 targLightVal,TARG_CTRL_METHOD targCtrlMethod,U16 whiteVal,U16 blackVal,U16 volt,U16 gyroOffset);
	*/
 TargetDrivenParm_init(&mTargetDrivenParm,0,0,TAIL_RUNNING,95,0,HYBRID_PID,0,0,0,0);

 /* data format
 void CngSectTerm_init(CngSectTerm *this_CngSectTerm,SectName nextSectName1,BOOL jBT1,int targDist1,BOOL jLineEdge1,U16 targGradient1,
 S32 targTime1,float targXCoo1,float targYCoo1,U16 makerhreshold1,int targObstDist1,int targTurnAngle1,int targBatteryVol1,
	SectName nextSectName2,BOOL jBT2,int targDist2,BOOL jLineEdge2,U16 targGradient2,S32 targTime2,float targXCoo2,float targYCoo2,U16 makerhreshold2,int targObstDist2,int targTurnAngle2,int targBatteryVol2,
	SectName nextSectName3,BOOL jBT3,int targDist3,BOOL jLineEdge3,U32 targGradient3,S32 targTime3,float targXCoo3,float targYCoo3,U32 makerhreshold3,int targObstDist3,int targTurnAngle3,int targBatteryVol3)
*/

 CngSectTerm_init(mCngSecTerm,FST_STRAIGHT,true,0,false,0,0,0,0,0,0,0,0,IDLE,false,0,false,0,0,0,0,0,0,0,0,IDLE,false,0,false,0,0,0,0,0,0,0,0);

 Sector_init(&idleSector,IDLE,mTargetDrivenParm,mCngSecTerm);
 idleSector.nextSect[0]=(Sector*)&fstStraightSector; //ãLñ@Ç†Ç¡ÇƒÇ‹Ç∑Ç©ÅH
 idleSector.nextSect[1]=NULL;
 idleSector.nextSect[2]=NULL;
// idleSector.nextSector[0]=&fstStraightSector;

 /*___________________MAKE FST_STRAIGHT____________________*/

 TargetDrivenParm_init(&mTargetDrivenParm,30,0,TAIL_RUNNING,95,0,HYBRID_PID,0,0,0,0);

 
 CngSectTerm_init(mCngSecTerm,UP_SLOPE,false,30,false,30,2000,0,0,0,0,0,0,IDLE,false,0,false,0,0,0,0,0,0,0,0,IDLE,false,0,false,0,0,0,0,0,0,0,0);
 Sector_init(&fstStraightSector,FST_STRAIGHT,mTargetDrivenParm,mCngSecTerm);
 /*____________________________________________________*/


}

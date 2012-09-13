
#include "RunnerRobot.h"
#include "../Common/Factory.h"

void RunnerRobot_init(RunnerRobot *this_RunnerRobot){
	this_RunnerRobot->robotState = CALIBRATION;
}

	/**
	 * 走行する
	 */
void RunneRobot_run(RunnerRobot *this_RunnerRobot){
	switch (this_RunnerRobot->robotState) {
		case CALIBRATION :
			RunnerRobot_calibrate(&mRunnerRobot);
		break;

		case RUNNING :
			Running_runCurrentSect(&mRunning);
			break;
		default :
			break;
	}
}
	/**
	 * 走行を終了する
	 */
void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot){

	//none
}

	/**
	 * キャリブレーションする
	 */
void RunnerRobot_carlibrate(RunnerRobot *this_RunnerRobot){
	S8 blackVal=0,whiteVal=0;
	U16 gyroOffset=0;
	//黒値取得
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,880, 512, 10);
			blackVal=LightSensor_getLightSensorVal(&mLightSensor);
			mSector.targetDrvenParm.blackVal=blackVal;

			systick_wait_ms(500);

			break;
		}
	}

	//白値取得
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,880, 512, 10);
			whiteVal=LightSensor_getLightSensorVal(&mLightSensor);
			mSector.targetDrvenParm.whiteVal=whiteVal;
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	//this_Calibration -> grayValue=(this_Calibration -> blackValue + this_Calibration -> whiteValue)/2;

	//ジャイロオフセット取得
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,932, 512, 10);
			gyroOffset=GyroSensor_getGyroAngVelocity(&mGyroSensor);
			mSector.targetDrvenParm.gyroOffset = gyroOffset;
			systick_wait_ms(500);
			break;
		}
	}
	while(1){ //タッチセンサスタート
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,932, 512, 10);
			RunnerRobot_updateRobotState(this_RunnerRobot,RUNNING);
			systick_wait_ms(500);
			break;
		}
	}


}

	/**
	 * 走行体状態更新する
	 */
void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot,ROBOT_STATE robotState){
	this_RunnerRobot->robotState = robotState;
}

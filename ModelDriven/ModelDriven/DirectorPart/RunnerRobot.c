
#include "RunnerRobot.h"
#include "../Common/Factory.h"
//for test driven part
#include "../TestDrivenPart.h"
//

void RunnerRobot_init(RunnerRobot *this_RunnerRobot){
	this_RunnerRobot->robotState = CALIBRATION;
}

	/**
	 * 走行する
	 */
void RunneRobot_run(RunnerRobot *this_RunnerRobot){
	switch (this_RunnerRobot->robotState) {
		case CALIBRATION :
			 RunnerRobot_calibrate(this_RunnerRobot);
		break;

		case DRIVING :
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
void  RunnerRobot_calibrate(RunnerRobot *this_RunnerRobot){
	S8 blackVal=0,whiteVal=0;
	U16 gyroOffset=0;
	//黒値取得
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,880, 512, 10);
			blackVal=LightSensor_getLightSensorVal(&mLightSensor);
			mSector.targetDrvenParm.blackVal=blackVal;

			systick_wait_ms(300);

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

	//ラインエッジ検出器にラインエッジの輝度値をセット
	//mLineEdge.edgeLightVal=( blackVal +  whiteVal)/2;
	mLineEdge.blackVal=blackVal;
	mLineEdge.whiteVal=whiteVal;

	//ジャイロオフセットと静止時の電圧値を取得
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,932, 512, 10);
			gyroOffset=GyroSensor_getGyroAngVelocity(&mGyroSensor);
			mSector.targetDrvenParm.gyroOffset = gyroOffset;
			//傾き検知器に基準となるオフセット値をセット
			mGradient.baseGyroVal=gyroOffset;
			//基準バッテリ電圧値をセット
			mBattery.initVol=ecrobot_get_battery_voltage();

			systick_wait_ms(500);
			break;
		}
	}
	while(1){ //タッチセンサスタート
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,932, 512, 10);
			setTargVals(mSector.targetDrvenParm); //test code
			RunnerRobot_updateRobotState(this_RunnerRobot,DRIVING);

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


#include "RunnerRobot.h"
#include "../Common/Factory.h"
//for test driven part
#include "../TestDrivenPart.h"
//

void RunnerRobot_init(RunnerRobot *this_RunnerRobot){
	this_RunnerRobot->robotState = CALIBRATION;
}

	/**
	 * ���s����
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
	 * ���s���I������
	 */
void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot){

	//none
}

	/**
	 * �L�����u���[�V��������
	 */
void  RunnerRobot_calibrate(RunnerRobot *this_RunnerRobot){
	S8 blackVal=0,whiteVal=0;
	U16 gyroOffset=0;
	//���l�擾
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

	//���l�擾
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

	//���C���G�b�W���o��Ƀ��C���G�b�W�̋P�x�l���Z�b�g
	//mLineEdge.edgeLightVal=( blackVal +  whiteVal)/2;
	mLineEdge.blackVal=blackVal;
	mLineEdge.whiteVal=whiteVal;

	//�W���C���I�t�Z�b�g�ƐÎ~���̓d���l���擾
	while(1){
		if(TouchSensor_getTouchSensor(&mTouchSensor) == TRUE)
		{
			Sound_soundTone(&mSound,932, 512, 10);
			gyroOffset=GyroSensor_getGyroAngVelocity(&mGyroSensor);
			mSector.targetDrvenParm.gyroOffset = gyroOffset;
			//�X�����m��Ɋ�ƂȂ�I�t�Z�b�g�l���Z�b�g
			mGradient.baseGyroVal=gyroOffset;
			//��o�b�e���d���l���Z�b�g
			mBattery.initVol=ecrobot_get_battery_voltage();

			systick_wait_ms(500);
			break;
		}
	}
	while(1){ //�^�b�`�Z���T�X�^�[�g
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
	 * ���s�̏�ԍX�V����
	 */
void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot,ROBOT_STATE robotState){
	this_RunnerRobot->robotState = robotState;
}

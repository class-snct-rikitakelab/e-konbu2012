#include "LookUpGate.h"
#include "runnerForTestRun.h"

int runningLookUpGate(){

	static int timecounter = 0;
	static int distance_before_gate = 0;
	static int distance_after_gate = 0;

	static int lookupgateendflag = 0;

	switch(runningmode){
		case (LOOKUPGATE_START):
			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			timecounter++;

			if(timecounter >= 150)					//�X�^�[�g���ɔ�������̂�h���i�e�X�g�p�j
			{
				if(sonarCheck(19) == 1)				//�����g�Z���T�������������ǂ���
				{
					ecrobot_sound_tone(900,512,30);
					runningmode = LOOKUPGATE_STOP;
					timecounter = 0;
				}
			}
			break;

			//���b�N�A�b�v�Q�[�g���s�������
		case (LOOKUPGATE_STOP):
			setCmdForward(RA_speed(5));
			setCmdTurn(RA_linetrace_PID(getCmdForward())+ RA_curvatureCtrl_PID(0));

			timecounter++;

			if(timecounter == 200)
			{	
				runningmode = LOOKUPGATE_DOWN;
				timecounter = 0;
			}

			break;
		
			//���b�N�A�b�v�Q�[�g���s�A�K���~��
		case (LOOKUPGATE_DOWN):
			setCmdForward(RA_speed(0));
			setCmdTurn(0);

			timecounter++;

			if(timecounter >= 200)
			{
				TailAngleChange(ANGLEOFLOOKUP);
				if(ecrobot_get_motor_rev(NXT_PORT_A) < ANGLEOFLOOKUP+5)
					{
						runningmode = LOOKUPGATE_MOVE;
						timecounter = 0;
						distance_before_gate = getNowDistance();
						changeGray();
					}
			}

			break;

			//���b�N�A�b�v�Q�[�g���s�A�K���~����ԂőO�i
		case (LOOKUPGATE_MOVE):

			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward())+ RA_curvatureCtrl_PID(0));


			distance_after_gate = getNowDistance();
			
			if(distance_after_gate - distance_before_gate > 30)
			{	
				runningmode = LOOKUPGATE_UP;
			}
			
			break;

			//���b�N�A�b�v�Q�[�g���s�A�O�i��|����Ԃ֕��A
		case (LOOKUPGATE_UP):
			if(timecounter < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			timecounter++;

			//�K�������グ�⏕
			if(timecounter == 200)
			{
				TailAngleChange(ANGLEOFDOWN);
				
				
			}
	
			if(ecrobot_get_motor_rev(NXT_PORT_A) < ANGLEOFDOWN+5 && timecounter >= 1050)
			{

				
				changeGray();
				lookupgateendflag = 1;
			}
			
			break;
		default:
			break;
	}
	return lookupgateendflag;
}
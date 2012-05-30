/*
 * sample10.c
 *
 * �o�����X���ė����A���C���g���[�X����
 * (c) COPYRIGHT 2010 Afrel., Inc.
 * All rights reserved
 */

#include "sample10.h"

/*
 * 臒l
*/
static unsigned int LIGHT_THRESHOLD=580;
static unsigned int BLACK_VALUE=600;
static unsigned int WHITE_VALUE=500;
static unsigned int GRAY_VALUE=550;

static float hensa;
	static float i_hensa = 0;
	static float d_hensa = 0;
	static float bf_hensa = 0;
	static float Kp = 1.75;
	static float Ki = 2.6;
	static float Kd = 0.012;

/*
 * �V�X�e���S�̂̏��
 */
typedef enum{
	RN_MODE_INIT, 		/* system initialize mode */
	RN_MODE_CONTROL,		/* control mode */
	RN_RUN
} RN_MODE;


/*
 * �L�����u���[�V�����̏��
 */
typedef enum{
	RN_SETTINGMODE_GYRO_START,
	RN_SETTINGMODE_END
} RN_SETTINGMODE;


/*
 *�������
 */

RN_MODE runner_mode = RN_MODE_INIT;					/* NXTway-GS mode */
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_GYRO_START;/* �L�����u���[�V�����̏�� */


/*
 * �W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
 */
static U32	gyro_offset = 0;    /* gyro sensor offset value */
static U32	avg_cnt = 0;		/* average count to calc gyro offset */
static U32	cal_start_time;		/* calibration start time */
static U32	battery_value = 0;


/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;
	
	
/*
 * �L�����u���[�V�����p�̃v���C�x�[�g�֐�
 */
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_p(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void shock();



/*
 * ���{�b�g����p�̃v���C�x�[�g�֐�
 */
int RN_move();

/*
 *�J�E���^�̐錾
 */
DeclareCounter(SysTimerCnt);


/*
 *�^�X�N�̐錾
 */
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


/*
 *�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
 */
const char target_subsystem_name[] = "Logtrace";


/*
 *��������
 */
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");
}


/*
 *��n������
 */
void ecrobot_device_terminate(void)
{
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
}


/*--------------------------------------------------------------------------*/
/* OSEK hooks                                                               */
/*--------------------------------------------------------------------------*/
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}


/*--------------------------------------------------------------------------*/
/* Function to be invoked from a category 2 interrupt                       */
/*--------------------------------------------------------------------------*/
void user_1ms_isr_type2(void){
	StatusType ercd;
	/*
	 *  Increment OSEK Alarm System Timer Count
     */
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}


/*
 *�^�X�N1�p�̊֐�
 */
TASK(ActionTask)
{

	switch (runner_mode){
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			battery_value = ecrobot_get_battery_voltage();
			break;

		case (RN_MODE_CONTROL):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S4),
				(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
				(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
	TerminateTask();
}


/*
 *�\���p�̊֐�
 */
TASK(DisplayTask)
{
	/* ���j�^�ɃV�X�e������\������ */
	ecrobot_status_monitor(target_subsystem_name);

	/* ���^�X�N�̏I�� */
	/* ��̓I�ɂ́C���^�X�N�����s��Ԃ���x�~��ԂɈڍs�����C*/
	/* �^�X�N�̏I�����ɍs���ׂ��������s�� */
	TerminateTask();
}



/*
 *�^�X�N2�p�̊֐�
 */
TASK(ActionTask2)
{
	RN_setting();

	/* ���^�X�N�̏I�� */
	/* ��̓I�ɂ́C���^�X�N�����s��Ԃ���x�~��ԂɈڍs�����C*/
	/* �^�X�N�̏I�����ɍs���ׂ��������s�� */
	TerminateTask();
}

void RA_linetrace(int forward_speed, int turn_speed) {

	if(ecrobot_get_battery_voltage() <= battery_value-400)
		{cmd_forward = forward_speed*3;
		ecrobot_sound_tone(880, 512, 30);}
	else if(ecrobot_get_battery_voltage() >= battery_value+400)
		{cmd_forward = forward_speed/3;
		ecrobot_sound_tone(900,512,30);}
//	else if(ecrobot_get_battery_voltage() <= battery_value-400)
//		ecrobot_sound_tone(890,512,30);
//	else if(ecrobot_get_battery_voltage() <= battery_value-300)
//		ecrobot_sound_tone(900,512,30);
//	else if(ecrobot_get_battery_voltage() <= battery_value-200)
//		ecrobot_sound_tone(910,512,30);
//	else
	else	cmd_forward = forward_speed;

	/* �����̔��f�����ĉ�]���������߂� */
	int light_value = 0;
	light_value = online();
	if (TRUE != light_value) {
		cmd_turn = (-1)*turn_speed;
	} else {
		cmd_turn = turn_speed;
	}

//	battery_value = ecrobot_get_battery_voltage();

}

void RA_linetrace_P(int forward_speed) {
	/* �O�i���� */

	shock();

	if (FALSE == TU_is_timeout() ) {

		/** �^�C�}�Ď����I�������� */
	//	cmd_forward = 0;
	}

//	else if(ecrobot_get_battery_voltage() >= battery_value+400)
//		{//cmd_forward = forward_speed/3;
//		ecrobot_sound_tone(900,512,30);}

	else if(TRUE == TU_is_timeout()){
		if (!TU_is_start()) {
		/** 90�b�̃^�C�}�Ď��J�n */
		TU_start(2000);}}
	cmd_forward = forward_speed;

	if(forward_speed<0)
		forward_speed*=(-1);

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	i_hensa = i_hensa+(hensa*0.0004);
	d_hensa = (hensa - bf_hensa)/0.0004;
	bf_hensa = hensa;
	/* �����Ɓ{�l */
	/* �����Ɓ|�l */
	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
	battery_value = ecrobot_get_battery_voltage();

}

void shock(void){
//	if(ecrobot_get_battery_voltage() <= battery_value-100){
//		ecrobot_sound_tone(880, 512, 30);
//	}
	if((ecrobot_get_gyro_sensor(NXT_PORT_S4) <= gyro_offset-50 || 
 ecrobot_get_gyro_sensor(NXT_PORT_S4) >= gyro_offset+50) &&
 ecrobot_get_battery_voltage() <= battery_value-100){
		ecrobot_sound_tone(880,512,30);}
	
//	if(ecrobot_get_gyro_sensor(NXT_PORT_S4) <= gyro_offset-100 || 
// ecrobot_get_gyro_sensor(NXT_PORT_S4) >= gyro_offset+100)
//		ecrobot_sound_tone(880,512,30);
}

int online(void) {
	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);
	if (GRAY_VALUE > light_value) {
		if ((GRAY_VALUE - 20) > light_value) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
	return TRUE;
}

TASK(LogTask)
{
	static S8 i,j;
	int sonar;

	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);

	/* send Sensor/Motors/NXT internal status to the host.
	 * NXT GamePad in the host PC accumulates all logging data
	 * and later you can save the logging data into a CSV file
	 */

	

	logSend(cmd_forward,cmd_turn,ecrobot_get_battery_voltage(),ecrobot_get_gyro_sensor(NXT_PORT_S4),
ecrobot_get_touch_sensor(NXT_PORT_S1),0);

	/* display Sensors/Motors/NXT internal status */ 
	ecrobot_status_monitor("Data Logging");

	TerminateTask();
}

void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4)
        {
            U8 data_log_buffer[32];

            *((U32 *)(&data_log_buffer[0]))  = (U32)systick_get_ms();
            *(( S8 *)(&data_log_buffer[4]))  =  (S8)data1;
            *(( S8 *)(&data_log_buffer[5]))  =  (S8)data2;
            *((U16 *)(&data_log_buffer[6]))  = (U16)ecrobot_get_light_sensor(NXT_PORT_S3);
            *((S32 *)(&data_log_buffer[8]))  = (S32)nxt_motor_get_count(0);
            *((S32 *)(&data_log_buffer[12])) = (S32)nxt_motor_get_count(1);
            *((S32 *)(&data_log_buffer[16])) = (S32)nxt_motor_get_count(2);
            *((S16 *)(&data_log_buffer[20])) = (S16)adc1;
            *((S16 *)(&data_log_buffer[22])) = (S16)adc2;
            *((S16 *)(&data_log_buffer[24])) = (S16)adc3;
            *((S16 *)(&data_log_buffer[26])) = (S16)adc4;
            *((S32 *)(&data_log_buffer[28])) = (S32)ecrobot_get_sonar_sensor(NXT_PORT_S2);
            
            ecrobot_send_bt_packet(data_log_buffer, 32);
        }

/*
 *�v���C�x�[�g�֐��̎���
 */
void RN_setting()
{
	switch (setting_mode){
		case (RN_SETTINGMODE_GYRO_START):
			RN_calibrate();
			break;

		case (RN_SETTINGMODE_END):
			runner_mode = RN_MODE_CONTROL;
			setting_mode=RN_RUN;

		case (RN_RUN):
			RA_linetrace_P(30);
			break;
		default:
			break;
	}
}

void RN_calibrate()
{
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;
	while(1){
	if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
		gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S4);
		break;
		}
	}
	while(1){
	/* �o���p�𗣂��Ǝ��̏�ԂɑJ�ڂ���(�ݒ胂�[�h���I������j*/
	if (ecrobot_get_touch_sensor(NXT_PORT_S1) != TRUE) {
		ecrobot_sound_tone(900, 512, 30);
		setting_mode = RN_SETTINGMODE_END;
		break;
		}
	}
}

/******************************** END OF FILE ********************************/

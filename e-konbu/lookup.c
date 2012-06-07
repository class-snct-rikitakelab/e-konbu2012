/*
 * logtrace.c
 * ���C���g���[�X�y�ђi�����m�v���O����
 */


#include "logtrace.h"


//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

static int counter = 0;

//�K���ݒ�p�x
#define ANGLEOFDOWN 100				//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x
#define ANGLEOFPUSH 210				//����ڕW�p�x�i���g�p�j

//���x���ߌW��
#define SPEED_COUNT 20

//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

#define CMD_START '1'    			//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��
static float Kp = 1.85;				//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;				//D����p

static float t_Kp = 1.85;
static float t_Ki = 2.6;

static int t_angle = 0;

static int t_count = 0;

static int wait_count = 0;

static int sonarflag;

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;


//�����v���p�ϐ�
int revL = 0;
int revR = 0;

//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_CONTROL,				//�|������ON
	RN_MODE_BALANCEOFF
} RN_MODE;


//�L�����u���[�V�����̏��
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_LOOKUP,
	RN_LOOKUPDOWN,						//��~
	RN_LOOKUPMOVE,
	RN_LOOKUPUP
} RN_SETTINGMODE;

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
	RN_TAILPUSH,				//�K�����o
	RN_TAILLOOKUP_0,
	RN_TAILLOOKUP_1,
	RN_TAILLOOKUP_2,
	RN_TAILLOOKUP_3
} RN_TAILMODE;

typedef enum{
	RN_SONAR_NEAR,
	RN_SONAR_FAR
} RN_SONAR_MODE;

//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILDOWN;
RN_SONAR_MODE sonar_mode = RN_SONAR_FAR;


//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void taildown();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
void sonarcheck(void);
void runner_mode_change(int flag);
int getsonarflag(void);
void tailpower(float value);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);


//�^�X�N�̐錾
/*
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);
*/

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "lookup";


//��������
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");
	ecrobot_init_sonar_sensor(NXT_PORT_S2);
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}


/*
 *��n������
 */
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
}


//OSEK�t�b�N�֐�
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}


//�t�b�N�֐�
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


//ON-OFF���䃉�C���g���[�X
void RA_linetrace(int forward_speed, int turn_speed) {

	cmd_forward = forward_speed;

	int light_value = 0;
	light_value = online();
	if (TRUE != light_value) {
		cmd_turn = turn_speed;
	} else {
		cmd_turn = (-1)*turn_speed;
	}

}


//P����݂̂̃��C���g���[�X
void RA_linetrace_P(int forward_speed){

	cmd_forward = forward_speed;

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

	cmd_turn = -(1.4 * hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
}


//PID���䃉�C���g���[�X
void RA_linetrace_PID(int forward_speed) {

	RA_speed(forward_speed,2);	//���x��i�K�I�ɕω�

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0005);
	d_hensa = (hensa - bf_hensa)/0.0005;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}


	/*�|������OFF��*/
	//nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	//nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//�i�K�I����
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	counter += 1;

	if(counter == SPEED_COUNT)
	{

		forward_speed = cmd_forward;

		if(limit-forward_speed >= 0){
			forward_speed += s_Kp;

			if(forward_speed > limit)
				forward_speed = limit;
		}
		else{
			forward_speed -= s_Kp;

			if(forward_speed < limit)
				forward_speed = limit;
		}

		cmd_forward = forward_speed;
		counter =0;
	}
}


//2�ԗւ̉�]�ʂ̍���P����Œ��߂���i�ڕW�l�ɋ߂Â���j
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}

//ON-OFF����p���C������
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);

	if (GRAY_VALUE > light_value) {
		if ((GRAY_VALUE) > light_value) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	return TRUE;
}


void sonarcheck(void)
{
	if(ecrobot_get_sonar_sensor(NXT_PORT_S2) <= 21)
	{
		sonarflag = 1;
	}
	else
		sonarflag = 0;
}

int getsonarflag(void)
{
	return sonarflag;
}

void runner_mode_change(int flag)
{
	switch(flag){
	case 0:
		runner_mode = RN_MODE_INIT;
		break;
	case 1:
		runner_mode = RN_MODE_CONTROL;
		break;
	case 2:
		runner_mode = RN_MODE_BALANCEOFF;
		break;
	default:
		break;
	}
}

void tailpower(float value)
{
	t_Kp = value;
}


//�K���p�x�ێ�
void taildown(){

	static float t_hensa = 0;
	static float t_ihensa = 0;

	static float t_speed = 0;

	t_count++;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILUP):
			t_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILPUSH):
			t_hensa = ANGLEOFPUSH - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILLOOKUP_0):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILLOOKUP_1):
			if(t_angle <= 45)
			{
				t_hensa = ANGLEOFDOWN - t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= 10)
				{
					t_angle += 1;
					t_count = 0;
				}
			}
			else
				t_angle = 45;

			break;

		case(RN_TAILLOOKUP_2):
			if(t_angle >= 0)
			{
				t_hensa = ANGLEOFDOWN - t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= 10)
				{
					t_angle -= 1;
					t_count = 0;
				}
			}
			else
				t_angle = 0;

			break;
		case(RN_TAILLOOKUP_3):
			t_hensa = ANGLEOFDOWN -45 - ecrobot_get_motor_rev(NXT_PORT_A);
		default:
			break;
	}

	t_ihensa = t_ihensa+(t_hensa*0.0004);

	t_speed = (t_Kp*t_hensa + t_Ki*t_ihensa);

		if (t_speed < -100)
			t_speed = -100;
		else if (t_speed > 100)
			t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);

}

static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* ��M�o�b�t�@���N���A */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* ��M�f�[�^���� */
		if (rx_buf[0] == CMD_START)
		{
			start = 1; /* ���s�J�n */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	return start;
}

//bluetooth�ɂ�郍�O���M
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4){
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


//���s��Ԑݒ�(���C��)
void RN_setting()
{
	static float beforestop = 0;

	switch (setting_mode){
			//�L�����u���[�V����f
		case (RN_SETTINGMODE_START):
			RN_calibrate();
			break;

			//�ʏ푖�s
		case (RN_RUN):
			RA_linetrace_PID(30);

			wait_count++;

			if(wait_count >= 200)
			{

				if(getsonarflag() == 1)
				{
					ecrobot_sound_tone(900,512,30);
					setting_mode = RN_LOOKUP;
					wait_count = 0;
				}
			}

			break;

		case (RN_LOOKUP):
			RA_speed(0,3);
			cmd_turn = RA_wheels(cmd_turn);

			wait_count++;

			if(cmd_forward <= 0)
			{
				gyro_offset -= 140;
				tail_mode = RN_TAILLOOKUP_0;
				systick_wait_ms(50);
				setting_mode = RN_LOOKUPDOWN;
				runner_mode_change(2);
				wait_count = 0;
				cmd_forward = 0;
			}

			break;

		case (RN_LOOKUPDOWN):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			wait_count++;

			if(wait_count == 1200)
			{
				tail_mode = RN_TAILLOOKUP_1;
			}

			else if(t_angle == 45)
			{
				setting_mode = RN_LOOKUPMOVE;
				wait_count = 0;
			}

			break;

		case (RN_LOOKUPMOVE):
			nxt_motor_set_speed(NXT_PORT_C, cmd_forward, 1);
			nxt_motor_set_speed(NXT_PORT_B, cmd_forward, 1);
			
			RA_speed(30,1);

			wait_count++;

			if(wait_count == 800)
			{	
				//tailpower(10.0);
				setting_mode = RN_LOOKUPUP;
				wait_count = 0;
			}

			break;

		case (RN_LOOKUPUP):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			wait_count++;

			//if(wait_count == 200)
				tail_mode = RN_TAILLOOKUP_2;

			if(t_angle == 0)
			{
				tail_mode = RN_TAILDOWN;
				setting_mode = RN_RUN;
			}

			break;

		default:
			break;
	}
}

//�L�����u���[�V����
void RN_calibrate()
{

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//�D�F�l�v�Z
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//�W���C���I�t�Z�b�g�y�уo�b�e���d���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			systick_wait_ms(500);
			break;
		}
	}

	//���s�J�n���}
	while(1){

		//�����[�g�X�^�[�g
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_CONTROL;
			break;
		}

		//�^�b�`�Z���T�X�^�[�g
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_CONTROL;
						tail_mode = RN_TAILUP;
						break;
					}
				}
			break;
		}
	}

	//�L�����u���[�V�����I��

}

//���s���@�ݒ�
void RN_modesetting()
{
	switch (runner_mode){
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//�o�����T�[
		case (RN_MODE_CONTROL):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
		 		(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

			//�o�����T�[����
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}


//���s���@�Ǘ�(4ms)
TASK(ActionTask)
{
	RN_modesetting();
	taildown();
	TerminateTask();
}

//���s��ԊǗ�(5ms)
TASK(ActionTask2)
{
	RN_setting();
	TerminateTask();
}

//��ԕ\���Ǘ�(20ms)
TASK(DisplayTask)
{
	ecrobot_status_monitor(target_subsystem_name);
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,ecrobot_get_battery_voltage(),t_count,
			t_angle,ecrobot_get_gyro_sensor(NXT_PORT_S1));

	sonarcheck();

	TerminateTask();
}


/******************************** END OF FILE ********************************/

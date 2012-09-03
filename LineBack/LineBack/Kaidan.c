

#include "Kaidan.h"
#include "logSend.h"
#include "math.h"
#include "tyreal_light_ver.h"
#include "LineBack.h"
#include "RobotPosutre.h"
#include "Factory.h"
#include "Calibration.h"
#include "PIDControl.h"


/*
 *	�e��ϐ���`
 */
/*
//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
*/

//�W���C���I�t�Z�b�g�l
//static unsigned int GYRO_OFFSET;	

static int counter = 0;


//�K���ݒ�p�x
#define ANGLEOFDOWN 95		//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x

//���x���ߌW��
#define SPEED_COUNT 20

//�o�b�e���~���l
#define STEP_BATTERY 300
#define STOP_BATTERY 400

//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

#define CMD_START '1'    			//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)
#define rapid_SPEED_UP_SIGNAL '3'

#define POSITION_X0 0
#define POSITION_Y0 0
#define THETA_0 0

#define WHEEL_R		41
#define MACHINE_W	162


static double min_vol;
static int stepflag = 0;

//�o�b�e���d���l���
static U32	battery_value;

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;


//�����v���p�ϐ�
int revL = 0;
int revR = 0;
float distance_before_step = 0;
float distance_step_brake = 0;
float distance_step_stop = 0;
float distance_gyro_up = 0;
float distance_over_forty = 0;


/* ���Ȉʒu����p�@�ϐ��錾 */
float d_theta_r;					/* ���݂̉E���[�^��]�p�x [rad] */
float d_theta_l;					/* ���݂̍����[�^��]�p�x [rad] */
static float d_theta_r_t;			/* 1 �X�e�b�v�O�̉E���[�^��]�p�x [rad] */
static float d_theta_l_t;			/* 1 �X�e�b�v�O�̍����[�^��]�p�x [rad] */
float velocity_r;					/* �E�ԗֈړ����x [cm/s] */
float velocity_l;					/* ���ԗֈړ����x [cm/s] */
float velocity;						/* ���{�b�g�̈ړ����x [cm/s] */
float omega;						/* ���{�b�g�̉�]�p�p�x [rad/s] */
static float position_x = POSITION_X0; /* ���{�b�g�� x ���W */
static float position_y = POSITION_Y0; /* ���{�b�g�� y ���W */
static float theta = THETA_0;		/* ���{�b�g�̎p���p */
unsigned short int l_val;			/* ���Z���T�l */
int temp_x;							/* ���{�b�g�� x ���W�i�o�͏����p�j */
int temp_y;							/* ���{�b�g�� y ���W�i�o�͏����p�j */
static double omega_r;			//�E�ԗւ̉�]�p���x
static double omega_l;			//���ԗւ̉�]�p���x
unsigned char tx_buf[BT_MAX_TX_BUF_SIZE]; /* ���M�o�b�t�@ */


//�o�b�e�����όv�Z
int battery_average;
static int now_battery = 0;
static int bt_counter = 0;
static int before_battery =0;
static int average_flag;


/*
 *	��Ԓ�`
 */


typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_STOP,					//��~
	RN_STEP_RAPID,
	RN_STEP_SHOCK,
	RN_STEP_SLOW,
	RN_STEP_STAY,
	RN_STEP_SECOND,
	TYREAL,
	RN_LINEBACK
} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;



RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = TYREAL;
RN_TAILMODE tail_mode = RN_TAILDOWN;


//�i�����m�֘A�}�N���A�v���g�^�C�v

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�

//void RA_linetrace_PID(int forward_speed);
//int RA_speed(int forward_speed);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "Kaidan";


/*
 *	�e��֐�
 */

//��������
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");

	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	factory();

	/*
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
	*/
}


//��n������
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



//�i�K�I�����p�֐��i�w��ʂ������x�����X�ɏ㏸�j
int RA_speed(int forward_speed){

	static int result_speed = 0;
	counter++;

	if(counter >= SPEED_COUNT)
	{
		if(forward_speed - result_speed >= 0){
			result_speed++;

			if(result_speed > forward_speed)
				result_speed = forward_speed;
		}
		else{
			result_speed--;

			if(result_speed < forward_speed)
				result_speed = forward_speed;
		}
		counter = 0;
	}

	return result_speed;
}


//�ԗ։�]�ʍ����ߊ֐��iPID����j
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}

//�Ռ����m�֐�
int shock(int target){

	int result=0;

	//�d���~���̍ŏ��l���X�V
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//�W���C���y�ѓd���~������Ռ����m
	if(target <= battery_value-min_vol)
	{
		result = 1;
	}
	else
		result = 0;

	return result;
}

/*
//ON-OFF����p���C������֐�
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
*/

//�K���p�x�R���g���[���֐�
void tailcontrol(){

	static const float t_Kp = 1.7;

	static float t_hensa = 0;
	static float t_speed = 0;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);		//�K����������
			break;

		case(RN_TAILUP):
			t_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);		//�K�����グ��
			break;

		default:
			break;
	}

	t_speed = t_Kp*t_hensa;

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);

}

//�d���l�`�F�b�N�֐��i�g��Ȃ��j
void battery_average_check(void)
{
	if(bt_counter == 40)
	{
		battery_average = now_battery/40;

		if(battery_average > before_battery)
			average_flag = 1;
		else
			average_flag = 0;

		before_battery = battery_average;
		
		now_battery = 0;
		bt_counter = 0;
	}
	else
	{
		now_battery += ecrobot_get_gyro_sensor(NXT_PORT_S1);
		bt_counter++;
	}
}

//���s�ݒ�֐�
void RN_setting()
{
	static int step_count = 0;
	static int counter=0;
	counter++;

	//�O�i�ʂƐ����
	ControlVals controlVals;
	controlVals.forward_val=0;
	controlVals.turn_val=0;

	switch (setting_mode){
		case (TYREAL) :
			//do_tyreal(&Kp,&Ki,&Kd);
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;

			//���s�J�n�O
		case (RN_SETTINGMODE_START):
		if(Calibration_doCalibrate(&mCalibration)==1){
			//RobotPosture_setPostureMode(&mRobotPosture,BALANCING);
			RobotPosture_setPostureMode(&mRobotPosture,TAIL_RUNING);
			tail_mode = RN_TAILDOWN;
			setting_mode = RN_RUN;
			

		}
			//		RN_calibrate();		//�L�����u���[�V����
			
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

	
			//�ʏ푖�s
		case (RN_RUN):
			controlVals.forward_val=RA_speed(20);
//			controlVals.turn_val=RA_linetrace_PID(controlVals.forward_val);

			controlVals.turn_val=PIDControl_PIDLineTrace(&mPIDControl,controlVals.forward_val);

			RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
			//test code
			setting_mode= RN_LINEBACK;
			//test code end
		//if(GYRO_OFFSET - 100 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || GYRO_OFFSET + 100 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
				if(LineBack_debugLineBackSignalReceive(&mLineBack) == 1)
			{
				ecrobot_sound_tone(932, 512, VOL);

				setting_mode= RN_LINEBACK;
			}
		//	cmd_turn = RA_wheels(cmd_turn);
			
			/*
			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				setting_mode = RN_STEP_RAPID;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);

				distance_before_step = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//�i���˓����̋����𑪒�
			}
			*/
			
			break;
		
		case (RN_LINEBACK):
			if(LineBack_doLineBack(&mLineBack)==1){ //���C�����A�ɐ���������P���Ԃ��Ă���
			setting_mode = RN_RUN;
			}
	
			/*
			//����
		case (RN_STEP_RAPID):
			RA_linetrace_PID(25);
			//if(rapid_speed_up(17) == 1)
			gyro_offset += 17;
			wait_count = 0;
			setting_mode = RN_STEP_SHOCK;
			break;

			//�i�����m
		case (RN_STEP_SHOCK):
			RA_linetrace_PID(25);
			wait_count++;

			if(wait_count > 100)
			{
				if(shock(STEP_BATTERY) == 1)
				{
					min_vol = battery_value;
					setting_mode = RN_STEP_SLOW;
				}
			}
			
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);

			distance_gyro_up = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//�i���˓����̋����𑪒�

			break;

			//����
		case (RN_STEP_SLOW):
			RA_linetrace_PID(25);
			//if(rapid_speed_up(-34) == 1)
			gyro_offset -= 34;
			ecrobot_sound_tone(880, 512, 30);
			setting_mode = RN_STEP_STAY;
			wait_count = 0;
			break;

			//���܂�
		case (RN_STEP_STAY):
			RA_linetrace_PID(25);
			wait_count++;

			if(wait_count == 85)
				gyro_offset += 16;
			
			if(wait_count >= 300)
			{
				setting_mode = RN_STEP_SECOND;

				wait_count = 0;
			}
			
			break;

			//��i��
		case (RN_STEP_SECOND):
			if(step_count == 0)
			//RA_linetrace_PID(25);
			RA_linetrace_PID(0);

			else if(step_count == 1)
			{
				RA_linetrace_PID(0);
				cmd_turn = RA_wheels(cmd_turn);
			}

			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				step_count = 1;
				setting_mode = RN_STEP_RAPID;
			}
			break;


*/
			//������~
		case(RN_STOP):
			cmd_forward=0;
			cmd_turn=0;
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{setting_mode= RN_RUN;

			}
			/*
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			*/
			break;

		default:
			break;
	}
}

//�}�����p�֐�
int rapid_speed_up(int target_gyro){
	/*
	static int rapid_speed_up_counter = 0;

	if(rapid_speed_up_counter >= 0)
	{
		if(GYRO_OFFSET > (target_gyro + GYRO_OFFSET))
			gyro_offset--;
		else
			gyro_offset++;

		rapid_speed_up_counter = 0;
	}

	rapid_speed_up_counter++;

	if((target_gyro + GYRO_OFFSET) == gyro_offset)
		return 1;
	else
		return 0;
		*/
	return 0;
}

 int RN_rapid_speed_up_signal_recevie(void)
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
		if (rx_buf[0] == rapid_SPEED_UP_SIGNAL)
		{
			start = 1;
			stepflag = 1;
		}
		
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		start = 1; 

	return start;
}

//�����[�g�X�^�[�g�֐�
 int remote_start(void)
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





void self_location()
{
	d_theta_l = (float)nxt_motor_get_count(NXT_PORT_C) * M_PI / 180.0;
	d_theta_r = (float)nxt_motor_get_count(NXT_PORT_B) * M_PI / 180.0;

	omega_l = (d_theta_l - d_theta_l_t)/0.004;
	omega_r = (d_theta_r - d_theta_r_t)/0.004;

	velocity_l = (WHEEL_R * 0.1) * omega_l;
	velocity_r = (WHEEL_R * 0.1) * omega_r;

	velocity = (velocity_r + velocity_l) / 2.0;
	omega = (velocity_r - velocity_l) / (MACHINE_W * 0.1);

	d_theta_l_t = d_theta_l;
	d_theta_r_t = d_theta_r;

	theta += omega * 0.004 + THETA_0;
	position_x += velocity * cos(theta) * 0.004 + POSITION_X0;
	position_y += velocity * sin(theta) * 0.004 + POSITION_Y0;
	
}



/*
 *	�e��^�X�N
 */

//���s���@�Ǘ�(4ms)
TASK(ActionTask)
{
	//RN_modesetting();	//���s�̏��
	RN_setting();
	tailcontrol();		//�K���R���g���[��
	self_location();	//���Ȉʒu����
	TerminateTask();
}

//���s��ԊǗ�(5ms)
TASK(ActionTask2)
{
	battery_average_check();
	TerminateTask();
}

//��ԕ\���Ǘ�(20ms)
TASK(DisplayTask)
{
	//ecrobot_status_monitor(target_subsystem_name);	//���j�^�o��
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(0,shock(STEP_BATTERY),theta*(180/M_PI)/*distance_gyro_up - distance_before_step*/,ecrobot_get_gyro_sensor(NXT_PORT_S1),
			position_x,position_y/*GyroVariation_getGyroSensorVariation(&mGyroVariation)*/,position_y);		//���O���
	TerminateTask();
}
/*
 *	PID_parameter.c
 *	PID�p�����[�^���ߗp�v���O����
 */

#include "PID_parameter.h"

/*
 *	�e��ϐ���`
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

static int counter = 0;

#define CMD_STOP '3'

#define ANGLE_OF_AIM 180  //�E�������p�x

//���x���ߌW��
#define SPEED_COUNT 10

/* �ԗ֔��a�A���s�̕�*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

#define CIRCUMFERENCE	25.8	//�ԗւ̉~��

#define POSITION_X0 0
#define POSITION_Y0 0
#define THETA_0 0

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


void self_location();


static unsigned int GYRO_OFFSET_FIRST;

//���C���g���[�X��PID����p�W��

static float Kp = 0.446;//0.436;//0.436;				//P����p
static float Ki = 1.31;//0;//1.31;					//I����p
static float Kd = 0.005;//0.074;//0.011;				//D����p

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

int distance_before_slope;
int distance_now_slope;
int distance_peak_slope;
int distance_after_slope;

/*
 *	��Ԓ�`
 */

//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_BALANCEOFF,				//�|������OFF
} RN_MODE;


typedef enum{
	RN_TYREAL,
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_RIGHT,
	RN_SLOPE_START,
	RN_SLOPE_DOWN,
	RN_SLOPE_AFTER,
	RN_SLOPE_END,
} RN_SETTINGMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = RN_TYREAL;

//RN_TAILMODE tail_mode = RN_TAILDOWN;

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
//void tailcontrol();
int RA_speed(int forward_speed);
void RN_modesetting();
static int remote_start(void);
static int remote_stop(void);
static int right(void);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "PID";


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
}


//��n������
void ecrobot_device_terminate(void)
{
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
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


//PID���䃉�C���g���[�X�֐�
void RA_linetrace_PID(int forward_speed) {

	//PID����p�΍��l
	static float hensa;					//P����p
	static float i_hensa = 0;			//I����p
	static float d_hensa = 0;			//D����p
	static float bf_hensa = 0;
	float right_motor_turn = 0,left_motor_turn = 0;					//�ڕWpwm�l
	float right_motor_turn_overflow = 0,left_motor_turn_overflow = 0;		//�ڕWpwm�l�I�[�o�[�t���[��

	
	forward_speed = RA_speed(forward_speed);	//���x��i�K�I�ɕω�

	//���Z���T�l�ƖڕW�P�x�l�̕΍��Z�o
	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0002);		//0��1����
	d_hensa = (hensa - bf_hensa)/0.0002;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	right_motor_turn = forward_speed - cmd_turn/2;
	left_motor_turn = forward_speed  + cmd_turn/2;


	//�I�[�o�[�t���[�΍�y�уI�[�o�[�t���[���l��
	if (/*-128*/-100 > right_motor_turn) {
		right_motor_turn_overflow = -(-128 - right_motor_turn);
		right_motor_turn = -128;
	} else if (100/*127*/ < right_motor_turn) {
		right_motor_turn_overflow = right_motor_turn - 100/*-127*/;
		right_motor_turn = 127;
	}

	if (/*-128*/-100 > left_motor_turn) {
		left_motor_turn_overflow = -(/*-128*/-100 - left_motor_turn);
		left_motor_turn = -128;
	} else if (/*127*/100 < left_motor_turn) {
		left_motor_turn_overflow = left_motor_turn -100/*- 127*/;
		left_motor_turn = 100/*127*/;
	}

	//�o��pwm�l�Z�o
	if(left_motor_turn + right_motor_turn_overflow >= -100/*-128*/ && left_motor_turn + right_motor_turn_overflow <= 100/*127*/)
		pwm_l = (int)(left_motor_turn + right_motor_turn_overflow);
	else
		pwm_l = (int)left_motor_turn;

	if(right_motor_turn + left_motor_turn_overflow >= -100/*-128*/ && right_motor_turn + left_motor_turn_overflow <= 100/*127*/)
		pwm_r = (int)(right_motor_turn + left_motor_turn_overflow);
	else
		pwm_r = (int)right_motor_turn;

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
/*
//�K���p�x�R���g���[���֐�
void tailcontrol(){

	static const float t_Kp = 1.85;

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
*/

//���s�ݒ�֐�
void RN_setting()
{
	int speed = 100;

	static int time_count = 0;

	switch (setting_mode){

		case (RN_TYREAL):
			do_tyreal(&Kp,&Ki,&Kd);

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				ecrobot_set_motor_speed(NXT_PORT_A,0);
				ecrobot_set_motor_speed(NXT_PORT_B,0);
				ecrobot_set_motor_speed(NXT_PORT_C,0);
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(10);
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;
		
			//���s�J�n�O
		case (RN_SETTINGMODE_START):
			RN_calibrate();								//�L�����u���[�V����
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

			//�ʏ푖�s
		case (RN_RUN):
			time_count++;
			RA_linetrace_PID(speed);
			if (remote_stop()==1)
			{	
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(500);
 		 		//speed = 0;
				runner_mode = RN_MODE_BALANCEOFF;
				setting_mode = RN_TYREAL;	
			}
			
			if(GYRO_OFFSET_FIRST - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && time_count > 1000)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE_START;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);
				distance_before_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
				time_count = 0;
			}
			break;

		case (RN_SLOPE_START):
			RA_linetrace_PID(speed);
			
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_now_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));

			if(distance_now_slope - distance_before_slope > 20)
			{
				setting_mode = RN_SLOPE_DOWN;
			}
			
			break;

		case (RN_SLOPE_DOWN):
			RA_linetrace_PID(speed - 25);
			
			if(GYRO_OFFSET_FIRST + 30 < (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE_AFTER;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);
				distance_peak_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			}
			break;

		case (RN_SLOPE_AFTER):
			RA_linetrace_PID(speed);
			
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_after_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));

			if(distance_after_slope - distance_peak_slope > 30)
			{
				setting_mode = RN_SLOPE_END;
			}

			break;

		case (RN_SLOPE_END):
			RA_linetrace_PID(speed);
			break;

			//�E������
		case(RN_RIGHT):
			right();
			break;

		default:
			break;
	}
}

//�E�������֐�
static int right()
{
	ecrobot_set_motor_rev(NXT_PORT_C, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 50);
	
	while(ecrobot_get_motor_rev(NXT_PORT_C) <= ANGLE_OF_AIM)
	{
	}
	
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
}



//�L�����u���[�V�����֐�
void RN_calibrate()
{
	TailModeChange(RN_TAILDOWN);
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

	TailModeChange(RN_TAILLOOKUP);

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

	TailModeChange(RN_TAILDOWN);
	//�D�F�l�v�Z
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//�W���C���I�t�Z�b�g�y�уo�b�e���d���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			GYRO_OFFSET_FIRST = gyro_offset;
			systick_wait_ms(500);
			break;
		}
	}

	//���s�J�n���}
	while(1){

		//�^�b�`�Z���T�X�^�[�g
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_BALANCEOFF;
						TailModeChange(RN_TAILDOWN);
						break;
					}
				}
			break;
		}
	}

}
//�����[�g�X�g�b�v�֐�
static int remote_stop(void)
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
		if (rx_buf[0] == CMD_STOP)
		{
			start = 1; /* ���s��~ */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	return start;
}

//���s�̏�Ԑݒ�֐�
void RN_modesetting()
{
	switch (runner_mode){

			//�������
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//�o�����T�[ON
		case (RN_MODE_BALANCE):
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

		case (RN_MODE_BALANCEOFF):
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		default:
			break;
	}
}

/*
 *	�e��^�X�N
 */

//���s���@�Ǘ�(4ms)
TASK(ActionTask)
{
	
	RN_modesetting();	//���s�̏��
	TailControl();		//�K���R���g���[��
	//nxt_motor_set_speed(NXT_PORT_C,127, 1);
	self_location();
	TerminateTask();
	 

}

//���s��ԊǗ�(5ms)
TASK(ActionTask2)
{
	RN_setting();		//���s���
	
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(velocity_l/*cmd_forward*/,0/*cmd_turn*/,pwm_l,pwm_r,distance_now_slope - distance_before_slope,velocity/*ecrobot_get_gyro_sensor(NXT_PORT_S1)*/);		//���O���
	TerminateTask();
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



/******************************** END OF FILE ********************************/

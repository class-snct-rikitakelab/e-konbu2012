

#include "Petbottle.h"
#include "logSend.h"
#include "math.h"


/*
 *	�e��ϐ���`
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

//�W���C���I�t�Z�b�g�l
static unsigned int GYRO_OFFSET;	

static int counter = 0;

#define BOTTLE 	27//[cm]

//�K���ݒ�p�x
#define ANGLEOFDOWN 95			//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x

#define CMD_STOP '3'

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

int x=30;   //���x�̕ϐ�

#define ANGLE_OF_AIM 180 //�E�������p�x

//���x���ߌW��
#define SPEED_COUNT 20

//�o�b�e���~���l
#define STEP_BATTERY 300
#define STOP_BATTERY 400

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��

/*
static float Kp = 0.85;				//P����p
static float Ki = 2.2;				//I����p

static float Kp = 1.03;				//P����p
static float Ki = 2.6;				//I����p
*/


static float Kp = 1.360;			//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;				//D����p

/*
static float Kp = 1.0944;			//P����p
static float Ki = 2.2;				//I����p
static float Kd = 0.002;				//D����p
*/

static int time_count = 0;

static double min_vol;
static int stepflag = 0;

static int get_sonar = 0;		//�����g�Z���T�̒l
static int bottle_flag ;		//�y�b�g�{�g���̔��f
static int alarm_flag;

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */ 

int distancestop;	//��~���̋���
int distanceback;	//��ގ��̋���

//�o�b�e���d���l���
static U32	battery_value;

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;

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
	RN_STOP,					//��~

	RN_Right,
	RN_Left,
	RN_Stop,
	RN_Go,
	RN_Hit,
	RN_Back,


} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
//RN_SETTINGMODE setting_mode = RN_TYREAL;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;

RN_TAILMODE tail_mode = RN_TAILDOWN;


//�i�����m�֘A�}�N���A�v���g�^�C�v
static int RN_rapid_speed_up_signal_recevie(void);

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
static int remote_stop(void);  // ��~�p�֐� 
static int right(int angle);  //�E�����֐� 
static int stop(void);//�~�܂�֐�
static int goforward(void); //�i�ފ֐�
static int hit(void); //�Փˌ��m�֐�
static int back(void);

int shock(int target);
void tailcontrol();
void RA_linetrace_P(int forward_speed);
int RA_speed(int forward_speed);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
int rapid_speed_up(int target_gyro);
void self_location(void);
void battery_average_check(void);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
int abs(int value);

void sonar(void);
void get_bottle(void);
void alarm(void);

int distance(void);


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
	ecrobot_init_sonar_sensor(NXT_PORT_S2);			//�����g�Z���T�N��
	ecrobot_init_bt_slave("LEJOS-OSEK");

	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
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


//ON-OFF���䃉�C���g���[�X�֐�
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


//P���䃉�C���g���[�X�֐�
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


//PID���䃉�C���g���[�X�֐�
void RA_linetrace_PID(int forward_speed) {


	forward_speed = RA_speed(forward_speed);	//���x��i�K�I�ɕω�

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0005);
	d_hensa = (hensa - bf_hensa)/0.0005;
	bf_hensa = hensa;

	//cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	cmd_turn=-(Kp*hensa);

	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}

	/*�|������OFF��*/
	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

//PID����p�΍����Z�b�g�֐�
void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
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

//bluetooth���O���M�֐�
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

//���s�ݒ�֐�
void RN_setting()
{
	int distancestop;	//��~���̋���
	int distanceback;	//��ގ��̋���

	switch (setting_mode){
		/*
		case (RN_TYREAL):
			do_tyreal(&Kp,&Ki,&Kd);
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(10);
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;
		*/
			//���s�J�n�O
		case (RN_SETTINGMODE_START):
			RN_calibrate();				//�L�����u���[�V����
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

			//�ʏ푖�s
		case (RN_RUN):
			time_count++;

			RA_linetrace_PID(x);
			if(time_count > 500)
			{
				if(hit()==1)
				{
					setting_mode=RN_Back;
					time_count = 0;
					distancestop = distance();
				}
			}
			/*
                        if (remote_stop()==1)
 			{                  
 		 		x=0;
				setting_mode=RN_Right;
			}
			*/
			break;


			//�E������
		case(RN_Right):
		
		if(right(180)==1)
		{
			alarm();
			setting_mode=RN_Left;
			/*ecrobot_sound_tone(880, 512, 10);
			systick_wait_ms(500);
			*/
		}

		break;
	
			//��������
		case(RN_Left):

		if(right(-180)==1)
		{	
		setting_mode=RN_Stop;
	
		}
		break;

		//�~�܂�
		case(RN_Stop):
		stop();
		/*if(stop()==1)
		{
		setting_mode=RN_Go;
		}*/
		break;


		//������
		case (RN_Back):

			back();
			distanceback = distance();
			/*
			if(distanceback - distancestop < -30)	//��~�������ƌ�ނ��Ă��鎞�̋������r
			{
				setting_mode=RN_Stop;
			}
			*/
			break;


		//�i��
		case(RN_Go):

		if(goforward()==1)
		{
		setting_mode=RN_Hit;
		}

		break;


		//�Ԃ���
		case(RN_Hit):
		
		hit();
		break;

		default:
			break;

		}
}



//�L�����u���[�V�����֐�
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
			GYRO_OFFSET = gyro_offset;
			battery_value = ecrobot_get_battery_voltage();
			min_vol = battery_value;
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
						//tail_mode = RN_TAILUP;

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

//�E�������֐�
static int right(int angle)
{
	static int flag=0;

	if(flag==0)
	{
		ecrobot_set_motor_rev(NXT_PORT_C, 0);
		ecrobot_set_motor_rev(NXT_PORT_B, 0);

		flag=1;
	}

	if(flag==1&& abs(ecrobot_get_motor_rev(NXT_PORT_C)) <= abs(angle) && 
		abs(ecrobot_get_motor_rev(NXT_PORT_B)) <= abs(angle) )
	{

		if(angle>0)
		{
			ecrobot_set_motor_speed(NXT_PORT_C,  20);
			ecrobot_set_motor_speed(NXT_PORT_B, -20);

		}

		else
		{
			ecrobot_set_motor_speed(NXT_PORT_C, -20);
			ecrobot_set_motor_speed(NXT_PORT_B,  20);
		}
	}
	else
	{
		ecrobot_set_motor_speed(NXT_PORT_C, 0);
		ecrobot_set_motor_speed(NXT_PORT_B, 0);
		flag=0;
		return 1;
	}

	return 0;
}

//�~�܂�֐�
static int stop(void)
{



	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);

	return 1;
}

//���C���g���[�X�Ȃ��őO�i
static int goforward(void)
{

	ecrobot_set_motor_speed(NXT_PORT_C, 30);
	ecrobot_set_motor_speed(NXT_PORT_B, 30);

	return 1;
}

//���C���g���[�X�Ȃ��Ō��
static int back(void)
{

	static int Flag=0;
	/*
	ecrobot_set_motor_rev(NXT_PORT_C, 0);
	ecrobot_set_motor_rev(NXT_PORT_B, 0);
	*/
    ecrobot_set_motor_speed(NXT_PORT_C, -30);
	ecrobot_set_motor_speed(NXT_PORT_B, -30);
	/*
	if(abs(ecrobot_set_motor_rev(NXT_PORT_C))<= 90 && abs(ecrobot_set_motor_rev(NXT_PORT_B))<= 90)
	{
	ecrobot_set_motor_speed(NXT_PORT_C, -30);
	ecrobot_set_motor_speed(NXT_PORT_B, -30);
	}
	else
	{
	return 1;
	}
	*/
	return 1;

}


//�Ռ������m����֐�
static int hit(void)
{
	static U16 buf=600;
	static int hit_counter=0;
	int hit_result=0;

	hit_counter=hit_counter+1;

	if(hit_counter==3)
	{
		buf=ecrobot_get_gyro_sensor(NXT_PORT_S1);
		hit_counter=0;
	}


	if((buf)-(ecrobot_get_gyro_sensor(NXT_PORT_S1))>=20||(buf)-(ecrobot_get_gyro_sensor(NXT_PORT_S1))<=-20 )
	{
		ecrobot_sound_tone(880, 512, 10);
		systick_wait_ms(30);
		hit_result=1;
		//ecrobot_set_motor_speed(NXT_PORT_C, 0);
		//ecrobot_set_motor_speed(NXT_PORT_B, 0);
	}

	/*
	ecrobot_set_motor_speed(NXT_PORT_C, -50);
	ecrobot_set_motor_speed(NXT_PORT_B, -50);*/
	return hit_result;
}

void sonar()
{
	
	get_sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);

}

void get_bottle()
{
	bottle_flag = 0;
	if(get_sonar < BOTTLE)
		bottle_flag = 1;
}

void alarm()
{
	if(bottle_flag == 1){
		ecrobot_sound_tone(900,512,30);
		alarm_flag = 1;
	}
	else
	{
		alarm_flag = 0;
	}
		
}

int abs(int value)
{
	int result=0;

	if(value>=0){
	result=value;
	}
	else {
	result = -value;
	}
	
	return result;
}

int distance(void)
{
	int revL = 0;
	int revR = 0;

	revL = nxt_motor_get_count(NXT_PORT_C);
	revR = nxt_motor_get_count(NXT_PORT_B);
	
	return fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
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
			break;

		default:
			break;
	}
}
/*
//bluetooth���O���M�֐�
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
*/

/*
 *	�e��^�X�N
 */

//���s���@�Ǘ�(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//���s�̏��
	tailcontrol();		//�K���R���g���[��
	TerminateTask();
}

//���s��ԊǗ�(5ms)
TASK(ActionTask2)
{
	RN_setting();		//���s���
	TerminateTask();
}

//��ԕ\���Ǘ�(20ms)
TASK(DisplayTask)
{
//	ecrobot_status_monitor(target_subsystem_name);	//���j�^�o��
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,ecrobot_get_gyro_sensor(NXT_PORT_S1),distanceback - distancestop,0,0);		//���O���
	sonar();	//�����g�Z���T�̒l���X�V
	get_bottle();	//�y�b�g�{�g�������邩�ǂ����X�V
	TerminateTask();
}


/******************************** END OF FILE ********************************/

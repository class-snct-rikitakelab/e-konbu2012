

#include "Kaidan.h"
#include "logSend.h"
//#include "tyreal.h"
#include "math.h"


/*
 *	�e��ϐ���`
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

static int counter = 0;


//�K���ݒ�p�x
#define ANGLEOFDOWN 104			//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x

//���x���ߌW��
#define SPEED_COUNT 50

//�o�b�e���~���l
#define DOWN_BATTERY 450			//�o�b�e���~���l

//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

#define CMD_START '1'    			//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)

#define POSITION_X0 0
#define POSITION_Y0 0
#define THETA_0 0

#define WHEEL_R		41
#define MACHINE_W	162

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��

static float Kp = 1.85;				//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;				//D����p


static int wait_count = 0;

static double min_vol;
static int stepflag=0;

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */

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

/*
 *	��Ԓ�`
 */

//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_CONTROL,				//�|������ON
	RN_MODE_STOP,					//�|������OFF
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_BALANCEOFF,					//�|������OFF
} RN_MODE;


typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_SLOW_RUN,				//�ᑬ���s
	RN_STOP,						//��~
	RN_STOP_WAIT,
	RN_RUPID_SPEED_UP,
	RN_STEP_BRAKE,				//�u���[�L
	RN_STEP_STOP,				//��~
	TYREAL,
} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILDOWN;


//�i�����m�֘A�}�N���A�v���g�^�C�v
#define RUPID_SPEED_UP_SIGNAL 3
static int RN_rupid_speed_up_signal_recevie(void);

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);

int shock();
void tailcontrol();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
void rupid_speed_up(int target_forward_speed);
void self_location(void);

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
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
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

//PID����p�΍����Z�b�g�֐�
void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//�i�K�I�����p�֐��i�w��ʂ������x�����X�ɏ㏸�j
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	counter ++;

	if(counter >= SPEED_COUNT)
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


//�ԗ։�]�ʍ����ߊ֐��iPID����j
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}


//�Ռ����m�֐�
int shock(void){
	int result=0;
	//�d���~���̍ŏ��l���X�V
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//�W���C���y�ѓd���~������Ռ����m
	if((ecrobot_get_gyro_sensor(NXT_PORT_S1) <= gyro_offset-PM_GYRO ||ecrobot_get_gyro_sensor(NXT_PORT_S1) >= gyro_offset+PM_GYRO) 
		&& min_vol <= battery_value-DOWN_BATTERY)
	{
		ecrobot_sound_tone(880,512,30);

		revL = nxt_motor_get_count(NXT_PORT_C);
		revR = nxt_motor_get_count(NXT_PORT_B);

		distance_before_step = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//�i���˓����̋����𑪒�


	//	setting_mode = RN_SLOW_RUN; 
		
		stepflag = 1;
		result = 1;
		//setting_mode = RN_STEP_BRAKE;		//�K�i�֌������u���[�L��������

		min_vol = battery_value;			//�ŏ��l���Z�b�g
	}
	return result;
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

	static const float t_Kp = 0.7;

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

//�����[�g�X�^�[�g�֐�
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



//���s�ݒ�֐�
void RN_setting()
{
//	static float beforestop = 0;

	switch (setting_mode){
			//���s�J�n�O
		case (RN_SETTINGMODE_START):
			RN_calibrate();				//�L�����u���[�V����
			break;

			//�ʏ푖�s
		case (RN_RUN):
			RA_linetrace_PID(25);
			wait_count = 0;
			if(remote_start()==1)
			{
				gyro_offset += 17;				
				/*while(1)
				{
					RA_linetrace_PID(25);
					wait_count++;
					if(wait_count == 400){
						gyro_offset -= 100;
						tail_mode = RN_TAILDOWN;
					}
					if(wait_count > 500)
					{
						setting_mode = RN_STOP;
						tail_mode = RN_TAILDOWN;
						runner_mode = RN_MODE_BALANCEOFF;
						break;
					}
				}
				*/
			}



			/*
			if(cmd_forward < 50)
				RA_speed(50,1);
			cmd_turn = RA_wheels(cmd_turn);
			*/
			/*
			if(1==RN_rupid_speed_up_signal_recevie()){
			ecrobot_sound_tone(880, 512, 30);
			systick_wait_ms(500);
			setting_mode = RN_RUPID_SPEED_UP;
			}

			//��񂾂��i�����m
			if(stepflag == 0){
			if(shock()==1){
				ecrobot_sound_tone(180, 512, 30);
			systick_wait_ms(500);
			
				setting_mode = RN_STOP;
			}
			}
			*/
			/*
			if ( tyreal_trigger() == 1) {
				ecrobot_sound_tone(932, 512, VOL);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, VOL);
				systick_wait_ms(10);
				setting_mode = TYREAL;
			}
			*/

			/*

			RA_linetrace_PID(45);		//���C���g���[�X
			shock();					//�i�����m
			*/
			break;

			//��苗�����u���[�L
		case(RN_STEP_BRAKE):
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);

			distance_step_brake = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//���݂̑��s�������v��

			RA_speed(-30,1);								//���X�Ƀu���[�L���������Ă����i�O�̏�Ԃ̑��x�̂܂܂ł͎~�܂��ė~�����ʒu�Ŏ~�܂��Ă���Ȃ����߁A�u���[�L����������j
			cmd_turn = RA_wheels(cmd_turn);					//���ԗւ̉�]�ʂ𓯂��ɂ��āA�������]�v�ɂ����̂�h��

			//��苗�����m��A��~���[�h��
			if((distance_before_step - distance_step_brake <= -10))	//-10��NXT Communicator�̃f�[�^���画�f
			{
				balance_init();										//�o�����T�[��������
				nxt_motor_set_count(NXT_PORT_B,0);					//���[�^�̉�]�ʂ�������
				nxt_motor_set_count(NXT_PORT_C,0);
				setting_mode = RN_STEP_STOP;						//��~��Ԃ�
			}

			break;


			//��~���
		case(RN_STEP_STOP):
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_step_stop = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//���݂̑��s��������

			wait_count++;			//���ԃJ�E���g
			RA_linetrace(0,2);		//ON/OFF����̑��x0���C���g���[�X�ňʒu�𐮂���

			/*
			//��苗�����s��A�ʏ푖�s��
			if(distance_step_stop-distance_step_brake < 0)
			{
				balance_init();
				nxt_motor_set_count(NXT_PORT_B,0);
				nxt_motor_set_count(NXT_PORT_C,0);
				RA_hensareset();
				wait_count = 0;
				setting_mode = RN_RUN;
				tail_mode = RN_TAILUP;
			}
			*/

			//��莞�Ԍo�ߌ�ʏ푖�s��
			if(wait_count >= 550)
			{
				balance_init();						//�o�����T�[������
				nxt_motor_set_count(NXT_PORT_B,0);	//���[�^�̉�]�ʏ�����
				nxt_motor_set_count(NXT_PORT_C,0);
				RA_hensareset();					//���C���g���[�X�̕΍�������
				wait_count = 0;						//�J�E���^������
				setting_mode = RN_RUN;				//�ʏ푖�s�Ɉڍs
				tail_mode = RN_TAILUP;				//�K�����グ��
			}

			break;

			//������~
		case(RN_STOP):
		/*
			tailcontrol();
			 tail_mode = RN_TAILDOWN;
			*/ 
			
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			/*
			RA_linetrace_PID(3);
			cmd_turn = RA_wheels(cmd_turn);
			*/
			/*
			cmd_turn=0;
			cmd_forward=0;
			*/
			/*
			if(1==RN_rupid_speed_up_signal_recevie()){
			ecrobot_sound_tone(880, 512, 30);
			systick_wait_ms(500);
			setting_mode = RN_RUPID_SPEED_UP;
			}
			*/
			break;
		case (RN_RUPID_SPEED_UP):
			rupid_speed_up(80);
			ecrobot_sound_tone(880,512,15);
			/*
		case(TYREAL):
			do_tyreal();
			break;
			*/
		default:
			break;
	}
}

//�}�����p�֐�
void rupid_speed_up(int target_forward_speed){
	static int rupid_speed_up_counter=0;
	int gyro_offset_operation = 10;
	
	rupid_speed_up_counter++;
	cmd_turn = RA_wheels(cmd_turn);

	if(rupid_speed_up_counter<2){
	gyro_offset = gyro_offset + gyro_offset_operation;
	//	cmd_forward++;
		//gyro_offset = gyro_offset - 0.5;
	}
	else if(rupid_speed_up_counter==100){
	//rupid_speed_up_counter=0;
	gyro_offset = gyro_offset-gyro_offset_operation;
		//setting_mode = RN_STOP;
	}
	if(rupid_speed_up_counter>2 && rupid_speed_up_counter<150 ) {
	RA_linetrace_PID(25);
	
	}
	//RA_linetrace_PID(25/*target_forward_speed*/);
	if(rupid_speed_up_counter>150) {
	RA_linetrace_PID(50/*target_forward_speed*/);
	
	}
	if(rupid_speed_up_counter>200) {
	RA_linetrace_PID(50/*target_forward_speed*/);
	rupid_speed_up_counter=0;
	setting_mode = RN_STOP;
	}

	/*
	int forward_hensa;

	forward_hensa = target_forward_speed - cmd_forward;
	

	cmd_forward = Kp*forward_hensa;

	if (cmd_forward < -100) {
		cmd_forward = -100;
	}
	else if (cmd_forward > 100) {
		cmd_forward = 100;
	}
	*/


	/*
	limit ���ڕW�l
	s_Kp�͈�񂠂���ɉ������鑀��ʁH
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
	*/

}

static int RN_rupid_speed_up_signal_recevie(void)
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
		if (rx_buf[0] == RUPID_SPEED_UP_SIGNAL)
		{
			start = 1;
		}
		
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}
	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
	{
		
			start = 1; 

	}

	return start;
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
			battery_value = ecrobot_get_battery_voltage();
			min_vol = battery_value;
			systick_wait_ms(500);
			break;
		}
	}

	//���s�J�n���}
	while(1){

		//�����[�g�X�^�[�g
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,10);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCE;
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
						runner_mode = RN_MODE_BALANCE;
						tail_mode = RN_TAILUP;
						break;
					}
				}
			break;
		}
	}

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


		case (RN_MODE_STOP):
			//�o�����T�[OFF
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
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
	RN_modesetting();	//���s�̏��
	tailcontrol();		//�K���R���g���[��
	self_location();	//���Ȉʒu����
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
	ecrobot_status_monitor(target_subsystem_name);	//���j�^�o��
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,position_x,position_y/*min_vol*/,
			velocity,ecrobot_get_gyro_sensor(NXT_PORT_S1));		//���O���
	TerminateTask();
}


/******************************** END OF FILE ********************************/

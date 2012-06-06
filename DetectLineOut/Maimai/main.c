/*
 * main.c
 * 
 */

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
#include "logSend.h"
#include "TimerUnit.h"
#include "DetectOnLine.h"

//���C���O�ꂽ���o�p�ϐ�


//���C���g���[�X�p�ڕW�l
static unsigned int black_value;	//���l
static unsigned int white_value;	//���l
static unsigned int gray_value;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
static unsigned int line_mokuhyou_value;		//���C���g���[�X�ڕW�l

static int counter = 0;

//�K���ݒ�p�x
#define ANGLEOFDOWN 118				//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x
#define ANGLEOFPUSH 210				//����ڕW�p�x�i���g�p�j

//���x���ߌW��
#define SPEED_COUNT 20

//�o�b�e���~���l
#define DOWN_BATTERY 450			//�o�b�e���~���l

//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

#define CMD_START '1'    			//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)

#define MANDATORY_START_RUN_SIGNAL '2' //���C�����O��邽�߂̑��s�ւ̏�ԑJ�ڐM��

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��
static float Kp = 1.85;				//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;				//D����p
//static float Kp = 0.648;
//static float Ki = 0.35;
//static float Kd = 0.0205;

static int wait_count = 0;

static int mode_count = 0;

static double min_vol;

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
float distance_now = 0;
float distance_before = 0;
float distance_slow = 0;
float distance_stop = 0;


//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_CONTROL,				//�|������ON
	RN_MODE_STOP,					//�|������OFF
} RN_MODE;


//���s��Ԑؑ֗p��enum
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_STOP,						//��~
	RN_MANDATORY_LINE_OUT
} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
	RN_TAILPUSH					//�K�����o
} RN_TAILMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILDOWN;


//�e��v���C�x�[�g�֐�
void RN_stop();
void RN_calibrate();
void RN_run_state_change();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void shock();
void taildown();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);

static int RN_mandatory_run_remotestart(void); //���C���������s�؂�ւ��M���p�֐�
void initialze_PID_param();


//���C���O��邽�߂̊֐�
void RN_mandatory_line_out(U8 port_B_speed,U8 port_C_speed);

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
const char target_subsystem_name[] = "DetectLineOut";


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

	hensa = (float)line_mokuhyou_value - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

	cmd_turn = -(1.4 * hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
}


//PID���䃉�C���g���[�X
void RA_linetrace_PID(int forward_speed) {

	if(runner_mode == RN_MODE_CONTROL){
		shock();
	}
	RA_speed(forward_speed,2);	//���x��i�K�I�ɕω�
	cmd_forward = forward_speed;

	if(forward_speed > 0)
		hensa = (float)line_mokuhyou_value - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)line_mokuhyou_value;

	i_hensa = i_hensa+(hensa*0.0004/*0.0005*/);
	d_hensa = (hensa - bf_hensa)/0.0004/*0.0005*/;
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
//	static int counter = 0;

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


//�Ռ����m
void shock(void){
	/*
	//�ŏ��l�ݒ�
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//�W���C���y�ѓd���~�����t���O�Ƃ���
	if((ecrobot_get_gyro_sensor(NXT_PORT_S1) <= gyro_offset-PM_GYRO ||
		ecrobot_get_gyro_sensor(NXT_PORT_S1) >= gyro_offset+PM_GYRO) &&
			min_vol <= battery_value-DOWN_BATTERY)
	{
		ecrobot_sound_tone(880,512,30);
		//�o�����T�[�����[�h1���ǂ���
		if(runner_mode == RN_MODE_CONTROL){
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			setting_mode = RN_SLOW_RUN;
			runner_mode = RN_MODE_CONTROL_2;	//�J��Ԃ����m��h�����߂Ƀo�����T�[�����[�h2
			gyro_offset -= 10;					//�W���C���I�t�Z�b�g�؂�ւ�
			min_vol = battery_value;			//�ŏ��l���Z�b�g
		}
	}
	*/
}


//ON-OFF����p���C������
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);

	if (line_mokuhyou_value > light_value) {
		if ((line_mokuhyou_value) > light_value) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	return TRUE;
}


//�K���p�x�ێ�
void taildown(){

	static const float t_Kp = 0.7;

	static float t_hensa = 0;
	static float t_speed = 0;

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

//�����[�g�X�^�[�g�p�֐�
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

//���C���������s�M����M�p�֐�
static int RN_mandatory_run_remotestart(void)
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
		if (rx_buf[0] == MANDATORY_START_RUN_SIGNAL)
		{
			start = 1; /* ���s�J�n */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	return start;
}


//���s��Ԑ؂�ւ�
void RN_run_state_change()
{
	//static float beforestop = 0;

	switch (setting_mode){
			//�L�����u���[�V����
		case (RN_SETTINGMODE_START):
			RN_calibrate();  //after calibarate cahge to RU_RUN
			break;

			//�ʏ푖�s
		case (RN_RUN):
			RA_linetrace_PID(20); //�ʏ푖�s�p�֐�
			checkVariation();
			
			/*
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			*/
			//PC����̐M������M�i���̏ꍇ'2'�j����M�������ԑJ��
			if(RN_mandatory_run_remotestart()==1){
				setting_mode = RN_MANDATORY_LINE_OUT;
			
			}
			break;

			//������~
		case(RN_STOP):
			RN_stop();
			break;

	
		case(RN_MANDATORY_LINE_OUT):
			RN_mandatory_line_out(30,0);
			break;

		default:
			break;
	}
}

void RN_stop(){
	cmd_turn=0;
	cmd_forward=0;

}

void initialze_PID_param(){

	hensa = 0;
	i_hensa =0;
	d_hensa =0;
	bf_hensa =0;
}

//�L�����u���[�V����
void RN_calibrate()
{

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 30);
			black_value=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 30);
			white_value=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//�D�F�l�v�Z
	gray_value=(black_value+white_value)/2;
	//���C���g���[�X�ڕW�l�̌v�Z
	line_mokuhyou_value =(black_value*0.6+white_value*0.4);

	//�W���C���I�t�Z�b�g�y�уo�b�e���d���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 30);
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
			ecrobot_sound_tone(982,512,30);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_CONTROL; //�o�����T�[�N��
			break;
		}

		//�^�b�`�Z���T�X�^�[�g
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,30);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_CONTROL; //�o�����T�[�N��
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

		case (RN_MODE_STOP):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}

//���C���𖳎��������s�����A�P�b�o�߂�����ʏ푖�s�֖߂�B
void RN_mandatory_line_out(U8 port_B_speed,U8 port_C_speed){
	
	/*
	static int flag =0;
	if(flag ==0) {
	nxt_motor_set_speed(NXT_PORT_C,port_C_speed,1);
	nxt_motor_set_speed(NXT_PORT_B,port_B_speed,1);
	}
	else {
		setting_mode = RN_RUN; //PID��p�����ʏ푖�s�֖߂�B
	
	}
	*/
	

	//nxt_motor_set_speed(NXT_PORT_C,port_C_speed,1);
	//nxt_motor_set_speed(NXT_PORT_B,port_B_speed,1);
	cmd_turn=port_B_speed;
	if (TU_is_start()/* && flag*/ ==0) {
		/** 4�b�̃^�C�}�Ď��J�n */
	/*
		ecrobot_sound_tone(880, 512, 30);
	systick_wait_ms(300); //����炷
	*/
		TU_start(1000); 
	}

	if (TRUE == TU_is_timeout()) {
	//balance_init();						/* �|���U�q���䏉���� */
	//nxt_motor_set_count(NXT_PORT_C, 0); /* �����[�^�G���R�[�_���Z�b�g */
	//nxt_motor_set_count(NXT_PORT_B, 0); /* �E���[�^�G���R�[�_���Z�b�g */
	//initialze_PID_param();
	TU_finish(); /** �^�C�}�Ď����I�������� */ 	
	//flag =1; //���^�C�}���g����������g��Ȃ��B
	/*
	ecrobot_sound_tone(440, 512, 30);
	systick_wait_ms(500); //����炷
	*/
	setting_mode = RN_RUN; //PID��p�����ʏ푖�s�֖߂�B
	
		
	}
		
	
}


void rapid_speed_up(){


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
	RN_run_state_change();
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
	logSend(cmd_forward,cmd_turn,ecrobot_get_battery_voltage(),getLightSensorVarieation()/*min_vol*/,
			distance_before - distance_slow,ecrobot_get_gyro_sensor(NXT_PORT_S1));
	TerminateTask();
}


/******************************** END OF FILE ********************************/

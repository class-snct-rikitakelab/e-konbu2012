/*
 * lookupgate.c
 * ���b�N�A�b�v�Q�[�g�ʉ߃v���O����
 */


#include "lookupgate.h"


/*
 *	�e���`
*/


//�K���ݒ�p�x
#define ANGLEOFDOWN 110				//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x
#define ANGLEOFPUSH 210				//����ڕW�p�x�i���g�p�j
#define ANGLEOFLOOKUP 58

//���x�J�E���^�̏���l
#define SPEED_COUNT 20

//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��

#define CMD_START '1'    			//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)


/*
 *	�O���[�o���ϐ�
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
static unsigned int LOOKUP_BLACK_VALUE;		//�p�x��ANGLEOFLOOKUP���̍��l
static unsigned int LOOKUP_WHITE_VALUE;		//�p�x��ANGLEOFLOOKUP���̔��l
static unsigned int LOOKUP_GRAY_VALUE;		//�p�x��ANGLEOFLOOKUP�n�̊D�F�l�i���݂͍��Ɣ��̕��ϒl�j

//���x���߃J�E���^�i�J�E���^������ɒB���閈�ɑ��x��1�㏸
static int speed_counter = 0;


//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��
static float Kp = 1.85;				//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;			//D����p

//�K��PI����p�W��
static float t_Kp = 1.85;			//P����p
static float t_Ki = 0;				//I����p

//�K������p�ϐ�
static int t_angle = 0;				//���݂̊p�x
static int t_count = 0;				//���䑬�x���ߗp�J�E���^
static int t_value = 0;				//�p�x�ڕW�l
static int t_count_limit = 0;		//�J�E���^�ő�l
static int t_up = 0;				//�����l

//�S�̗p�J�E���^�i���ԉ҂��p�j
static int wait_count = 0;

//�����g�Z���T�ڕW�l
static int target_sonar = 20;

//�����g�Z���T�p�t���O
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


/*
 *	�e���Ԓ�`
 */


//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_BALANCEOFF				//�|������OF
} RN_MODE;


//�L�����u���[�V�����̏��
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_LOOKUP,					//���b�N�A�b�v�Q�[�g����
	RN_LOOKUPDOWN,				//���s�̍~��
	RN_LOOKUPMOVE,				//���s�̑O�i
	RN_LOOKUPUP					//���s�̕��A
} RN_SETTINGMODE;

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;

//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	�e��v���C�x�[�g�֐���`
 */


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
int getsonarflag(int target_sonar);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void RA_linetrace_PID_balanceoff(int forward_speed);

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



/*
 *	�֐�����
 */


//���������֐��i�v���O�����̍ŏ��ɌĂяo���j
void ecrobot_device_initialize(void)
{

	ecrobot_set_light_sensor_active(NXT_PORT_S3);	//���Z���T�N��
	ecrobot_init_bt_slave("LEJOS-OSEK");			//Bluetooth�N��
	ecrobot_init_sonar_sensor(NXT_PORT_S2);			//�����g�Z���T�N��

	//���[�^���Z�b�g
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}



//��n�������֐��i�v���O�����I�����Ăяo���j
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;							//�K�����グ��i�Ӗ�����̂��H�j

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);	//���Z���T�I��
	ecrobot_term_sonar_sensor(NXT_PORT_S2);			//�����g�Z���T�I��
	ecrobot_term_bt_connection();					//Bluetooth�I��

	//���[�^���Z�b�g
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A, 0);			
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
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

//PID���䃉�C���g���[�X�֐��i�o�����T�[OFF�p�j
void RA_linetrace_PID_balanceoff(int forward_speed){
	RA_speed(forward_speed,2);	//���x��i�K�I�ɕω�

	if(forward_speed > 0)
		hensa = (float)LOOKUP_GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)LOOKUP_GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0005);
	d_hensa = (hensa - bf_hensa)/0.0005;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}

	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);
}

//�i�K�I�����p�֐�
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	speed_counter++;						//���x���ߗp�J�E���^

	if(speed_counter >= SPEED_COUNT)		//�J�E���^���ő�ő��x�ύX��
	{

		forward_speed = cmd_forward;

		if(limit-forward_speed >= 0){
			forward_speed += s_Kp;		//�w��ʂ������x�㏸

			if(forward_speed > limit)
				forward_speed = limit;
		}
		else{
			forward_speed -= s_Kp;		//�w��ʂ������x����

			if(forward_speed < limit)
				forward_speed = limit;
		}

		cmd_forward = forward_speed;
		speed_counter = 0;					//�J�E���^���Z�b�g
	}
}


//2�ԗւ̉�]�ʍ��AP����֐��i�ڕW�l�ɋ߂Â���j
int RA_wheels(int turn){
	float w_kp = 1.4;	//��]��P����p�����[�^

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);	//��]�ʍ�
	turn = w_kp * def;	//��]�ʎZ�o

	return turn;
}


//ON-OFF����p���C������֐�
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);	//���݂̋P�x�l
					
	if (GRAY_VALUE > light_value)		//�P�x�l���ڕW�l���傫�������f
		return FALSE;					//���C���O
	else
		return TRUE;					//���C����

}

//�����g�Z���T��Ԍ��o�֐�
void sonarcheck(void)
{
	if(ecrobot_get_sonar_sensor(NXT_PORT_S2) <= target_sonar)	//�����g�Z���T�̒l���ڕW�l�ȉ������f���t���O�ύX
	{
		sonarflag = 1;
	}
	else
		sonarflag = 0;
}

//�����g�Z���T��Ԋm�F�֐�
int getsonarflag(int target)
{
	target_sonar = target;				//�����g�Z���T�̖ڕW�l���w��

	return sonarflag;					//�w�肵���l�ɋ߂Â��Ă��邩�ǂ�����Ԃ��i1:�͈͓��j
}


//���s�̃��[�h�ύX�֐��i��Ƀo�����T�[��ON/OFF�j
void runner_mode_change(int flag)
{
	switch(flag){
	case 0:
		runner_mode = RN_MODE_INIT;			//���s�̏������
		break;
	case 1:
		runner_mode = RN_MODE_BALANCE;		//�o�����T�[ON
		break;
	case 2:
		runner_mode = RN_MODE_BALANCEOFF;	//�o�����T�[OFF
		break;
	default:
		break;
	}
}

//�K������p�����[�^�ύX�֐�
void tailpower(float value)
{
	t_Kp = value;							//�����̒l�ɐK������̃p�����[�^��ύX
}


//�K���p�x���ߊ֐�
void taildown(){

	//X-ecrobot_get_motor_rev(NXT_PORT_S4) ��X = �ڕW�l
	//�ڕW�l��1������(���s�ڕW�lt_angle���̂��̂��ŏI�ڕW�lt_value�֋߂Â��Ă���)
	//t_angle�̏����l�͌��݂̊p�x

	static float t_hensa = 0;	//�K���p�x�̖ڕW�l�Ƃ̍�
	static float t_ihensa = 0;	//I����p�΍�

	static float t_speed = 0;	//�K�����[�^�ɑ��鑬�x

	t_count++;					//���x����p�J�E���^


	/*	��̏�Ԃłł��Ȃ����ǂ����v�Ē�
	if(t_angle < t_value)
	{
		t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
		if(t_count >= t_count_limit)	//�J�E���^�ŐK���������鑬�x�𒲐�
		{
			t_angle+=t_up;			//�p�x���グ��
			t_count = 0;
		}
	}

	else if(t_angle > t_value)
	{
		t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
		if(t_count >= t_count_limit)	//�J�E���^�ŐK�����グ�鑬�x�𒲐�
		{
			t_angle-=t_up;			//�p�x��������
			t_count = 0;
		}
	}

	else
	{
		t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
	}
	*/


	switch(tail_mode){
		case(RN_TAILDOWN):				//�K����������
			if(t_angle <= t_value)		//���݂̊p�x���ڕW�l�ȉ����ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//�J�E���^�ŐK���������鑬�x�𒲐�
				{
					t_angle+=t_up;			//�p�x���グ��
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}
			
			break;

		case(RN_TAILUP):				//�K�����グ��
			if(t_angle >= t_value)		//���݂̊p�x���ڕW�l�ȏォ�ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//�J�E���^�ŐK�����グ�鑬�x�𒲐�
				{
					t_angle-=t_up;			//�p�x��������
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}
			
			break;

		default:
			break;
			
	}
	
	t_ihensa = t_ihensa+(t_hensa*0.0004);		//I����p�΍�

	t_speed = (t_Kp*t_hensa + t_Ki*t_ihensa);	//�΍������ɑ��x����

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);	//���[�^�ɑ��x�𑗂�

}

//�K�����[�h�ύX�֐�
void tail_mode_change(int mode,int value,int limit,int up)	//mode(0:�K�������낷�A1:�K�����グ��) value(�ڕW�l) limit(�グ��قǒx���j up�i�グ��قǉ����j1
{

	static int flag;					//�K�����[�h�t���O

	switch(mode){
		case 0:
			tail_mode = RN_TAILDOWN;	//�K�������낷
			flag = 0;
			break;
		case 1:
			tail_mode = RN_TAILUP;		//�K�����グ��
			flag = 1;
			break;
		default:
			break;
	}

	//�ڕW�l���ς�������̂݁A�p�x�����߂�i������Ăяo���ꍇ�������l���L�[�v���邽�߁j
	if(t_value != value)
		t_angle = ecrobot_get_motor_rev(NXT_PORT_A);

	t_value = value;			//�ڕW�l�ݒ�
	t_count_limit = limit;		//�J�E���^�ő�l�ݒ�
	t_up = up;					//�p�x�����l�ݒ�

}

//�����[�g�X�^�[�g�Ǘ��֐�
static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//��ԃt���O

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; //��M�o�b�t�@���N���A
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		//��M�f�[�^����
		if (rx_buf[0] == CMD_START)
		{
			start = 1; //���s�J�n�t���O
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); //��M�f�[�^���G�R�[�o�b�N
	}

	return start;
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


//���s��Ԑݒ�֐�
void RN_setting()
{

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_SETTINGMODE_START):
			tail_mode_change(0,ANGLEOFDOWN,1,2);
			RN_calibrate();
			break;

			//�ʏ푖�s���
		case (RN_RUN):
			RA_linetrace_PID(30);

			wait_count++;

			if(wait_count >= 200)					//�X�^�[�g���ɔ�������̂�h���i�e�X�g�p�j
			{

				if(getsonarflag(22) == 1)				//�����g�Z���T�������������ǂ���
				{
					ecrobot_sound_tone(900,512,30);
					setting_mode = RN_LOOKUP;
					wait_count = 0;
				}
			}
			
			break;

			//���b�N�A�b�v�Q�[�g���s�������
		case (RN_LOOKUP):
			RA_speed(0,3);
			cmd_turn = RA_wheels(cmd_turn);

			wait_count++;

			if(cmd_forward <= 0 && wait_count == 400)
			{
				tail_mode_change(0,ANGLEOFDOWN,0,1);
				while(wait_count <= 800 || t_angle <= ANGLEOFDOWN)
				{
					RA_speed(-15,3);
					cmd_turn = RA_wheels(cmd_turn);
					wait_count++;
				}
				gyro_offset -= 280;
				setting_mode = RN_LOOKUPDOWN;
				wait_count = 0;
				cmd_forward = 0;
				runner_mode_change(2);
			}

			break;

			//���b�N�A�b�v�Q�[�g���s�A�K���~��
		case (RN_LOOKUPDOWN):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			wait_count++;

			if(wait_count >= 800)
			{
				tailpower(10.0);
				tail_mode_change(1,ANGLEOFLOOKUP,10,1);
				if(t_angle == ANGLEOFLOOKUP)
					{
						setting_mode = RN_LOOKUPMOVE;
						wait_count = 0;
					}
			}

			break;

			//���b�N�A�b�v�Q�[�g���s�A�K���~����ԂőO�i
		case (RN_LOOKUPMOVE):

			RA_linetrace_PID_balanceoff(30);

			wait_count++;
			
			if(wait_count == 430)
			{	
				tailpower(15.0);
				setting_mode = RN_LOOKUPUP;
				wait_count = 0;
			}
			
			break;

			//���b�N�A�b�v�Q�[�g���s�A�O�i��|����Ԃ֕��A
		case (RN_LOOKUPUP):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			wait_count++;

			if(wait_count == 200)
				tail_mode_change(0,ANGLEOFDOWN,10,1);
			
			if(t_angle == ANGLEOFDOWN && wait_count >= 1200)
			{
				runner_mode_change(1);
				tailpower(1.85);
				tail_mode_change(1,ANGLEOFUP,1,2);
				gyro_offset += 280;
				setting_mode = RN_RUN;
			}
			
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

	//�K�������b�N�A�b�v�Q�[�g���̊p�x��
	tail_mode_change(1,ANGLEOFLOOKUP,0,2);

	//���b�N�A�b�v�Q�[�g�p���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			LOOKUP_BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���b�N�A�b�v�Q�[�g�p���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			LOOKUP_WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���b�N�A�b�v�Q�[�g�p�D�F�l�v�Z
	LOOKUP_GRAY_VALUE=(LOOKUP_BLACK_VALUE+LOOKUP_WHITE_VALUE)/2;

	//�K���𒼗���~��Ԃ̊p�x��
	tail_mode_change(0,ANGLEOFDOWN,0,2);

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
	while(1)
	{
		//�����[�g�X�^�[�g
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			tail_mode_change(1,ANGLEOFUP,0,2);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCE;
			break;
		}

		//�^�b�`�Z���T�X�^�[�g
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			while(1)
			{
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_BALANCE;
						tail_mode_change(1,ANGLEOFUP,0,2);
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

			//���s�̏������
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//�o�����T�[
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

			//�o�����T�[����
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}


/*
 *	�e��^�X�N
 */

//���s�̊Ǘ��^�X�N(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//���s�̏�Ԑݒ�
	taildown();			//�K������
	TerminateTask();
}

//���s��ԊǗ��^�X�N(5ms)
TASK(ActionTask2)
{
	RN_setting();		//���s��Ԑݒ�
	TerminateTask();
}

//��ԕ\���Ǘ��^�X�N(20ms)
TASK(DisplayTask)
{
	ecrobot_status_monitor(target_subsystem_name);	//���j�^�ɏ�ԕ\��
	TerminateTask();
}

//���O���M�A�����g�Z���T�Ǘ��^�X�N(50ms) (����50ms�łȂ���Γ��삵�Ȃ��j
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,wait_count,t_value,		//Bluetooth��p���ăf�[�^���M
			LOOKUP_GRAY_VALUE,GRAY_VALUE);

	sonarcheck();															//�����g�Z���T��ԊǗ�

	TerminateTask();
}


/******************************** END OF FILE ********************************/
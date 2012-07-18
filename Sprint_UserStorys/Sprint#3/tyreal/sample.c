/**
 ******************************************************************************
 **	�t�@�C���� : sample.c
 **
 **	�T�v : �^�C�����������؃v���O����
 **
 **
 ******************************************************************************
 **/
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
#include "logSend.h"
#include "tyreal.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ���L�̃}�N���͌�/���ɍ��킹�ĕύX����K�v������܂� */
/* sample_c1�}�N�� */
#define GYRO_OFFSET  600 /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define LIGHT_WHITE	 500 /* ���F�̌��Z���T�l */
#define LIGHT_BLACK	 700 /* ���F�̌��Z���T�l */
/* sample_c2�}�N�� */
#define SONAR_ALERT_DISTANCE 30 /* �����g�Z���T�ɂ���Q�����m����[cm] */
/* sample_c3�}�N�� */
#define TAIL_ANGLE_STAND_UP 107 /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3 /* �o�����X���s���̊p�x[�x] */
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */
/* sample_c4�}�N�� */
#define DEVICE_NAME       "KINGYO"  /* Bluetooth�ʐM�p�f�o�C�X�� */
#define PASS_KEY          "1234" /* Bluetooth�ʐM�p�p�X�L�[ */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h(�ύX�֎~) */

//Macro for PID 
#define MOKUHYOU 600
#define DELTA 0.004


/* �֐��v���g�^�C�v�錾 */
static int sonar_alert(void);
static void tail_control(signed int angle);
static int remote_start(void);




//char val_string[INT_EXPRESS_RENGE+TYRE_ANGLE_CHAR_NUM];
/* Bluetooth�ʐM�p�f�[�^��M�o�b�t�@ */
char rx_buf[BT_MAX_RX_BUF_SIZE]; 
int rx_buf_int[BT_MAX_RX_BUF_SIZE];



//�p�����[�^�����p�v���O�����̏��


//*****************************************************************************
// �֐��� : ecrobot_device_initialize
// �� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : ECROBOT�f�o�C�X�������t�b�N�֐�
//*****************************************************************************
void ecrobot_device_initialize()
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3); /* ���Z���T�ԐFLED��ON */
	ecrobot_init_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)������ */
	nxt_motor_set_count(NXT_PORT_A, 0); /* ���S��~�p���[�^�G���R�[�_���Z�b�g */
	ecrobot_init_bt_slave(PASS_KEY); /* Bluetooth�ʐM���� */
}

//*****************************************************************************
// �֐��� : ecrobot_device_terminate
// �� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : ECROBOT�f�o�C�X�I�������t�b�N�֐�
//*****************************************************************************
void ecrobot_device_terminate()
{
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* ���Z���T�ԐFLED��OFF */
	ecrobot_term_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)���I�� */
	ecrobot_term_bt_connection(); /* Bluetooth�ʐM���I�� */
}

//*****************************************************************************
// �֐��� : user_1ms_isr_type2
// �� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : 1msec���荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//*****************************************************************************
void user_1ms_isr_type2(void){}

//*****************************************************************************
// �^�X�N�� : TaskMain
// �T�v : ���C���^�X�N
//*****************************************************************************


TASK(TaskMain)
{
	signed char forward;   /* �O��i���� */
	signed char turn;         /* ��񖽗� */
	signed char pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	
	//PID����p�ϐ��Q
	static float Kp = 0.648;
	static float Ki = 0.35;
	static float Kd = 0.0205;
	
	static float now_diff = 0;
	static float before_diff = 0;
	static float integral = 0;
	
	static float gyro_offset = 600;  /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
	static float  light_white = 500; /* ���F�̌��Z���T�l */
static float light_black =700; /* ���F�̌��Z���T�l */
	//���O���M�p�ϐ�
		S8 data1=0;
		S8 data2=0;
		S16 adc1=0;
		S16 adc2=0;
		S16 adc3=0;
		S16 adc4=0;
		S32 valOfDistance=0;
		
	/**
	 * Bluetooth�ʐM�p�f�o�C�X���̕ύX�́ABluetooth�ʐM�ڑ����m������Ă��Ȃ��ꍇ�̂ݗL��ł��B
	 * �ʐM�ڑ��m�����ɂ̓f�o�C�X���͕ύX����܂���B(���L��API�͉������܂���)
	 */
	ecrobot_set_bt_device_name(DEVICE_NAME);

	/*
	 int fw=50;
	int tn=0;
	int tail_angle=0;
	float Kp_a=3.212;
	float Ki_b=2.223;
	char int_string[INT_EXPRESS_RENGE];
	char float_string[100];
	char print_string[100];
	*/

	nxt_motor_set_count(NXT_PORT_C, 0); // motor encoder reset

	
	//tyal ���ؗp�̖������[�v
	//���[�v�̊O�Ő錾�����l���^�C���̉�]�ɍ��킹�Đ��䂵�Ă���
   while(1){
 
   	do_tyreal();
   }
  

	
	while(1)
	{
		tail_control(TAIL_ANGLE_STAND_UP); /* ���S��~�p�p�x�ɐ��� */
  
	
		if (remote_start() == 1)
		{
			break; /* �����[�g�X�^�[�g */
		}

		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
		{
			systick_wait_ms(10);
			break; /* �^�b�`�Z���T�������ꂽ */
		}

		systick_wait_ms(10); /* 10msec�E�F�C�g */
	}

	
	balance_init();						/* �|���U�q���䏉�� */
	nxt_motor_set_count(NXT_PORT_C, 0); /* �����[�^�G���R�[�_���Z�b�g */
	nxt_motor_set_count(NXT_PORT_B, 0); /* �E���[�^�G���R�[�_���Z�b�g */

	
	
	
//	int buf[100];
		U32 off=0;
		U32 len;
	U8 *buf;
	U32 recv_byte;
rx_buf[0]=5;
	buf[1]=10;
   int  i;
	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = '0'; /* ��M�o�b�t�@���N���A */
	}
	
	while(1) {
		recv_byte=/*ecrobot_read_bt_packet(buf,len);*/ecrobot_read_bt(rx_buf,off,len);
		if(recv_byte>0){
			
			ecrobot_sound_tone(440, 512, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(369, 256, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(880, 512, VOL);
				systick_wait_ms(100);
		}
		tail_control(TAIL_ANGLE_STAND_UP); // ���S��~�p�p�x�ɐ���
		nxt_motor_set_speed(NXT_PORT_C, (rx_buf[0]-'0')*10 /*(int)(rx_buf[0]-'0')*/, 1); // �����[�^PWM�o�̓Z�b�g(-100�`100)
		nxt_motor_set_speed(NXT_PORT_B,(rx_buf[1]-'0')*10/*(int)(rx_buf[1]-'0')*/, 1); // �E���[�^PWM�o�̓Z�b�g(-100�`100)
	//break;	
		
		
		
		
	}	
		while(1) {
		
		tail_control(TAIL_ANGLE_STAND_UP); // ���S��~�p�p�x�ɐ���
		nxt_motor_set_speed(NXT_PORT_C, buf[0], 1); // �����[�^PWM�o�̓Z�b�g(-100�`100)
		nxt_motor_set_speed(NXT_PORT_B, buf[0], 1); // �E���[�^PWM�o�̓Z�b�g(-100�`100)
	//break;	
		
		
		
	}	
	while(1)
	{
		tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */

		if (sonar_alert() == 1) /* ��Q�����m */
		{
			//forward = turn = 0; /* ��Q�������m�������~ */
		}
		else
		{
			forward = 40; /* �O�i���� */
			
			before_diff = now_diff;
			now_diff = (0.6*light_black+0.4*light_white) - ecrobot_get_light_sensor(NXT_PORT_S3);
			integral = integral + ((now_diff + before_diff)/2.0 * DELTA);
			
			turn = Kp*now_diff + Ki*integral + Kd*((now_diff - before_diff) / DELTA);
			
			if(turn > 100){
				turn = 100;
			}
			else if(turn < -100){
				turn = -100;
			}
			
		}
	
		
		
		// bluetooth send data
		adc1 = (S16)ecrobot_get_gyro_sensor(NXT_PORT_S1);
		adc2 =(S16)ecrobot_get_battery_voltage();
		
		logSend(data1,data2,adc1,adc2,adc3,adc4);
		
		
		
		
		
		
	
		/* �|���U�q����(forward = 0, turn = 0�ŐÎ~�o�����X) */
		balance_control(
			(float)forward,								 /* �O��i����(+:�O�i, -:��i) */
			(float)turn,								 /* ��񖽗�(+:�E���, -:�����) */
			(float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* �W���C���Z���T�l */
			(float)gyro_offset,							 /* �W���C���Z���T�I�t�Z�b�g�l */
			(float)nxt_motor_get_count(NXT_PORT_C),		 /* �����[�^��]�p�x[deg] */
			(float)nxt_motor_get_count(NXT_PORT_B),		 /* �E���[�^��]�p�x[deg] */
			(float)ecrobot_get_battery_voltage(),		 /* �o�b�e���d��[mV] */
			&pwm_L,										 /* �����[�^PWM�o�͒l */
			&pwm_R);									 /* �E���[�^PWM�o�͒l */
		nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* �����[�^PWM�o�̓Z�b�g(-100�`100) */
		nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* �E���[�^PWM�o�̓Z�b�g(-100�`100) */

		systick_wait_ms(4); /* 4msec�E�F�C�g */
	
		
	}



}

//*****************************************************************************
// �֐��� : sonar_alert
// �� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int sonar_alert(void)
{
	static unsigned int counter = 0;
	static int alert = 0;

	signed int distance;

	if (++counter == 40/4) /* ��40msec���ɏ�Q�����m  */
	{
		/* v
		 * �����g�Z���T�ɂ�鋗��������́A�����g�̌��������Ɉˑ����܂��B
		 * NXT�̏ꍇ�́A40msec�����x���o����̍ŒZ������ł��B
		 */
		distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
		if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
		{
			alert = 1; /* ��Q�������m */
		}
		else
		{
			alert = 0; /* ��Q������ */
		}
		counter = 0;
	}

	return alert;
}

//*****************************************************************************
// �֐��� : tail_control
// �� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
static void tail_control(signed int angle)
{
	float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*P_GAIN; /* ��ᐧ�� */
	/* PWM�o�͖O�a���� */
	if (pwm > PWM_ABS_MAX)
	{
		pwm = PWM_ABS_MAX;
	}
	else if (pwm < -PWM_ABS_MAX)
	{
		pwm = -PWM_ABS_MAX;
	}

	nxt_motor_set_speed(NXT_PORT_A, (signed char)pwm, 1);
}

//*****************************************************************************
// �֐��� : remote_start
// �� : ����
// �Ԃ�l : 1(�X�^�[�g)/0(�ҋ@)
// �T�v : Bluetooth�ʐM�ɂ�郊���[�g�X�^�[�g�B Tera Term�Ȃǂ̃^�[�~�i���\�t�g����A
//       ASCII�R�[�h��1�𑗐M����ƁA�����[�g�X�^�[�g����B
//*****************************************************************************
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

void calibration(){

}
	
	


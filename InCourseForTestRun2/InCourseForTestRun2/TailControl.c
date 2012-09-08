#include "TailControl.h"

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
	if(ecrobot_get_motor_rev(NXT_PORT_A) < t_value)
	{
		t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
		if(t_count >= t_count_limit)	//�J�E���^�ŐK���������鑬�x�𒲐�
		{
			t_angle+=t_up;			//�p�x���グ��
			t_count = 0;
		}
	}

	else if(ecrobot_get_motor_rev(NXT_PORT_A) > t_value)
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
	
	
	
	switch(tail_mode){
		case(RN_TAILDOWN):				//�K����������
			if(ecrobot_get_motor_rev(NXT_PORT_A) < t_value)		//���݂̊p�x���ڕW�l�ȉ����ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//�J�E���^�ŐK���������鑬�x�𒲐�
				{
					t_angle += t_up;			//�p�x���グ��
					t_count = 0;
				}
			}
			else// if(ecrobot_get_motor_rev(NXT_PORT_A) == t_value)
			{
//				t_angle = t_value;
				t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
			}
			
			break;

		case(RN_TAILUP):										//�K�����グ��
			if(ecrobot_get_motor_rev(NXT_PORT_A) > t_value)		//���݂̊p�x���ڕW�l�ȏォ�ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)					//�J�E���^�ŐK�����グ�鑬�x�𒲐�
				{
					t_angle -= t_up;							//�p�x��������
					t_count = 0;
				}
			}
			else
			{
//				t_angle = t_value;
				t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
			}
			
			break;

		default:
			break;
			
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
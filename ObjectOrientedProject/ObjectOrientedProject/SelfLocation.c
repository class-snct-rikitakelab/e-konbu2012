#include "SelfLocation.h"


void SelfLocation_init(SelfLocation * this_SelfLocation){
	this_SelfLocation ->distance = 0;
	this_SelfLocation ->direction = 0;
	this_SelfLocation ->xCoo = 0;
	this_SelfLocation ->yCoo = 0;
}

//���Ȉʒu����֐�
void SelfLocation_SelfLocate(SelfLocation * this_SelfLocation)
{
	/* ���Ȉʒu����p�ϐ� */
	float d_theta_r;			//1�X�e�b�v������̉E�ԗ։�]�p�x
	float d_theta_l;			//1�X�e�b�v������̍��ԗ։�]�p�x

	static float d_theta_r_t = 0;		//�O��̃X�e�b�v�̉E�ԗ։�]�p�x
	static float d_theta_l_t = 0;		//�O��̃X�e�b�v�̍��ԗ։�]�p�x

	static double omega_r;			//�E�ԗւ̉�]�p���x
	static double omega_l;			//���ԗւ̉�]�p���x

	float v_r;				//�E�ԗւ̉�]���x
	float v_l;				//���ԗւ̉�]���x

	float v;					//���s�̂̑��s���x
	float omega;				//���s�̂̉�]�p���x

//	static float x_r = 0;		//�ԑ̂�X���W
//	static float y_r = 0;		//�ԑ̂�Y���W
//	static float theta_R = 0;	//�ԑ̂̊p�x

	static float x_r_zero = 0;	//X���W�����l
	static float y_r_zero = 0;	//Y���W�����l
	static float theta_R_zero = 0;	//�ԑ̊p�x�����l

	d_theta_l = (float)nxt_motor_get_count(NXT_PORT_C) * M_PI / 180.0;
	d_theta_r = (float)nxt_motor_get_count(NXT_PORT_B) * M_PI / 180.0;

	omega_l = (d_theta_l - d_theta_l_t)/0.004;
	omega_r = (d_theta_r - d_theta_r_t)/0.004;

	v_l = (WHEEL_R * 0.1) * omega_l;
	v_r = (WHEEL_R * 0.1) * omega_r;

	v = (v_r + v_l) / 2.0;
	omega = (v_r - v_l) / (MACHINE_W * 0.1);

	d_theta_l_t = d_theta_l;
	d_theta_r_t = d_theta_r;

	this_SelfLocation -> direction += omega * 0.004 + theta_R_zero;

	this_SelfLocation -> xCoo += v * cos(this_SelfLocation -> direction) * 0.004 + x_r_zero;

	this_SelfLocation -> yCoo += v * sin(this_SelfLocation -> direction) * 0.004 + y_r_zero;

	this_SelfLocation -> distance = fabs(CIRCUMFERENCE/360.0 * ((nxt_motor_get_count(NXT_PORT_C) + nxt_motor_get_count(NXT_PORT_B))/2.0));
	
}

float SelfLocation_GetDistance(SelfLocation * this_SelfLocation)
{
	return this_SelfLocation->distance;
}

float SelfLocation_GetXCoo(SelfLocation * this_SelfLocation)
{
	return this_SelfLocation->xCoo;
}

float SelfLocation_GetYCoo(SelfLocation * this_SelfLocation)
{
	return this_SelfLocation->yCoo;
}

float SelfLocation_GeyDirection(SelfLocation * this_SelfLocation)
{
	return this_SelfLocation->direction;
}

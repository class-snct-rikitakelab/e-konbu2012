#include "SelfLocation.h"


void SelfLocation_init(SelfLocation * this_SelfLocation){
	this_SelfLocation ->distance = 0;
	this_SelfLocation ->direction = 0;
	this_SelfLocation ->xCoo = 0;
	this_SelfLocation ->yCoo = 0;
}

//自己位置同定関数
void SelfLocation_SelfLocate(SelfLocation * this_SelfLocation)
{
	/* 自己位置同定用変数 */
	float d_theta_r;			//1ステップ当たりの右車輪回転角度
	float d_theta_l;			//1ステップ当たりの左車輪回転角度

	static float d_theta_r_t = 0;		//前回のステップの右車輪回転角度
	static float d_theta_l_t = 0;		//前回のステップの左車輪回転角度

	static double omega_r;			//右車輪の回転角速度
	static double omega_l;			//左車輪の回転角速度

	float v_r;				//右車輪の回転速度
	float v_l;				//左車輪の回転速度

	float v;					//走行体の走行速度
	float omega;				//走行体の回転角速度

//	static float x_r = 0;		//車体のX座標
//	static float y_r = 0;		//車体のY座標
//	static float theta_R = 0;	//車体の角度

	static float x_r_zero = 0;	//X座標初期値
	static float y_r_zero = 0;	//Y座標初期値
	static float theta_R_zero = 0;	//車体角度初期値

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

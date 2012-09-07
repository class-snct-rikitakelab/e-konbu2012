#include "SelfLocation.h"

//é©å»à íuìØíËä÷êî
void self_location()
{
	d_theta_l = (float)nxt_motor_get_count(NXT_PORT_C) * PI / 180.0;
	d_theta_r = (float)nxt_motor_get_count(NXT_PORT_B) * PI / 180.0;

	omega_l = (d_theta_l - d_theta_l_t)/0.004;
	omega_r = (d_theta_r - d_theta_r_t)/0.004;

	v_l = (WHEEL_R * 0.1) * omega_l;
	v_r = (WHEEL_R * 0.1) * omega_r;

	v = (v_r + v_l) / 2.0;
	omega = (v_r - v_l) / (MACHINE_W * 0.1);

	d_theta_l_t = d_theta_l;
	d_theta_r_t = d_theta_r;

	theta_R += omega * 0.004 + theta_R_zero;
	x_r += v * cos(theta_R) * 0.004 + x_r_zero;
	y_r += v * sin(theta_R) * 0.004 + y_r_zero;
	
}
#ifndef __Kaidan
#define	__Kaidan

#include "config.h"

extern float RA_linetrace_PID(int forward_speed);
extern int RA_speed(int forward_speed);


//�i�����m�֘A�}�N���A�v���g�^�C�v
 int RN_rapid_speed_up_signal_recevie(void);

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�

void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);

//void RA_linetrace_PID(int forward_speed);
//int RA_speed(int forward_speed);


int shock(int target);
void tailcontrol();
void RA_linetrace_P(int forward_speed);
//int RA_wheels(int turn);

void RN_modesetting();
/*static*/ int remote_start(void);
int rapid_speed_up(int target_gyro);
void self_location(void);
void battery_average_check(void);



#endif

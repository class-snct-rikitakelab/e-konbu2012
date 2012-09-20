#ifndef _REMOTESTART_H_
#define _REMOTESTART_H_

#include "ecrobot_interface.h"
#include "kernel_id.h"
#include "kernel.h"

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)
#define rapid_SPEED_UP_SIGNAL '3'


extern int remote_start(void);

extern int RN_rapid_speed_up_signal_recevie(void);
#endif

#ifndef _SONAR_H_
#define _SONAR_H_

#include "ecrobot_interface.h"

//超音波センサ用フラグ
static int sonarflag;

static int sonarvalue;

extern void getSonarValue(void);
extern int sonarCheck(int target_sonar);

#endif

#ifndef __Common
#define __Common

#include "ecrobot_interface.h"
#include "balancer.h"

#ifndef BOOL /* BOOL ‚ª’è‹`‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç */
#define BOOL int
#endif

#ifndef true /* TRUE ‚ª’è‹`‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç */
#define true 1
#endif

#ifndef false           /* FALSE ‚ª’è‹`‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç */
#define false 0
#endif

/*
typedef struct {
float forward_val;
float turn_val;
} ControlVals;
*/

typedef struct {
S8 pwmL;
S8 pwmR;
} PWMValues;

//for test code??
typedef struct {

}CtrlParm;


#endif




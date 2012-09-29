#ifndef __LineEdge
#define __LineEdge 

#include "ecrobot_interface.h"
#include "../NXTHardWare/LightSensor.h"
#include "../Common/Common.h"

typedef enum{
ON_BLACK,
ON_WHITE,
BALCK_TO_WHITE,
WHITE_TO_BLACK,
}LineEdgeState;

typedef struct {
LineEdgeState lineEdgeState;
float whiteVal;
float blackVal;
}LineEdge;


extern void LineEdge_init(LineEdge *this_LineEdge);
extern BOOL  LineEdge_getLineEdgeDetectPulse(LineEdge *this_LineEdge);

extern BOOL LineEdge_detectLineEdge(LineEdge *this_LineEdge);
void LineEdge_setLineEdgeState(LineEdge *this_LineEdge,LineEdgeState state);
LineEdgeState LineEdge_getLineEdgeState(LineEdge *this_LineEdge);

BOOL LineEdge_detectWhite(LineEdge *this_LineEdge);
BOOL LineEdge_detectBlack(LineEdge *this_LineEdge);


#endif

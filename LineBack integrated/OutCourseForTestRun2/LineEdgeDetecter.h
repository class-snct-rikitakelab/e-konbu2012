#ifndef __LineEdgeDetecter
#define __LineEdgeDetecter 

#include "ecrobot_interface.h"
#include "Calibration.h"

#define LINE_THRESHOLD 50
typedef enum{
ON_BLACK,
ON_WHITE,
BALCK_TO_WHITE,
WHITE_TO_BLACK,
}LineEdgeDetecterState;

typedef struct {
LineEdgeDetecterState lineEdgeDetecterState;
}LineEdgeDetecter;


extern void LineEdgeDetecter_init(LineEdgeDetecter *this_LineEdgeDetecter);
extern int  LineEdgeDetecter_getLineEdgeDetectPulse(LineEdgeDetecter *this_LineEdgeDetecter);

extern int LineEdgeDetecter_detectLineEdge(LineEdgeDetecter *this_LineEdgeDetecter);

void LineEdgeDetecter_setLineEdgeDetecterState(LineEdgeDetecter *this_LineEdgeDetecter,LineEdgeDetecterState state);
LineEdgeDetecterState LineEdgeDetecter_getLineEdgeDetecterState(LineEdgeDetecter *this_LineEdgeDetecter);

int LineEdgeDetecter_detectWhite(LineEdgeDetecter *this_LineEdgeDetecter);
int LineEdgeDetecter_detectBlack(LineEdgeDetecter *this_LineEdgeDetecter);


#endif

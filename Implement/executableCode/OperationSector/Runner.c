#include "Runner.h"


void Runner_StartRunning(Runner * this){
	runningState = RUN;
}

void Runner_finishRunning(Runner *this){
	runningState = FIN;
	
}

void Runner_init(Runner *this, int runningState);
void Runner_finish(Runner *this);

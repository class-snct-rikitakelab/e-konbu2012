
#include "DetectionSector/WirelessReceiver.h"
#include "OperationSector/DriveOperetor.h"

typedef struct {
	/**
	 * ���s���
	 */
	int runningState;
	
} Runner
/**
 * ���s�i���j
 */



	/**
	 * ���s�J�n
	 */
	void Runner_StartRunning(Runner * this);

	/**
	 * ���s�I��
	 */
	void Runner_finishRunning(Runner *this);

void Runner_init(Runner *this, int runningState);
void Runner_finish(Runner *this);





#include "DetectionSector/WirelessReceiver.h"
#include "OperationSector/DriveOperetor.h"

typedef struct {
	/**
	 * 走行状態
	 */
	int runningState;
	
} Runner
/**
 * 走行（仮）
 */



	/**
	 * 走行開始
	 */
	void Runner_StartRunning(Runner * this);

	/**
	 * 走行終了
	 */
	void Runner_finishRunning(Runner *this);

void Runner_init(Runner *this, int runningState);
void Runner_finish(Runner *this);





#include "DetectionSector/WirelessReceiver.h"
#include "OperationSector/DriveOperetor.h"

namespace OperationSector
{
/**
 * ���s�i���j
 */
class Runner
{
private:
	/**
	 * ���s���
	 */
	int runningState;

	DriveOperetor driveOperetor;
	DetectionSector::WirelessReceiver wirelessReceiver;

public:
	/**
	 * ���s�J�n
	 */
	void StartRunning();

	/**
	 * ���s�I��
	 */
	void finishRunning();

};

}  // namespace OperationSector

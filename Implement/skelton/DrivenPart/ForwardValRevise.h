
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * �O�i�ʐ����
 */
class ForwardValRevise
{
private:
	/**
	 * �ڕW�O�i��
	 */
	int targForwardVal;


public:
	/**
	 * �O�i�ʂ�␳����
	 */
	S8 ForwardValRevise_riveseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * �ڕW�O�i�ʂ�ݒ肷��
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

};

}  // namespace DrivenPart

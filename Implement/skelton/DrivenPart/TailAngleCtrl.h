
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DrivenPart
{
/**
 * �K���p�x�����
 */
class TailAngleCtrl
{
public:
	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	/**
	 * �ڕW�p�x��ݒ肷��
	 */
	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int agnle);

	/**
	 * �p�x�ڕW��������s����
	 */
	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

};

}  // namespace DrivenPart

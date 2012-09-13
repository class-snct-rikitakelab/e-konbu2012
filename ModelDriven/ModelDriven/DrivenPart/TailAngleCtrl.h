#ifndef __TailAngleCtrl
#define __TailAngleCtrl

/**
 * K”öŠp“x§ŒäŠí
 */
typedef struct  
{

}TailAngleCtrl;

	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	/**
	 * –Ú•WŠp“x‚ğİ’è‚·‚é
	 */
	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int agnle);

	/**
	 * Šp“x–Ú•W§Œä‚ğÀs‚·‚é
	 */
	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

#endif


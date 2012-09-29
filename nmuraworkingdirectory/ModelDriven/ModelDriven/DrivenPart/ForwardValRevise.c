
#include "ForwardValRevise.h"
#include "../Common/Factory.h"

/*
 *	ForwardValRevise.c
 *	�O�i�ʐ����N���X
 */

//�ڕW���x�擾���\�b�h
S32 ForwardValRevise_reviseForwardVal(ForwardValRevise *this_ForwardValRevise)
{	
	static int counter = 0;
	counter++;


	if(counter >= SPEED_COUNT)
	{
		if(this_ForwardValRevise->targForwardVal - this_ForwardValRevise->ForwardVal >= 0){
			this_ForwardValRevise->ForwardVal++;

			if(this_ForwardValRevise->ForwardVal > this_ForwardValRevise->targForwardVal)
				this_ForwardValRevise->ForwardVal = this_ForwardValRevise->targForwardVal;
		}
		else{
			this_ForwardValRevise->ForwardVal--;

			if(this_ForwardValRevise->ForwardVal < this_ForwardValRevise->targForwardVal)
				this_ForwardValRevise->ForwardVal = this_ForwardValRevise->targForwardVal;
		}
		counter = 0;
	}
	return this_ForwardValRevise->ForwardVal;
}

//�ڕW���x�ݒ胁�\�b�h
void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal)
{
	this_ForwardValRevise->targForwardVal = ForwardVal;
}

//���������\�b�h
void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise)
{
	this_ForwardValRevise->targForwardVal=0;
}

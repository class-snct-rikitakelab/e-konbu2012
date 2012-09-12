
#include "ForwardValRevise.h"

S8 ForwardValRevise_riveseForwardVal(ForwardValRevise *this_ForwardValRevise)
{	
	return this_ForwardValRevise->targForwardVal;

}

void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal)
{
	this_ForwardValRevise->targForwardVal = ForwardVal;
}

void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise)
{
	this_ForwardValRevise->targForwardVal=0;
}

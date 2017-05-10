// 
// 
// 

#include "inputs.h"

void InputsClass::init()
{
	_lastState = 0;
			

}

int InputsClass::LastAddNew(int newState)
{
	int returnVal = _lastState + newState;
	return returnVal;
}


InputsClass Inputs;


// inputs.h

#ifndef _INPUTS_h
#define _INPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include"OnewireKeypad.h"

class InputsClass
{
 protected:


 public:
	void init();
	int LastAddNew(int newState);
private:
	static int _lastState;
};

extern InputsClass Inputs;

#endif


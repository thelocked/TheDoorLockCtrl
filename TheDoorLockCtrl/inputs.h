// inputs.h

#ifndef _INPUTS_h
#define _INPUTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#if _VM_DEBUG
#pragma GCC optimize ("O0")
#endif

long resetSessionTime = 0;

int lastKPState = WAITING;
long releaseDetected = 0;

//bool checkInputSessionRestartTimer();

char checkSerialInput();

char checkKeypadInput();

int userPincommitCheck();

void resetInputSession();


//class InputsClass
//{
// protected:
//
//
// public:
//	void init();
//	int LastAddNew(int newState);
//private:
//	static int _lastState;
//};
//
//extern InputsClass Inputs;

#endif


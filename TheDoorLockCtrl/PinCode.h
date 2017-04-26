// PinCode.h

#ifndef _PINCODE_h
#define _PINCODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SUCCSESS 1
#define FAIL 0
#define IGNORED 2 

class PinCode
{

 public:
	 PinCode(char* pincode);
	 bool addInput(char addInput);
	 void reset();
	 int checkPin();



	 
private:
	String userInput;
	String correctPin;
};


#endif


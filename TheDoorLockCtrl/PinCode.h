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
#define TO_MANY_FAIL -1
#define IGNORED_ATTEMPT 2
/*
Used for user input, verification against the correct pin,
counting number of incorrect inputs and if to many return that.
User input starts session timer and if nothing is registered after 
that delay all of the current inputs are reseted recorded values and
num of retries
*/
class PinCode
{

 public:
	 PinCode(char* pincode, //The Correct pin
		 int numOfRetrties,
		 long inputResetDelay);

	 String addInput(char addInput);
	 int checkPin();

	 void resetAddedInput();
	 void resetAll();
	 

	 
private:
	String userInput;
	String correctPin;

	long inputSessionStart;



};


#endif


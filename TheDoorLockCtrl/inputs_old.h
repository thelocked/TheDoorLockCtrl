// inputs_old.h

#ifndef _INPUTS_OLD_h
#define _INPUTS_OLD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OnewireKeypad.h"


int lastKPState = WAITING;
long releaseDetected = 0;

//Declare input keypad object
//OnewireKeypad <Print, 12> KeyPadInput;


char checkKeypadInput()
{
	//Begin Keypad Checked for input states and read input.
	char inputFound = 0;
	int kpState = inputKeypad.Key_State();


	////Checks if keypad state has changed or not compared to last detected state
	////Only if new state is detected read from keypad
	if (kpState = RELEASED) //keyPadState = NO_KEY
	{
		//Detected No keys on keypad are pressed.
		//To help ensure correct user input so must keypad have been released and no input
		//detect for period from keypad
		if (releaseDetected < 0)
		{
			releaseDetected = millis();
		}
		//set timer for how long no keys are pressed
	}
	else if (kpState = PRESSED)
	{

		//Check if keypad has been released period is enough for read new key press
		if (releaseDetected + pauseForNextKeyPress < millis())
		{
			//keypad released period is enough for reading new key press
			inputFound =  inputKeypad.Getkey();
			//Disable relesedCheck Timer
			releaseDetected = 0;


		}


	}
		//Update last keypad state with New keypad state 
		lastKPState = kpState;
	/*End of keypad check*/
	return inputFound;
}
#endif


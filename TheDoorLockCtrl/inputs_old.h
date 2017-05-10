// inputs_old.h

#ifndef _INPUTS_OLD_h
#define _INPUTS_OLD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OnewireKeypad.h"


static int LastKeyPadstate;
static long releaseDetected;

bool CheckingTest(bool b)
{
	return b;
}



char CheckingKeypadInput(int kpState,char kpPressedKey)
{
	static long relTime;
	

	char userInputFound = 0;
	///*Begin Keypad Checked for input states and read input.*/
	
	////Checks if keypad state has changed or not compared to last detected state
	////Only if new state is detected read from keypad
	{
		if (kpState = RELEASED) //keyPadState = NO_KEY
		{
			//Detected No keys on keypad are pressed.
			//To help ensure correct user input so must keypad have been released and no input
			//detect for period from keypad
			//set timer for how long no keys are pressed
			releaseDetected = millis();
		}
		else if (kpState = PRESSED)
		{
		
			//Check if keypad has been released period is enough for read new key press
			if (releaseDetected + pauseForNextKeyPress < millis())
			{
				//keypad released period is enough for reading new key press
				userInput = KeyPad.Getkey();
				Serial << "Pressed Key: " << userInput << "\n";

			}
	
		}
		//Update last keypad state with New keypad state 
		lastKpState = kpState;
	//}


	/*End of keypad check*/
	return userInputFound;
}
#endif


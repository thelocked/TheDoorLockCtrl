#include "inputs.h"

#if _VM_DEBUG
#pragma GCC optimize ("O0")
#endif

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
			inputFound = inputKeypad.Getkey();
			//Disable relesedCheck Timer
			releaseDetected = 0;
		}
	}
	//Update last keypad state with New keypad state 
	lastKPState = kpState;
	/*End of keypad check*/
	return inputFound;
}

//bool checkInputSessionRestartTimer()
//{
//	if (inputSessionResetTime < millis())
//	{
//		//Last Session Time is passed and system will reset all input.
//		//lockAccessPin.resetAddedInput();//Resets added input in pin sequence from last session
//		//userIncorrectCount = 0;//Resets counter for previus faled input attemps
//		//inputSessionResetTime = userInputResetDelay + millis();//Set start time for new session
//		//													   //Try to flush all reaming input from serial
//		//while (Serial.available() > 0)
//		//{
//		//	char c = Serial.read();
//		//}
//	}
//	else
//	{
//		return false;
//	}
//}

char checkSerialInput()
{
	char userInputFound = 0;
	
	char serialInput = Serial.read(); //Trying to read and store from serial input;
	//Check if input exists;
	if ((serialInput > 1) && (serialInput < 255))
	{
		
		//Run through a bunch of chars that if detected they should be silently ignored
		//and not turn up as provided user input
		switch (serialInput)
		{
		case 0:
		case 255:
		case '\n': //new line;
				   //this is just rushed through with no break statements. Because if anything is matched same result is provided
			serialInput = 0;
			break;
		default:
			break;
		}

		//userInput = Serial.read();//get one char from serial.
		//Check if value matches anything compared to chars provided by keypad
		//Note this is needed to filter out junk and bogus values from serial input
		if (serialInput > 0)  //Run this only when input is provided 
		{
			
			int ifFoundIndexNo = keyPadKEYS.indexOf(serialInput); //Search for chars that if found in keypad layout.
																  //if (!keyPadKEYS.indexOf(userInput))
			if (ifFoundIndexNo >= 0)
			{
				//userInput match with keypad is found
				userInputFound = serialInput; //Update user input with serial find,

			}
		}
		//If no serial input found return default val = 0
		return userInputFound;
	}


			
}
int userPincommitCheck()
{
/* 
	Serial << "user committed current input and pin code evaluation is performed\n";
	int pinCheckStatus = inputPincode.checkPin();

	if (pinCheckStatus == PIN_CORRECT)
	{

		activateAlarmLockOutput(DOOR_LOCK_PIN, doorAccessEnabledPeriod);
		inputSessionResetTime = 0;//This will trigger a new session and reset current input.
		Serial << "Correct pin has been provided and Door lock will be disabled\n";

	}
	else if (pinCheckStatus == PIN_TO_MANY_FAIL)
	{
		//User have entered incorrect pin code
		userIncorrectCount++; //Increment failed attempts counter
							  //Check if or how many incorrect attempts
		Serial << "Incorrect pin from user\n";
		if (userIncorrectCount >= failedAttemptsTriggerAlarm)
		{
			//User have provided to many incorrect pin code and alarm device will be enabled
			Serial << "To many incorrect pins provided\n";
			//digitalWrite(ALARM_PIN, HIGH);//Enables Alarm device by turning output high
			////statusLedEnable(true);//Turn status led on.
			//long currentTime = millis();

			//Ativate Alarm OUTPUT!!
			activateAlarmLockOutput(ALARM_PIN, alarmTriggeredPeriod);

			//resetTimeAlarmLock = currentTime + doorAccessEnabledPeriod;//Set reset time for output enable period
			inputSessionResetTime = 0;//This will trigger a new session and reset current input.
			Serial << "Alarm is activated\n";			//User has provided to many failed attempts



		}
		else
		{

			//The user have not reached max failed attempts
			Serial << "Number of incorrect attempts: " << userIncorrectCount << " of max " << failedAttemptsTriggerAlarm << "\n";

		}
	}
	else
	{
		//Returned val from pin check is IGNORE
		Serial << "This user commit is ignored\n";
	}
*/
}

bool inputSessionResetCheck()
{
	//Check timer if reset enable time is passed
	long t = millis();
	if ((resetSessionTime < t) || (resetSessionTime = 0))
	{
		//Set new reset trigger period
		resetSessionTime = t + userInputResetDelay;
		//
		inputPincode.resetAllInput();
		//Input states init
		//Set Keypad to default state
		releaseDetected = 0;
		lastKPState = WAITING;

		//If any Serial input remaining read input until input buffer empty
		//while (Serial.available() >= 0)
		//{
		//	//When data from serial byte by byte
		//	Serial.read();
		//}
		return true;
	}
	else
	{
		//No reset done
		return false;
	}


	

		//Last Session Time is passed and system will reset all input.
		//lockAccessPin.resetAddedInput();//Resets added input in pin sequence from last session
		//userIncorrectCount = 0;//Resets counter for previus faled input attemps
		//inputSessionResetTime = userInputResetDelay + millis();//Set start time for new session
		//													   //Try to flush all reaming input from serial
		//while (Serial.available() > 0)
		//{
		//	char c = Serial.read();
		//}
		//Restore Input initial values and states
		
		//Last Session Time is passed and system will reset all input.
		//lockAccessPin.resetAddedInput();//Resets added input in pin sequence from last session
		//userIncorrectCount = 0;//Resets counter for previus faled input attemps
		//inputSessionResetTime = userInputResetDelay + millis();//Set start time for new session
		//													   //Try to flush all reaming input from serial
		//while (Serial.available() > 0)
		//{
		//	char c = Serial.read();
		//}
		
	
}




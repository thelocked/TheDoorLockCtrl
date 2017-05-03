/*
 Name:		DorrAccessCtrlPrototype.ino

 Beskrivning:
	Applikationen ska se till att en dörr endast kan öppnas med hjälp av en kod.
	Dörren manuvres med hjälp av ett elektriskt elsutbläck som i strömlöst tillstånd
	blockerar möjligheten att öpnna dörren.
	Koden ska matas in med en sifffra i taget med viss fördöjning mellan inmatningarna som ska
	hjälpa tilla att filtrera bort rgistrering av inmatning vid situationer tex. att en knapp
	fastnar eller behålls nedtryckt, flera knappar trycks ned samtidigt eller snabba störningar
	av inläsningar på ingången.
	Men om korrekt kod anges så matas låsfunktionen med spänning och slutar blockera öppning
	av dörren under en period

	Användaren har misst antal försök att mata in korrekt kod annars utlöses alarm trigger,
	som återställs efter en viss tid.

	En påbörjad inmatning nollställs efter en period.

Hur systemet används:
	Användaren matar in en siffer kombination via manöverpanelen, en kort perid grön indikering
	bekräftarknapptrykningen läst in och bekräftar med "#". Om användaren vill ångra påbörjad
	inmating så återställs den med "*".
	
	Om korrekt kod matats in så sluta dörren att blockeras så anvöndaren kan öppna
	Grönt fast sken indikerar att dörran kan öppnas.
	
	Om en felaktig kod matats in och inte överkrider andtal tillåtna felaktiga inmatningar,
	så meddelas dett med röd indikering och användare får nytt försök.
	Men skulle antalet felaktiga försök iföjld överskrida det tillåtna antalet,
	så aktiveras alarm under in viss tid.

	Skulle en användare mälja att mata in men inte bekräfta en kod eller
	av sluta efter ett eller flera felaktiga inmatningar, denna inmatings session
	att återställas efter en viss tid. Detta för att förhindra att en annan avändare inte
	ska kunna aktivera alarmet pga. tidigare inmatningar. Detta gör dock att en användare inte 
	har hur lång tid på sig som helst att mata in koden.


*/



//#if defined(ARDUINO) && ARDUINO >= 100
//#include "arduino.h"
//#else
//#include "WProgram.h"
//#endif
 


#include "PinCode.h"
#include "OnewireKeypad.h" //For info: http://playground.arduino.cc/Code/OneWireKeyPad

//***Global Const and properties

#define PINCODE "1234"
#define USERINPUT_COMMIT_KEY '#'
#define USERINPUT_RESET_KEY '*'
#define failedAttemptsTriggerAlarm 3 //How many times the user may enter wrong pin bef alarm trigger.



#define userInputResetDelay 30000 //How long any input session from keypad will stored in millis
#define doorAccessEnabledPeriod 15000//How long the user is abled to open door millis.

#define alarmTriggeredPeriod 60000//How long will the alarm output stay triggered millis.

//How long the keypad has to be released between key presses. To not record stuck buttons or interference on input pin.
#define pauseForNextKeyPress 500 

//**Pin configuration on the arduino

#define KEYPAD_PIN A0 //Pin input used for reading input from the keyboard hardware.
#define STATUS_LED_PIN 13 //Pin output used for display feedback to user. For door accsess granted and i key is pressed.

#define DOOR_LOCK_PIN 7 //Pin output used to enable the lock to stop blocking door access.
#define ALARM_PIN 8 //Pin output for triggering alarm.


PinCode lockAccessPin = PinCode(PINCODE,failedAttemptsTriggerAlarm,userInputResetDelay); //Provide pincode for door access

//** Keypad hardware setup and layout Settings
#define keypadRows 4
#define keypadCols 3
//#define keypadRow_Res 4700 //for 5.0V
//#define keypadCol_Res 1000 //for 5.0V

#define keypadRow_Res 2400 //for 3.3V		
#define keypadCol_Res 680 //for 3.3V

#define Precision ExtremePrec 


char keyPadKeysLayout[] = {
	'1','2','3',
	'4','5','6',
	'7','8','9',
	'*','0','#'
};

String keyPadKEYS = keyPadKeysLayout; //Keypad layout converted to string for later use

String keyPadKeysAsString = keyPadKEYS;
//Object that gets input from keypad
OnewireKeypad <Print, 12> KeyPad(Serial, keyPadKeysLayout, keypadRows, keypadCols, KEYPAD_PIN, keypadRow_Res, keypadCol_Res, Precision);

long LastkeyPressedTime = 0; //last input from user if not reseted..
//long doorLockOpenTime = 0; //is access or when was the lock opened.
//long alarmTriggerTime = 0; //is or when was the alarm triggerd.
long lockAlarmResetTimer = 0;//Start time for lock or alarm output enabed.

byte lastKpState = WAITING;
char timeLastKeyPress = 0;
//char lastPinInput;
String pincodeInput = ""; //Stores input from keyPad by user.

long userInputreset = userInputResetDelay;
long inputSessionResetTime = 0; //After how long time all input and retries will be reseted.

byte userIncorrectCount = 0;
long keypadReleasedTimer = 0;




// the setup function runs once when you press reset or power the board
void setup() {
	
	Serial.begin(9600); //For debugging output.

	//for debugging without keypad only.
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	//Set the pin modes and start values
	pinMode(ALARM_PIN, OUTPUT);
	pinMode(DOOR_LOCK_PIN, OUTPUT);
	pinMode(STATUS_LED_PIN, OUTPUT);

	digitalWrite(ALARM_PIN, LOW); //No alarm triggers;
	digitalWrite(DOOR_LOCK_PIN, LOW); //Doorlock is closed;
	digitalWrite(STATUS_LED_PIN, LOW); //Led inditation off.


	//To prevent detection of false keypress on start up
	//KeyPad.Getkey();

	Serial.flush(); //Remove any crap

	Serial.println("End of setup.");
	
	//resetUserInput(); //Clear all and provide serial output instructions.

}

// the loop function runs over and over again until power down or reset
void loop() {
	#if _VM_DEBUG
	#pragma GCC optimize ("O0")
	#endif
	char userInput = 0; //Stores value read from userinput
	

	/*Check if any userinput is provided or input session should be reset
	NOTE: Does nothing to alarm or door outputs.*/
	if (checkUserSessionResetTimer())
	{
		//Reset timer has triggerd new user session
		Serial << "Keypad input session is Reset.\n";
		//Print instructions to serial window
		Serial << "\nEnable door access by enter correct pin code and confirm with <ENTER>\n";
	}

	/*Begin Keypad Checked for input states and read input.*/
	byte kpState = KeyPad.Key_State();

	//Checks if keypad state has changed or not compared to last detected state
	//Only if new state is detected read from keypad
	if (kpState != lastKpState)
	{
		if (kpState = RELEASED) //keyPadState = NO_KEY
		{
			//Detected No keys on keypad are pressed.
			//To help ensure correct user input so must keypad have been released and no input
			//detect for period from keypad
			//set timer for how long no keys are pressed
			keypadReleasedTimer = millis();
		}
		else if (kpState = PRESSED)
		{
		
			//Check if keypad has been released period is enough for read new key press
			if (LastkeyPressedTime + pauseForNextKeyPress < millis())
			{
				//keypad released period is enough for reading new key press
				userInput = KeyPad.Getkey();
				Serial << "Pressed Key: " << userInput << "\n";

			}
	
		}
		//Update last keypad state with New keypad state 
		lastKpState = kpState;
	}


	/*End of keypad check*/


	/*Check serial for user input
	This enables input from serial connection for test and debug
	Note this can be commented out or removed*/
	
	//while(Serial.read() != -1);  //clears data in the PC Serial Port
	
	char serialInput = Serial.read(); //Trying to read and store from serial input;
	if ((serialInput > 0) && (serialInput < 255))
	{
		//userInput = Serial.read();//get one char from serial.
		//Check if value matches anything compared to chars provided by keypad
		//Note this is needed to filter out junk and bogus values from serial input
		int ifFoundIndexNo = keyPadKEYS.indexOf(serialInput); //Search for chars that if found in keypad layout.
		//if (!keyPadKEYS.indexOf(userInput))
		if (ifFoundIndexNo < 0)
		{
			//No match found reset value back to nothing
			userInput = 0;
			Serial << "Serial input not matched and ignored\n";
		}
		else
		{
			//userInput match with kaypad is found
			userInput = serialInput; //Update user input with serial find,
		Serial << "Serial input found: " << userInput<< "\n";
		Serial.println(userInput, DEC);
		}


		
	

	}

	/*End of serial input check*/

	//Verify if any new user input have been provided

	if (userInput > 0)
	{
	    //New input found provided from user
		switch (userInput)
		{
		//User whants to reset current pin sequnce.Note Not the number of faild attemts
		case USERINPUT_RESET_KEY: lockAccessPin.resetAddedInput(); 
			//Output to serial com window
			Serial << "Current pin sequence Reset";
			break; 
		//User whants to commit current key sequence and it vill be verifed
		case USERINPUT_COMMIT_KEY: userInputCommit(); break;
		//Input provider by user is added to pincode sequence and result of it all is returned
		default: String allUserInput = lockAccessPin.addInput(userInput);
			//Prints to serial to indikate pin sequence lenght, 

			//Of course do we have to discuse it before printing it out
			//Note: Uneccesary functionality just implemented for "Show off :)"
			String allUserInputDisquised;
			for (byte i = 0; i < allUserInput.length(); i++)
			{
				//Not a great way to Fill with '*' for each char, but who cares
				allUserInputDisquised += "*";
			}

			Serial << "Current user pin: ";
			Serial.println(allUserInputDisquised);
		}

	}

	void checkLockAlarmResetTimer();
}





//Called when user sends commit command and evaluates pin code and
//provide response depending on result
void userInputCommit()
{
	Serial << "user committed current input and pin code evaluation is performed\n";
	int pinCheckStatus = lockAccessPin.checkPin();

	if (pinCheckStatus == SUCCSESS)
	{
		//User have provided correct pin code and lock will be disabled
		digitalWrite(DOOR_LOCK_PIN, HIGH);//Enables door accesss by disable lock by turning output high
		digitalWrite(ALARM_PIN, LOW);//Disable alarm if its enabled.NOTE User is able to deactivate alarm with correct pin.
		digitalWrite(STATUS_LED_PIN,HIGH);//Turn status led on when lock is disabled.
			lockAlarmResetTimer = millis() + doorAccessEnabledPeriod;//Set reset time for output enable period
			Serial << "Door lock is disabled/n";
	}
	else if (pinCheckStatus == FAIL)
	{
		//User have entered incorrect pin code
		//Check how many incorrect attempts
		if (userIncorrectCount < failedAttemptsTriggerAlarm)
		{
			//The user have not reached max failed attempts
			userIncorrectCount++; //Increment failed attemts counter
		}
		else
		{
			//User have provided to many incorrect pin code and alarm device will be enabled
			digitalWrite(ALARM_PIN, HIGH);//Enables Alarm device by turning output high
			//statusLedEnable(true);//Turn status led on.
			lockAlarmResetTimer = millis() + doorAccessEnabledPeriod;//Set reset time for output enable period
			inputSessionResetTime = 0;//This will trigger a new session and reset current input.
			Serial << "Alarm is activated/n";			//User has provided to many failed attempts
			

		}
	}


}




//Checks time if all user input and incorrect attempts will be reseted and
//makes system ready for another user.
bool checkUserSessionResetTimer()
{
	if (inputSessionResetTime < millis())
	{
		//Last Session Time is passed and system will reset all input.
		lockAccessPin.resetAll();//Resets all input from last session
		userIncorrectCount = 0;//Resets counter for previus faled input attemps
		inputSessionResetTime = userInputResetDelay + millis();//Set start time for new session

		return true;
	}
	else
	{
		//Current session is still valid and no reset
		return false;
	}
}

//Checks time if the current lock and alarm output pins should be disabled
//NOTE this is used for both alarm and lock pins
bool checkLockAlarmResetTimer()
{
	if (lockAlarmResetTimer < millis())
	{
		//Last Session Time is passed and system lock and alarm output pins.
		digitalWrite(DOOR_LOCK_PIN, LOW); //Disables door access
		digitalWrite(ALARM_PIN, LOW);//Disables alarm
		digitalWrite(STATUS_LED_PIN,LOW);//Disables status led
		return true;
	}
	else
	{
		//Current session is still valid and no reset
		return false;
	}
}


//Used for resetting current user input session
//void resetUserInput()
//{
//	LastkeyPressedTime = 0; //last input from user if not reseted..
//	doorLockOpenTime = 0; //is access or when was the lock opened.
//	alarmTriggerTime = 0; //is or when was the alarm trigged.
//
//
//	inputSessionResetTime = inputSessionResetTime + millis();
//
//	
//
//
//
//}

//Set output High/Low for status led pin
//void statusLedEnable(bool ledOn)
//{
//	if (statusLedEnable)
//	{
//		digitalWrite(STATUS_LED_PIN, HIGH); //Led on.
//
//	}
//	else
//	{
//		digitalWrite(STATUS_LED_PIN, LOW); //Led off.
//	}
//}
//void openDoorLock()
//{
//	Serial << "Door lock will be disabled";
//}
//
////User have provided an incorrect pin code.
//void incorrectPinCode()
//{
//	//Check if user have made to many retries
//
//}
//
//void beginAlarm()
//{
//	Serial << "Alarm is trigged!!";
//}
	/*switch (pinCheckStatus)
	{
	case SUCCSESS: openDoorLock(); break;
	default:
		break;
	}*/
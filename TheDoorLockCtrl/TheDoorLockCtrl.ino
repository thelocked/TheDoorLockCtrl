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

	#if _VM_DEBUG
	#pragma GCC optimize ("O0")
	#endif


//#if defined(ARDUINO) && ARDUINO >= 100
//#include "arduino.h"
//#else
//#include "WProgram.h"
//#endif



#include "PinCode.h"
#include "OnewireKeypad.h" //For info: http://playground.arduino.cc/Code/OneWireKeyPad

//***Global Const and properties

#define PINCODE "1234"


#define maxIncorrecInputs 3 //How many times the user may enter wrong pin bef alarm trigger.



#define userInputResetDelay 30000 //How long any input session from keypad will stored in millis
#define doorAccessEnabledPeriod 15000//How long the user is abled to open door millis.

#define alarmTriggeredPeriod 60000//How long will the alarm output stay triggered millis.

//How long the keypad has to be released between key presses. To not record stuck buttons or interference on input pin.
#define pauseForNextKeyPress 500 

//**Pin configuration on the arduino

#define KEYPAD_PIN A0 //Pin input used for reading input from the keyboard hardware.
#define LED_OK_PIN 5 //Pin output used for display feedback to user. For door accsess granted and i key is pressed.
#define LED_ERR_PIN 6 //Pin out put used to inform user of wrong pin entered, alarm is triggerd and input reseted.
#define DOOR_ACCESS_PIN 7 //Pin output used to enable the lock to stop blocking door access.
#define ALARM_PIN 8 //Pin output for triggering alarm.


PinCode lockAccessPin = PinCode(PINCODE,maxIncorrecInputs,userInputResetDelay); //Provide pincode for door access

//** Keypad hardware setup and layout Settings
#define keypadRows 4
#define keypadCols 3
//#define keypadRow_Res 4700 //for 5.0V
//#define keypadCol_Res 1000 //for 5.0V

#define keypadRow_Res 2400 //for 3.3V		
#define keypadCol_Res 680 //for 3.3V

#define Precision ExtremePrec 

char keyPadKEYS[] = {
	'1','2','3',
	'4','5','6',
	'7','8','9',
	'*','0','#'
};

//Object that gets input from keypad
OnewireKeypad <Print, 12> KeyPad(Serial, keyPadKEYS, keypadRows, keypadCols, KEYPAD_PIN, keypadRow_Res, keypadCol_Res, Precision);

long LastkeyPressedTime = 0; //last input from user if not reseted..
long doorLockOpenTime = 0; //is access or when was the lock opened.
long alarmTriggerTime = 0; //is or when was the alarm triggerd.

byte lastKpState = WAITING;
char timeLastKeyPress = 0;
//char lastPinInput;
String pincodeInput = ""; //Stores input from keyPad by user.

long userInputreset = userInputResetDelay;
long inputSessionResetTime = 0; //After how long time all input and retries will be reseted.


long keypadReleasedTimer = 0;




// the setup function runs once when you press reset or power the board
void setup() {
	
	Serial.begin(9600); //For debugging output.

	//for debugging without keypad only.
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	//Set the pin modes and start values
	pinMode(ALARM_PIN, OUTPUT);
	pinMode(LED_ERR_PIN, OUTPUT);
	pinMode(DOOR_ACCESS_PIN, OUTPUT);
	pinMode(LED_OK_PIN, OUTPUT);

	digitalWrite(ALARM_PIN, LOW); //No alarm triggers;
	digitalWrite(DOOR_ACCESS_PIN, LOW); //Doorlock is closed;
	digitalWrite(LED_OK_PIN, LOW); //Led inditation off.
	digitalWrite(LED_ERR_PIN, LOW); //Led off

	//To prevent detection of false keypress on start up
	//KeyPad.Getkey();


	Serial.println("End of setup.");
	
	//resetUserInput(); //Clear all and provide serial output instructions.

}

// the loop function runs over and over again until power down or reset
void loop() {
	char userInput = 0;
	

	//Check if any userinput is provided or input session should be reset
	//OBS Does nothing to alarm or door outputs.
	if (checkUserSessionResetTimer())
	{
		//Reset timer has triggerd new user session
		Serial << "Keypad input session is Reset.\n";
		//Print instructions to serial window
		Serial << "\nEnable door access by enter correct pin code and confirm with <ENTER>\n";
	}

	//Check keypad input states and read input.
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


	


	//Check serial for user input
	char serialInput;
	if (Serial.available() > 0)
	{
		userInput = Serial.read();//get one char from serial.
		Serial << "Serial input found: " << userInput<< "\n";
		Serial.println(userInput, DEC);
	

	}
		switch (serialRead)
			case '*': lockAccessPin.resetAddedInput(); break;
			case '\n': lockAccessPin.checkPin(); break;
			default: lockAccessPin.addInput(userInput);

	    //Check if and what ben provided from user
		switch (userInput)
		{
		case '*': lockAccessPin.resetAddedInput(); break;
		case '#': lockAccessPin.checkPin(); break;
		default: lockAccessPin.addInput(keyPressed);
		}
	//else if (KpState == HELD)
	//{
	//	Serial << "Key:" << KeyPad.Getkey() << " being held\n";
	//}
	//else if (KpState == RELEASED)
	//{
	//	Serial << "Key Released\n";
	//}
	//else if (KpState == NO_KEY)
	//{
	//	Serial << "NO_KEY\n";
	//}
	//else if (KpState == WAITING)
	//{
	//	Serial << "Waiting\n";
	//}
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
		openDoorLock();
	}
	else if (pinCheckStatus == FAIL)
	{
		//User have entered incorrect pin code
	}

	switch (pinCheckStatus)
	{
	case SUCCSESS: openDoorLock(); break;
	default:
		break;
	}

}

void openDoorLock()
{
	Serial << "Door lock will be disabled";
}

//User have provided an incorrect pin code.
void incorrectPinCode()
{
	//Check if user have made to many retries

}

void beginAlarm()
{
	Serial << "Alarm is trigged!!";
}



//Checks time if all user input and incorrect attempts will be reseted and
//makes system ready for another user.
bool checkUserSessionResetTimer()
{
	if (inputSessionResetTime < millis())
	{
		//Last Session Time is passed and system will reset all input.
		lockAccessPin.resetAll();
		inputSessionResetTime = userInputResetDelay + millis();

		return true;
	}
	else
	{
		//Current session is still valid and no reset
		return false;
	}
}


//Used for resetting current user input session
void resetUserInput()
{
	LastkeyPressedTime = 0; //last input from user if not reseted..
	doorLockOpenTime = 0; //is access or when was the lock opened.
	alarmTriggerTime = 0; //is or when was the alarm trigged.


	inputSessionResetTime = inputSessionResetTime + millis();

	



}
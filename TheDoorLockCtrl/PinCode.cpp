// 
// 
// 

#include "PinCode.h"



PinCode::PinCode(char * pincode,
	int numOfRetrties = 0, //Default unlimited retries
	long inputResetDelay = 0) //Default session never resets
{
	correctPin = pincode;
}

//Append char to user input sequence and returns current pin sequence
String PinCode::addInput(char addInput)
{

	//Ignore adding if user input is larger then pincode
	if (userInput.length() < correctPin.length())
	{
		//Current input is not larger
		userInput = userInput + addInput;
		//return userInput;
	}
	//User input not appended
	return userInput;
}

void PinCode::resetAddedInput()
{
	userInput = "";
}

void PinCode::resetAll()
{}



//Evaluates if the user input is equal to pincode and returns info
//if correct or incorrect action should be trigged or ignored
int PinCode::checkPin()
{
	int lengthOfUserInput = userInput.length();
	//Check if user input have correct length
	if (lengthOfUserInput == 0)
	{
		//User have not provided any input to evaluate.
		//return IGNORED;
	}
	else if (userInput==correctPin)
	{
		//User input is matched to pincode
		return SUCCSESS;
	}
	else
	{
		//No match is found
		return FAIL;
	}

	//else if	(lengthOfUserInput != correctPin.length())
	//{
	//	//User input and pinkode lenght is not matched
	//	return FAIL;
	//}
	//else
	//{
	//	//Start checking if user input and pincode is matched
	//	for (int i = 0; i < (lengthOfUserInput -); i++)
	//	{

	//	}

	//}
	int status = IGNORED_ATTEMPT;

	return IGNORED_ATTEMPT;
}







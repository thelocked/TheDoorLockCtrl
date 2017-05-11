// 
// 
// 

#include "PinCode.h"



PinCode::PinCode(char * pincode,int numOfRetrties = 0) //Default session never resets
{
	correctPin = pincode;
	maxFailnum = numOfRetrties;
}



//Append char to user input sequence and returns current pin sequence
String PinCode::addInput(char addInput)
{

	//Ignore adding if user input already have more chars than then correct pincode
	if (userInput.length() <= correctPin.length())
	{
		//Current input have less chars than correct pin
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



void PinCode::resetAllInput()
{
	userInput = "";
	countFailed = 0;
}





//Evaluates if the user input is equal to pincode and returns info
//if correct or incorrect action should be trigged or ignored
int PinCode::checkPin()
{
	if (userInput == correctPin)
	{
		//Input is matched with pin
		//resetAllInput();
		return PIN_CORRECT;
	}
	else
	{
		countFailed = (countFailed+1);
		//Check failed Attempts counter
		if (countFailed < maxFailnum)
		{
			//Returns num of retries left for correctPin
			return(maxFailnum - countFailed);
		}
		else
		{
			//Last retry FAILED
			return(PIN_TO_MANY_FAIL);
		}
	}
	

	//int lengthOfUserInput = userInput.length();
	////Check if user input have correct length
	//if (lengthOfUserInput == 0)
	//{
	//	//User have not provided any input to evaluate.
	//	return;
	//}

	//return PIN_TO_MANY_FAIL;


	//else if (lengthOfUserInput == correctPin.length())
	//{
	//	//User input is matched by lenght

	//	//Now verfy match both pincodes char by char
	//	//Note: i have totally ignored investigating all possibillites to do this in a more elegant way
	//	//But this works correct!
	//	for (byte charIndexPins = 0; charIndexPins < lengthOfUserInput; charIndexPins++)
	//	{
	//		if (userInput[charIndexPins] != correctPin[charIndexPins])
	//		{
	//			//Matching of pincodes failed
	//			return FAIL; //Exit and return fail.
	//		}
	//	}
	//	//Every thing matches correctly
	//	return SUCCSESS;
	//}
	//else
	//{
	//	//Length of pincodes does not match
	//	return FAIL;
	//}


	

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

}







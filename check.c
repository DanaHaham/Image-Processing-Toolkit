#include "check.h"

// Exit the program in case that malloc fail - there is no more place in the memory for the program
void mallocFail(void* pointer)
{
	if (pointer == NULL)
	{
		printf("Memory allocation failed!!");
		exit(1);
	}
}

// Exit the program in case that open file fail 
void openFileFail(FILE* fp)
{
	if (fp == NULL)
	{
		printf("Failed opening the file. Exiting!\n");
		exit(1);
	}
}

// Exit the program in case of an invalid magic number
void isValidMagicNumber(char givenMagicNumber, char requestedMagicNumber)
{
	if (givenMagicNumber != requestedMagicNumber)
	{
		printf("Invalid Magic Number P%c (P%c requested)", givenMagicNumber, requestedMagicNumber);
		exit(1);
	}
}
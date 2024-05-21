#ifndef CHECK_H
#define CHECK_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Functions 
void mallocFail(void* pointer);
void openFileFail(FILE* fp);
void isValidMagicNumber(char givenMagicNumber, char requestedMagicNumber);
#endif
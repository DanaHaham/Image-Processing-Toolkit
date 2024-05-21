#ifndef MENU_H
#define MENU_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "imageConvert.h"
#include "imageCompress.h"
#include "segment.h"
#include "check.h"

// Consts
#define INCREASE_PHS_SIZE 2
#define INIT_ARR_SIZE 1

#define NEW_LINE '\n'
#define END_STR '\0'

#define EXIT_PROGRAM 12

// Functions 
void menu();

COLOR_IMAGE* section1();
GRAY_IMAGE* section2(unsigned char* depth);
int section3(GRAY_IMAGE* grayImg, IMG_POS_LIST** pSegments);
GRAY_IMAGE* section4(IMG_POS_LIST* segments, int sizeSegments);
void section5(GRAY_IMAGE* grayImg, unsigned char depth);
void section6();
void section7();
void section8();
void section9();
void section10();
void section11();

char* scanName();
char* createString(unsigned short* pLogSize, unsigned short* pPhsSize);
char* updateSizeString(char* name, unsigned short logSize, unsigned short* pPhsSize);
#endif
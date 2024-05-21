#ifndef IMAGE_CONVERT_H
#define IMAGE_CONVERT_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "image.h"

// Consts
#define RGB_NUM 3

#define WHITE 1
#define BLACK 0

#define BW_DEPTH 1

//Functions
void convertPPMToPGM(char* fname);
void convertPGMToBW(char* fname);

void convertPPMToPGM_Bin(char* fname);
void convertPGMToBW_Bin(char* fname);

void convertColorImageToPGM(char* orgFname, COLOR_IMAGE* colorImg, char magicNumber);
void convertGrayImageToBw(char* orgFname, GRAY_IMAGE* grayImg, int k, char magicNumber);

unsigned char colorToGreyPixel(RGB colorPixel);

char* createPGMNameFromPPM(char* fname);
char* createBWNameFromPGM(char* fname, int k);

#endif

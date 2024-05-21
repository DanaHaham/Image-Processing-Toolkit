#ifndef IMAGE_H
#define IMAGE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "check.h"

// Consts
#define MAX_DEPTH 255

#define MAGIC_SIZE 3
#define ID_MAGIC_INDEX 1

#define COMMENT '#'
#define NEW_LINE '\n'
#define CR '\r'
#define TAB '\t'
#define SPACE ' '
#define V_TAB '\v'
#define END_STR '\0'

#define P2 '2'
#define P5 '5'
#define P3 '3'
#define P6 '6'

// Structs
typedef struct rgb {
	unsigned char r, g, b;
}RGB;

typedef struct colorImage {
	unsigned short rows, cols;
	RGB** pixels;
}COLOR_IMAGE;

typedef struct grayImage {
	unsigned short rows, cols;
	unsigned char** pixels;
}GRAY_IMAGE;

// Functions
COLOR_IMAGE* readPPM(char* fname);
GRAY_IMAGE* readPGM(char* fname);

COLOR_IMAGE* readPPM_Bin(char* fname);
GRAY_IMAGE* readPGM_Bin(char* fname);

COLOR_IMAGE* createColorImage(unsigned short rows, unsigned short cols);
GRAY_IMAGE* createGrayImage(unsigned short rows, unsigned short cols);

void createPGMFile(char* fname, GRAY_IMAGE* grayImg, unsigned char depth, char magicNumber);

unsigned char getDeptFromFile(char* fname);
long int getHeader(char* fname, unsigned short* rows, unsigned short* cols, unsigned char* depth, char* magicNumberId);
long int printHeader(char* fname, unsigned short rows, unsigned short cols, unsigned char depth, char magicNumber);

void skipUntilData(FILE* fp);

void freeColorImage(COLOR_IMAGE* colorImg);
void freeGrayImage(GRAY_IMAGE* grayImg);
#endif
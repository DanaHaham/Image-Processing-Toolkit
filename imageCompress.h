#ifndef IMAGE_COMPRESS_H
#define IMAGE_COMPRESS_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "image.h"

// Consts
#define BITS_PER_BYTE 8
#define EMPTY_BYTE 0
#define BIN_BASE 2

// Structs
typedef unsigned char BYTE;

//Functions
void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel);
void convertCompressedImageToPGM(char* fname);

BYTE* compressGrayImagePixels(unsigned char* pixelArr, unsigned short sizePixel, int bitsPerPixel, unsigned short* pSizeRes);
unsigned char* extractGrayImagePixels(BYTE* pixelArr, unsigned short sizeRes, int bitsPerPixel);

void createCompressFile(char* fname, BYTE* compressedPixels, unsigned short sizeCompPixels, unsigned short rows, unsigned short cols, unsigned char maxGrayLevel);
BYTE* readCompreesImage(char* fname, unsigned short* rows, unsigned short* cols, unsigned char* maxGrayLevel, int* bitsPerPixel);

int getBitsPerPixel(unsigned char maxGray);
unsigned char* convertPixelsMaxGray(GRAY_IMAGE* image, unsigned char maxGrayLevel);
char* createPGMNameFromCompressedImge(char* fname);
#endif
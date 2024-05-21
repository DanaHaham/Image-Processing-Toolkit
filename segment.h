#ifndef SEGMENT_H
#define SEGMENT_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include "image.h"
#include "segmentTree.h"
#include "segmentList.h"

// Consts
#define NEIGHBORS 8

#define MIN_ROW 0
#define MIN_COL 0

// Structs
typedef struct segment
{
	TNODE* root;
}SEGMENT;

// Functions 
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold);
void findSingleSegmentHelper(TNODE* root, GRAY_IMAGE* img, unsigned char threshold, unsigned char startPos, bool** isPixelInSegment);

int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments);
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size);

IMG_POS_LIST convertSegmentToPosList(SEGMENT segment);
IMG_POS_LIST convertSegmentToPosListHelper(TNODE* root);

bool** createBoolMatrix(unsigned short rows, unsigned short cols);
bool isValidNeighbor(GRAY_IMAGE* img, unsigned char threshold, unsigned char startPos, bool** isPixelInSegment, IMG_POS position);
void findMaxColRow(IMG_POS_LIST* segments, unsigned int size, IMG_POS maxPos);

void freeSegment(SEGMENT segment);
void freeBoolMatrix(bool** boolMatrix, unsigned short rows);
#endif
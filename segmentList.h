#ifndef SEGMENT_LST_H
#define SEGMENT_LST_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "check.h"
#include "segmentTree.h"

//Structs
typedef struct img_pos_node {
	IMG_POS position;
	struct img_pos_node* next, * prev;
}IMG_POS_NODE;

typedef struct img_pos_list {
	IMG_POS_NODE* head;
	IMG_POS_NODE* tail;
}IMG_POS_LIST;

//Functions
IMG_POS_NODE* createNewImgPosNode(IMG_POS position, IMG_POS_NODE* prev, IMG_POS_NODE* next);

void insertDataToEndPosList(IMG_POS_LIST* lst, IMG_POS data);
void insertNodeToEndPosList(IMG_POS_LIST* lst, IMG_POS_NODE* tail);

void makeEmptyImgPosList(IMG_POS_LIST* lst);
bool isEmptyImgPosList(IMG_POS_LIST* lst);

void freeImgPosList(IMG_POS_LIST* lst);
void freePosListArr(IMG_POS_LIST* arrPosLst, int size);
#endif
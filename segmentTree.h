#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "check.h"

// Consts
#define ROW 0
#define COL 1

// Structs
typedef struct tnode TNODE;

typedef unsigned short IMG_POS[2];

typedef struct tnode_lnode {
	TNODE* node;
	struct tnode_lnode* next;
}TNODE_LNODE;

typedef struct tnode_list {
	TNODE_LNODE* head;
	TNODE_LNODE* tail;
}TNODE_LIST;

struct tnode {
	IMG_POS position;
	TNODE_LIST nextPossiblePositions;//הילדים רשימת
};

// Functions 
TNODE* createNewTNode(IMG_POS position);
TNODE_LNODE* createNewTNodeLNode(TNODE* node, TNODE_LNODE* next);

void insertTNodeLNodeToEndList(TNODE_LIST* lst, TNODE_LNODE* tail);
void insertDataToEndTNodeList(TNODE_LIST* lst, TNODE* node);

void makeEmptyTNodeList(TNODE_LIST* lst);
bool isEmptyTNodeList(TNODE_LIST* lst);

void freeTNode(TNODE* root);
#endif
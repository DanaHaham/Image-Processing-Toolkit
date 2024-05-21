#include "segmentList.h"

// Create new IMG_POS_NODE, update his values acordding to the given variables and retrun its pointer
// Dosent free the new IMG_POS_NODE! Must happend after use.
IMG_POS_NODE* createNewImgPosNode(IMG_POS position, IMG_POS_NODE* prev, IMG_POS_NODE* next)
{
	IMG_POS_NODE* res;

	// Create new IMG_POS_NODE
	res = (IMG_POS_NODE*)malloc(sizeof(IMG_POS_NODE));
	mallocFail(res);

	// Init the values of the IMG_POS_NODE
	res->position[ROW] = position[ROW];
	res->position[COL] = position[COL];

	res->prev = prev;
	res->next = next;

	return res;
}

// Create a new IMG_POS_NODE and insert it to the end of the given IMG_POS list
// Dosent free the new IMG_POS_NODE! Must happend after use.
// Change the given IMG_POS list
void insertDataToEndPosList(IMG_POS_LIST* lst, IMG_POS position)
{
	IMG_POS_NODE* newTail;

	newTail = createNewImgPosNode(position, NULL, NULL);
	insertNodeToEndPosList(lst, newTail);
}

// Add the given IMG_POS_NODE to the end of the given IMG_POS list
// Change the given IMG_POS list
void insertNodeToEndPosList(IMG_POS_LIST* lst, IMG_POS_NODE* tail)
{
	if (isEmptyImgPosList(lst))
	{
		lst->head = lst->tail = tail;
		tail->prev = NULL;
	}
	else
	{
		tail->prev = lst->tail;
		lst->tail->next = tail;
		lst->tail = tail;
	}

	tail->next = NULL;
}

// Init the the given IMG_POS list as an empty list
// Change the the given IMG_POS list
void makeEmptyImgPosList(IMG_POS_LIST* lst)
{
	lst->head = lst->tail = NULL;
}

// Check if the IMG_POS list is empty and return the answer
bool isEmptyImgPosList(IMG_POS_LIST* lst)
{
	return (lst->head == NULL);
}

// Free all the IMG_POS_NODEs in the list
// Change the IMG_POS list
void freeImgPosList(IMG_POS_LIST* lst)
{
	IMG_POS_NODE* currNode = lst->head, * nextNode;

	while (currNode != NULL)
	{
		nextNode = currNode->next;

		// Free the IMG_POS_NODE
		free(currNode);

		currNode = nextNode;
	}

	makeEmptyImgPosList(lst);
}

// Free the given dynamic IMG_POS_LIST array and the lists in it
// Assume that the given segments is large enough for the given size
void freePosListArr(IMG_POS_LIST* arrPosLst, int size)
{
	// Free all the lists in the array
	for (int i = 0; i < size; i++)
	{
		// Free IMG_POS_LIST list
		freeImgPosList(&arrPosLst[i]);
	}

	// Free dynamic array
	free(arrPosLst);
}


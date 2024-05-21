#include "segmentTree.h"

// Create new TNODE, update his values acordding to the given variables and retrun its pointer
// Dosent free the new TNODE! Must happend after use.
TNODE* createNewTNode(IMG_POS position)
{
	TNODE* res;

	// Create new TNode
	res = (TNODE*)malloc(sizeof(TNODE));
	mallocFail(res);

	// Init the values of the new TNode
	res->position[ROW] = position[ROW];
	res->position[COL] = position[COL];

	makeEmptyTNodeList(&res->nextPossiblePositions);

	return res;
}

// Create new TNODE_LNODE, update his values acordding to the given variables and retrun its pointer
// Dosent free the new TNODE_LNODE! Must happend after use.
TNODE_LNODE* createNewTNodeLNode(TNODE* node, TNODE_LNODE* next)
{
	TNODE_LNODE* res;

	// Create new TNode_LNode
	res = (TNODE_LNODE*)malloc(sizeof(TNODE_LNODE));
	mallocFail(res);

	// Init the values of the TNode_LNode
	res->node = node;
	res->next = next;

	return res;
}

// Create a new TNODE_LNODE and insert it to the end of the given TNode list
// Dosent free the new TNODE_LNODE! Must happend after use.
// Change the given TNode list
void insertDataToEndTNodeList(TNODE_LIST* lst, TNODE* node)
{
	TNODE_LNODE* newTail;

	newTail = createNewTNodeLNode(node, NULL);
	insertTNodeLNodeToEndList(lst, newTail);
}

// Add the given TNODE_LNODE to the end of the given TNode list
// Change the given TNODE_LNODE list
void insertTNodeLNodeToEndList(TNODE_LIST* lst, TNODE_LNODE* tail)
{
	if (isEmptyTNodeList(lst))
	{
		lst->head = lst->tail = tail;
	}
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}

	tail->next = NULL;
}

// Init the the given TNODE_LIST list as an empty TNode list
// The function change the the given TNODE_LNODE list
void makeEmptyTNodeList(TNODE_LIST* lst)
{
	lst->head = lst->tail = NULL;
}

// Check if the TNode list is empty and return the answer
bool isEmptyTNodeList(TNODE_LIST* lst)
{
	return (lst->head == NULL);
}

// Free the given TNODE and his children
void freeTNode(TNODE* root)
{
	if (root != NULL)
	{
		TNODE_LNODE* currTnodeLnode, * nextTnodeLnode;
		currTnodeLnode = root->nextPossiblePositions.head;

		// Free all the children of the root 
		while (currTnodeLnode != NULL)
		{
			nextTnodeLnode = currTnodeLnode->next;

			// Free the current TNODE child
			freeTNode(currTnodeLnode->node);

			// Free the TNODE_LNODE
			free(currTnodeLnode);

			currTnodeLnode = nextTnodeLnode;
		}

		// Free the root
		free(root);
	}
}


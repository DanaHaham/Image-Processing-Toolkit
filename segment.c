#include "segment.h"

// Create a new segment acordding to the given start position and threshold (and the definition of segment) and return it.
// Create new TNODEs for the segment and dosen't free them! Must happend after use. 
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold)
{
	SEGMENT res;
	bool** isPixelInSegment;
	unsigned char startPos;

	// Create flag matrix that is match to the pixel's table
	isPixelInSegment = createBoolMatrix(img->rows, img->cols);

	// Save the start pos
	startPos = img->pixels[start[ROW]][start[COL]];

	// Mark the current pixel as a pixel in segment
	isPixelInSegment[start[ROW]][start[COL]] = true;
	res.root = createNewTNode(start);

	// Init the root of the segment 
	findSingleSegmentHelper(res.root, img, threshold, startPos, isPixelInSegment);

	freeBoolMatrix(isPixelInSegment, img->rows);
	return res;
}

// Create a new TNODE acordding to the given start position and threshold (and the definition of segment) and return it.
// Dosen't free TNODE and the TNODE's children! Must happend after use. 
void findSingleSegmentHelper(TNODE* root, GRAY_IMAGE* img, unsigned char threshold, unsigned char startPos, bool** isPixelInSegment)
{
	IMG_POS neighborsPosArr[NEIGHBORS] = { {root->position[ROW] - 1, root->position[COL]}, // North
											{root->position[ROW] - 1, root->position[COL] + 1}, // North East
											{root->position[ROW], root->position[COL] + 1}, // Eeast
											{root->position[ROW] + 1, root->position[COL] + 1}, // South East
											{root->position[ROW] + 1, root->position[COL]}, // South
											{root->position[ROW] + 1, root->position[COL] - 1},  // South West
											{root->position[ROW], root->position[COL] - 1}, // West
											{root->position[ROW] - 1, root->position[COL] - 1}, // North West
	};

	TNODE_LNODE* currTNodeLNode;

	// Find and mark all the valid neighbor of the current pixel
	for (int i = 0; i < NEIGHBORS; i++)
	{
		// Check if the current neighbor is a child of the root
		if (isValidNeighbor(img, threshold, startPos, isPixelInSegment, neighborsPosArr[i]))
		{
			// Add current neighbor to the children's list of the root
			TNODE* childTNode;
				
			childTNode = createNewTNode(neighborsPosArr[i]);
			insertDataToEndTNodeList(&root->nextPossiblePositions, childTNode);

			// Mark the current neighbor as a pixel in segment
			isPixelInSegment[neighborsPosArr[i][ROW]][neighborsPosArr[i][COL]] = true;
		}
	}

	currTNodeLNode = root->nextPossiblePositions.head;

	// Update the children of every TNODE in the list of the root
	while (currTNodeLNode != NULL)
	{
		findSingleSegmentHelper(currTNodeLNode->node,img, threshold, startPos, isPixelInSegment);
		currTNodeLNode = currTNodeLNode->next;
	}
}

// Find all the segments in the given image, convert them into IMG_POS_LIST lists and return an array of lists (by reference) and its size.
// Create a dynamic array of IMG_POS_LIST lists and IMG_POS_NODEs for each list and dosen't free them! Must happend after use.
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments)
{
	IMG_POS_LIST* segmentsArr;
	bool** isPixelInSegment;
	unsigned short arrLogSize = 0;

	// Create flag matrix that is match to the pixel's table
	isPixelInSegment = createBoolMatrix(img->rows, img->cols);

	// Create a new dynamic array of IMG_POS_LIST with maximum size
	segmentsArr = (IMG_POS_LIST*)malloc(sizeof(IMG_POS_LIST) * img->rows * img->cols);
	mallocFail(segmentsArr);

	// Add all the pixels in the given image to segments
	for (int i = 0; i < img->rows; i++)
	{
		for (int j = 0; j < img->cols; j++)
		{
			SEGMENT newSegment;
			IMG_POS currPos;

			// The current pixel isn't in an exist segment
			if (!isPixelInSegment[i][j])
			{
				currPos[ROW] = i;
				currPos[COL] = j;

				// Mark the current pixel as a pixel in segment
				isPixelInSegment[currPos[ROW]][currPos[COL]] = true;
				
				newSegment.root = createNewTNode(currPos);

				// Create the segment of the current pixel
				 findSingleSegmentHelper(newSegment.root, img, threshold, img->pixels[i][j], isPixelInSegment);

				// Add the segment to the IMG_POS_LIST array
				segmentsArr[arrLogSize] = convertSegmentToPosList(newSegment);
				arrLogSize++;

				// Free the segment
				freeSegment(newSegment);
			}
		}
	}

	// Update the size of the IMG_POS_LIST array
	segmentsArr = (IMG_POS_LIST*)realloc(segmentsArr, sizeof(IMG_POS_LIST) * arrLogSize);
	mallocFail(segmentsArr);

	freeBoolMatrix(isPixelInSegment, img->rows);

	// Return the new segments array and it's size
	*segments = segmentsArr;
	return arrLogSize;
}

// Create a new GRAY_IMAGE, update it's pixles to be shades of gray acordding to the formula and the maching segments and return it's pointer.
// Dosent free the new GRAY_IMAGE and it's arrays! Must happend after use.
// Assume that the given segments is large enough for the given size.
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size)
{
	GRAY_IMAGE* newImage;
	IMG_POS maxPosition;

	// Find maximum row and col
	findMaxColRow(segments, size, maxPosition);

	// Create an empty image with the maximum rows and cols
	newImage = createGrayImage(maxPosition[ROW] + 1, maxPosition[COL] + 1);

	// Update the shade of the image's pixels acordding to the matching segments 
	for (unsigned int i = 0; i < size; i++)
	{
		IMG_POS_NODE* currPos;
		unsigned char segColor;

		// Current segment's list
		currPos = segments[i].head;
		segColor = (i * (MAX_DEPTH / (size - 1)));

		// Update the locations that are belonge to the current segments to the maching shade acordding to the formula 
		while (currPos != NULL)
		{
			newImage->pixels[currPos->position[ROW]][currPos->position[COL]] = segColor;
			currPos = currPos->next;
		}
	}

	return newImage;
}

// Convert the given segment into a IMG_POS_LIST array and return it.
// Create new IMG_POS_NODEs and dosent free them! Must happend after use. 
IMG_POS_LIST convertSegmentToPosList(SEGMENT segment)
{
	return convertSegmentToPosListHelper(segment.root);
}

// Convert the given TNODE into a IMG_POS_LIST array and return it.
// Create new IMG_POS_NODEs and dosent free them! Must happend after use. 
IMG_POS_LIST convertSegmentToPosListHelper(TNODE* root)
{
	IMG_POS_LIST res;

	// Init list
	makeEmptyImgPosList(&res);

	if (root != NULL)
	{
		// Add the current TNODE's value into the IMG_POS_LIST list 
		insertDataToEndPosList(&res, root->position);

		if (!isEmptyTNodeList(&root->nextPossiblePositions))
		{
			TNODE_LNODE* curr = root->nextPossiblePositions.head;

			// Add all the TNODE's children to the IMG_POS_LIST list
			while (curr != NULL)
			{
				IMG_POS_LIST tempList;

				tempList = convertSegmentToPosListHelper(curr->node);

				// Add the current child and its children to the IMG_POS_LIST list
				tempList.head->prev = res.tail;
				res.tail->next = tempList.head;
				res.tail = tempList.tail;

				curr = curr->next;
			}
		}
	}

	return res;
}

// Find the maximum position (max col and row existing) from all the array and return it (by refernce in the given maxPos)
void findMaxColRow(IMG_POS_LIST* segments, unsigned int size, IMG_POS maxPos)
{
	IMG_POS_NODE* curr;

	// Init the max position to be smallest possible
	maxPos[COL] = 0;
	maxPos[ROW] = 0;

	// Run the array
	for (unsigned int i = 0; i < size; i++)
	{
		// Current segments's list
		curr = segments[i].head;

		while (curr != NULL)
		{
			// Find the max position from all the lists (all the array)
			if (curr->position[ROW] >= maxPos[ROW] && curr->position[COL] >= maxPos[COL])
			{
				maxPos[ROW] = curr->position[ROW];
				maxPos[COL] = curr->position[COL];
			}

			curr = curr->next;
		}
	}
}

// Check if the given position is a valid neighbor and return the answer
bool isValidNeighbor(GRAY_IMAGE* img, unsigned char threshold, unsigned char startPos, bool** isPixelInSegment, IMG_POS position)
{
	// The neighbor is in the matrix
	if (position[ROW] < MIN_ROW || position[ROW] >= img->rows || position[COL] < MIN_COL || position[COL] >= img->cols)
	{
		return false;
	}

	// The neighbor is in a segment already
	else if (isPixelInSegment[position[ROW]][position[COL]])
	{
		return false;
	}

	// The neighbor is not in the threshold
	else if (abs(startPos - img->pixels[position[ROW]][position[COL]]) > threshold)
	{
		return false;
	}

	return true;
}

// Create new bool matrix, init its values with false and retrun its pointer
// Dosent free the new rows array and the array of every row! Must happend after use.
bool** createBoolMatrix(unsigned short rows, unsigned short cols)
{
	bool** res;

	// Create the rows array
	res = (bool**)malloc(sizeof(bool*) * rows);
	mallocFail(res);

	// Create array for every row
	for (int i = 0; i < rows; i++)
	{
		res[i] = (bool*)malloc(sizeof(bool) * cols);
		mallocFail(res[i]);

		// Init every cell to be false
		for (int j = 0; j < cols; j++)
		{
			res[i][j] = false;
		}
	}

	return res;
}

// Free the given segment
void freeSegment(SEGMENT segment)
{
	freeTNode(segment.root);
}

// Free the the given Bool matrix 
// Free the row's array and the arrays of every row
// Assume that the given boolMatrix is large enough for the given rows.
void freeBoolMatrix(bool** boolMatrix, unsigned short rows)
{
	// Free the array of every row
	for (int i = 0; i < rows; i++)
	{
		free(boolMatrix[i]);
	}

	// Free the row's array
	free(boolMatrix);
}

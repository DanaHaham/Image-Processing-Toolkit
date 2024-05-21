#include "menu.h"

// Menu of the program
// Scan a wanted action from the user and call the relevent function
// End with exit input
void menu()
{
	COLOR_IMAGE* colorImg = NULL;
	GRAY_IMAGE* grayImgSec2 = NULL, * grayImgSec4 = NULL;
	unsigned char depthImgSec2 = 0;

	IMG_POS_LIST* segmentsSec3 = NULL;
	int sizeSegmentsSec3;

	int selection = 0;

	// Start program
	while (selection != EXIT_PROGRAM)
	{
		// Print menu
		printf("Welcome to Nitzan and Dana final project!\n Take a look on our menu!\n");
		printf("1. Read an image file in PPM (p3) format\n2. Read an image file in PGM(p2) format\n3. Find all segments\n4. Color the segments\n5. Save the colored image to PGM\n6. Compress and save the original image in a compressed format\n7. Convert a compressed image to PGM format\n8. Convert PPM to PGM(p3->p2)\n9. Convert PGM to BW(p2->p2)\n10.Convert PPM to PGM(p6->p5)\n11.Convert PGM to BW(p5->p5)\n12.Exit program\n\n");

		// Scan an action from the user
		printf("What would you like to do? Please choose a number of an action from the menu: ");
		scanf("%d", &selection);

		switch (selection)
		{
		case 1:
		{
			colorImg = section1();
			break;
		}
		case 2:
		{
			grayImgSec2 = section2(&depthImgSec2);
			break;
		}
		case 3:
		{
			sizeSegmentsSec3 = section3(grayImgSec2, &segmentsSec3);
			break;
		}
		case 4:
		{
			grayImgSec4 = section4(segmentsSec3, sizeSegmentsSec3);
			break;
		}
		case 5:
		{
			section5(grayImgSec4, depthImgSec2);
			break;
		}
		case 6:
		{
			section6();
			break;
		}
		case 7:
		{
			section7();
			break;
		}
		case 8:
		{
			section8();
			break;
		}

		case 9:
		{
			section9();
			break;
		}

		case 10:
		{
			section10();
			break;
		}

		case 11:
		{
			section11();
			break;
		}
		case EXIT_PROGRAM:
		{
			// Free the allocations
			freeColorImage(colorImg);
			freeGrayImage(grayImgSec2);
			freeGrayImage(grayImgSec4);
			freePosListArr(segmentsSec3, sizeSegmentsSec3);
			break;
		}
		default:
		{
			printf("Invalid choice! Try again.\n\n");
			break;
		}
		}
	}

	exit(1);
}

// Create a new color image acordding to the user and return it
// Dosent free the new COLOR_IMAGE! Must happend after use.
COLOR_IMAGE* section1()
{
	char* fname;
	COLOR_IMAGE* colorImg;

	// Scan the name of the PPM file with P3 magic number
	printf("Please enter the name of the PPM file with Magic Number P3: ");
	fname = scanName();

	// Call the funcion
	colorImg = readPPM(fname);
	printf("\nThe color image is saved succssesfully in the memory!\n\n");

	free(fname);
	return colorImg;
}

// Create a new gray image acordding to the user and return it and the depth of the image (by refernce)
// Dosent free the new GRAY_IMAGE! Must happend after use.
GRAY_IMAGE* section2(unsigned char* depth)
{
	char* fname;
	GRAY_IMAGE* grayImg;

	// Scan the name of the PGM file with P2 magic number
	printf("Please enter the name of the PGM file with Magic Number P2: ");
	fname = scanName();

	// Call the funcion
	grayImg = readPGM(fname);
	printf("\nThe gray image is saved succssesfully in the memory!\n\n");

	// Get the depth from the file
	*depth = getDeptFromFile(fname);

	free(fname);
	return grayImg;
}

// Create segments array for the gicen gray image and return the size of the array and the array by reference
// Dosent free the new IMG_POS_LIST array! Must happend after use.
int section3(GRAY_IMAGE* grayImg, IMG_POS_LIST** pSegments)
{
	IMG_POS_LIST* segments;
	unsigned char treshold;
	int sizeSegments;

	// Check if a gray image from section 2 was loaded
	if (grayImg == NULL)
	{
		printf("You haven't lodad an image! Call section 2 before\n\n");
		return 0;
	}
	else
	{
		// Scan the name of the PGM file
		printf("Please enter treshold: ");
		scanf("%hhu", &treshold);

		// Call the function
		sizeSegments = findAllSegments(grayImg, treshold, &segments);
		printf("\nThe segments of the gray image are saved succssesfully in the memory!\n\n");

		*pSegments = segments;
		return sizeSegments;
	}
}

// Create new gray image from the given segments array
// Assume that the given segments is large enough for the given size
// Dosent free the new GRAY_IMAGE! Must happend after use.
GRAY_IMAGE* section4(IMG_POS_LIST* segments, int sizeSegments)
{
	GRAY_IMAGE* grayImg = NULL;

	// Check if there are segments that were loaded from section 3
	if (segments == NULL)
	{
		printf("You haven't lodad segments! Call section 3 before\n\n");
	}
	else
	{
		// Call the function
		grayImg = colorSegments(segments, sizeSegments);
		printf("\nThe new gray image with the segments is saved succssesfully in the memory!\n\n");
	}

	return grayImg;
}

// Create new PGM file for the given gray image
void section5(GRAY_IMAGE* grayImg, unsigned char depth)
{
	char* fname, magicNumber[MAGIC_SIZE];

	// Check if there is gray image that was created in section 4
	if (grayImg == NULL)
	{
		printf("You haven't lodad a gray image! Call section 4 before\n\n");
	}
	else
	{
		// Scan the name of the PGM file and the wanted magic number
		printf("Please enter the name of the PGM file: ");
		fname = scanName();

		printf("Please enter wanted magic number: ");
		scanf("%s", magicNumber);

		// Create a new PGM file with the gray image's data
		createPGMFile(fname, grayImg, depth, magicNumber[ID_MAGIC_INDEX]);
		printf("\n A new file of the gray image is saved succssesfully!\n\n");

		free(fname);
	}
}

// Create a new file with a compress gray image 
void section6()
{
	FILE* fp;
	GRAY_IMAGE* grayImg;
	int maxGrayLevel;
	char* fname, magicNumber[MAGIC_SIZE];

	// Scan the name of the PGM file
	printf("Please enter the name of the PGM file: ");
	fname = scanName();

	// Scan the max level gray
	printf("Please enter the max gray level: ");
	scanf("%d", &maxGrayLevel);

	// Open the file
	fp = fopen(fname, "r");
	openFileFail(fp);

	// Get the magic number from the file
	skipUntilData(fp);
	fgets(magicNumber, MAGIC_SIZE, fp);

	// Close the file
	fclose(fp);

	// Create a new image
	if (magicNumber[ID_MAGIC_INDEX] == P2)
	{
		grayImg = readPGM(fname);
	}
	else
	{
		grayImg = readPGM_Bin(fname);
	}

	// Call the funcion
	saveCompressed(fname, grayImg, maxGrayLevel);
	printf("\n A new binary file of the compressed gray image is saved succssesfully!\n\n");

	freeGrayImage(grayImg);
	free(fname);
}

// Create a new PGM file with P2 magic number with an extract gray image 
void section7()
{
	char* fname;

	// Scan the name of the PGM file
	printf("Please enter the name of the PGM compressed file: ");
	fname = scanName();

	// Call the funcion
	convertCompressedImageToPGM(fname);
	printf("\n A new file (with magic number P2) of the extraced gray image is saved succssesfully!\n\n");

	free(fname);
}

// Create a new PGM file with P2 magic number
void section8()
{
	char* fname;

	// Scan the name of the PPM file with magic number P3
	printf("Please enter the name of the PPM file with Magic Number P3: ");
	fname = scanName();

	// Call the funcion
	convertPPMToPGM(fname);
	printf("\n A new file (with magic number P2) of the gray image is saved succssesfully!\n\n");

	free(fname);
}

// Create a new BW PGM files with P2 magic number
void section9()
{
	char* fname;

	// Scan the name of the PGM file with magic number P2
	printf("Please enter the name of the PGM file with Magic Number P2: ");
	fname = scanName();

	// Call the funcion
	convertPGMToBW(fname);
	printf("\n A new files (with magic number P2) of the BW image are saved succssesfully!\n\n");

	free(fname);
}

// Create a new PGM file with P5 magic number
void section10()
{
	char* fname;

	// Scan the name of the PPM file with magic number P6
	printf("Please enter the name of the PPM file with Magic Number P6: ");
	fname = scanName();

	// Call the funcion
	convertPPMToPGM_Bin(fname);
	printf("\n A new file (with magic number P5) of the gray image is saved succssesfully!\n\n");

	free(fname);
}

// Create a new BW PGM files with P5 magic number
void section11()
{
	char* fname;

	// Scan the name of the PGM file with magic number P5
	printf("Please enter the name of the PGM file with Magic Number P5: ");
	fname = scanName();

	// Call the funcion
	convertPGMToBW_Bin(fname);
	printf("\n A new files (with magic number P5) of the BW image are saved succssesfully!\n\n");

	free(fname);
}

// Scan from the user a name of file, and return it.
// Dosent free the new dynamic string that is created! Must happend after use
char* scanName()
{
	char* res;
	char currChar;
	unsigned short logSizeName, phsSizeName;

	// Create new 
	res = createString(&logSizeName, &phsSizeName);

	scanf("%c", &currChar);
	scanf("%c", &currChar);

	while (currChar != NEW_LINE)
	{
		res = updateSizeString(res, logSizeName, &phsSizeName);

		res[logSizeName] = currChar;
		logSizeName++;

		scanf("%c", &currChar);
	}

	logSizeName++;

	// Update the size of the string
	res = (char*)realloc(res, sizeof(char) * logSizeName);
	mallocFail(res);

	res[logSizeName - 1] = END_STR;

	return res;
}

// Create and return a new dynamic string, its log size and its physc size
// Dosent free the new dynamic string that is created! Must happend after use
char* createString(unsigned short* pLogSize, unsigned short* pPhsSize)
{
	char* res;

	// Init the size of the string
	*pLogSize = 0;
	*pPhsSize = INIT_ARR_SIZE;

	// Create the string
	res = (char*)malloc(*pPhsSize * sizeof(char));
	mallocFail(res);

	return res;
}

// Check if there is'nt a engouh place in the given string and if it is needed, allocate more memory
// Return the string and it's new phsycs size
// Dosent free the new dynamic string that is created! Must happend after use
char* updateSizeString(char* name, unsigned short logSize, unsigned short* pPhsSize)
{
	unsigned int phsSize = *pPhsSize;

	// There is no place in the string
	if (logSize == phsSize)
	{
		// Increase the physical size of the string
		phsSize *= INCREASE_PHS_SIZE;
		name = (char*)realloc(name, phsSize * sizeof(char));
		mallocFail(name);
	}

	*pPhsSize = phsSize;
	return name;
}
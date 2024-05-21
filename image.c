#include "Image.h"

// Create new COLOR_IMAGE from the data in the given file (with magic number P3) and retrun its pointer
// Dosent free the new COLOR_IMAGE and its arrays! Must happend after use
COLOR_IMAGE* readPPM(char* fname)
{
	COLOR_IMAGE* res;
	FILE* fp;
	char magicNumber;
	unsigned short rows, cols;
	unsigned char depth;
	long int pixelPointer;

	// Get all the data in the header of the file (Magic number, rows, cols and depth)
	pixelPointer = getHeader(fname, &rows, &cols, &depth, &magicNumber);
	isValidMagicNumber(magicNumber, P3);
	
	// Open PPM file
	fp = fopen(fname, "r");
	openFileFail(fp);

	// Move the file's pointer to the place of the pixels
	fseek(fp, pixelPointer, SEEK_SET);

	// Create a new color image with empty pixels
	res = createColorImage(rows, cols);

	// Init the image's pixels
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(fp, "%hhu", &res->pixels[i][j].r);
			skipUntilData(fp);

			fscanf(fp, "%hhu", &res->pixels[i][j].g);
			skipUntilData(fp);

			fscanf(fp, "%hhu", &res->pixels[i][j].b);
			skipUntilData(fp);
		}
	}

	fclose(fp);
	return res;
}

// Create new GRAY_IMAGE from the data in the given file (with magic number P2) and retrun its pointer
// Dosent free the new GRAY_IMAGE and its arrays! Must happend after use
GRAY_IMAGE* readPGM(char* fname)
{
	GRAY_IMAGE* res;
	FILE* fp;
	char magicNumber;
	unsigned short rows, cols;
	unsigned char depth;
	long int pixelPointer;

	// Get all the data in the header of the file (Magic number, rows, cols and depth)
	pixelPointer = getHeader(fname, &rows, &cols, &depth, &magicNumber);
	isValidMagicNumber(magicNumber, P2);

	// Open image
	fp = fopen(fname, "r");
	openFileFail(fp);

	// Move the file's pointer to the place of the pixels
	fseek(fp, pixelPointer, SEEK_SET);
	
	// Create a new color image with empty pixels
	res = createGrayImage(rows, cols);

	// Init the image's pixels
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(fp, "%hhu", &res->pixels[i][j]);
			skipUntilData(fp);
		}
	}

	fclose(fp);
	return res;
}

// Create new COLOR_IMAGE from the data in the given file (with magic number P6) and retrun its pointer
// Dosent free the new COLOR_IMAGE and its arrays! Must happend after use
COLOR_IMAGE* readPPM_Bin(char* fname)
{
	COLOR_IMAGE* res;
	FILE* fp;
	char magicNumber;
	unsigned short rows, cols;
	unsigned char depth;
	long int pixelPointer;

	// Get all the data in the header of the given PPM file (Magic number, rows, cols and depth)
	pixelPointer = getHeader(fname, &rows, &cols, &depth, &magicNumber);
	isValidMagicNumber(magicNumber, P6);

	// Open PPM file to read
	fp = fopen(fname, "rb");
	openFileFail(fp);

	// Move the PPM file's pointer to the place of the pixels
	fseek(fp, pixelPointer, SEEK_SET);

	// Create a new color image with empty pixels
	res = createColorImage(rows, cols);

	// Init the image's pixels
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			// Get the RGB's pixel from the PPM file
			fread(&res->pixels[i][j].r, sizeof(unsigned char), 1, fp);
			fread(&res->pixels[i][j].g, sizeof(unsigned char), 1, fp);
			fread(&res->pixels[i][j].b, sizeof(unsigned char), 1, fp);
		}
	}

	// Close PPM file 
	fclose(fp);
	return res;
}

// Create new GRAY_IMAGE from the data in the given file (with magic number P5) and retrun its pointer
// Dosent free the new GRAY_IMAGE and its arrays! Must happend after use
GRAY_IMAGE* readPGM_Bin(char* fname)
{
	GRAY_IMAGE* res;
	FILE* fp;
	char magicNumber;
	unsigned short rows, cols;
	unsigned char depth;
	long int pixelPointer;

	// Get all the data in the header of the file (Magic number, rows, cols and depth)
	pixelPointer = getHeader(fname, &rows, &cols, &depth, &magicNumber);
	isValidMagicNumber(magicNumber, P5);

	// Open image
	fp = fopen(fname, "rb");
	openFileFail(fp);

	// Move the file's pointer to the place of the pixels
	fseek(fp, pixelPointer, SEEK_SET);

	// Create a new color image with empty pixels
	res = createGrayImage(rows, cols);

	// Init the image's pixels
	for (int i = 0; i < rows; i++)
	{
		fread(res->pixels[i], sizeof(unsigned char), cols, fp);
	}

	fclose(fp);
	return res;
}

// Create new COLOR_IMAGE, update his values acordding to the given variables and retrun its pointer
// Create an array of pointers to RGB's arrays (table of color pixels, color pixel = RGB).
// The pixels are empty.
// Dosent free the new COLOR_IMAGE and the arrays! Must happend after use.
COLOR_IMAGE* createColorImage(unsigned short rows, unsigned short cols)
{
	COLOR_IMAGE* res;

	// Create new color image
	res = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE));
	mallocFail(res);

	// Init rows and cols
	res->cols = cols;
	res->rows = rows;

	// Create an array of pixel's pointers 
	res->pixels = (RGB**)malloc(sizeof(RGB*) * rows);
	mallocFail(res->pixels);

	// Create arrays of pixels 
	for (int i = 0; i < rows; i++)
	{
		res->pixels[i] = (RGB*)malloc(sizeof(RGB) * cols);
		mallocFail(res->pixels[i]);
	}

	return res;
}

// Create new GRAY_IMAGE, update his values acordding to the given variables and retrun its pointer
// Create an array of pointers to gray pixel's arrays (table of gray pixels).
// The pixels are empty.
// Dosent free the new GRAY_IMAGE and the arrays! Must happend after use.
GRAY_IMAGE* createGrayImage(unsigned short rows, unsigned short cols)
{
	GRAY_IMAGE* res;

	// Create new gray image
	res = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	mallocFail(res);

	// Init rows and cols
	res->cols = cols;
	res->rows = rows;

	// Create an array of pixel's pointers 
	res->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * rows);
	mallocFail(res->pixels);

	// Create arrays of gray pixels 
	for (int i = 0; i < rows; i++)
	{
		res->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * cols);
		mallocFail(res->pixels[i]);
	}

	return res;
}

// Create a new PGM file from the given gray image's data acordding to the given magicNumber
void createPGMFile(char* fname, GRAY_IMAGE* grayImg, unsigned char depth, char magicNumber)
{
	FILE* fp;
	long int pixelPointer;

	// Print the header's data
	pixelPointer = printHeader(fname, grayImg->rows, grayImg->cols, depth, magicNumber);

	if (magicNumber == P2)
	{
		// Open new PGM file as ascii
		fp = fopen(fname, "r+");
		openFileFail(fp);

		// Move the file's pointer to the place of the pixels
		fseek(fp, pixelPointer, SEEK_SET);

		// Print the image data as ascii
		for (int i = 0; i < grayImg->rows; i++)
		{
			for (int j = 0; j < grayImg->cols; j++)
			{
				fprintf(fp, "%hhu ", grayImg->pixels[i][j]);
			}

			fprintf(fp, "\n");
		}
	}
	else 
	{
		// Open new PGM file as binary
		fp = fopen(fname, "rb+");
		openFileFail(fp);

		// Move the file's pointer to the place of the pixels
		fseek(fp, pixelPointer, SEEK_SET);

		// Print the image data as binary
		for (int i = 0; i < grayImg->rows; i++)
		{
			// Print every row
			fwrite(grayImg->pixels[i], sizeof(unsigned char), grayImg->cols, fp);
		}
	}

	fclose(fp);
}

// Return the depth from the file (given as name of the file)
unsigned char getDeptFromFile(char* fname)
{
	char magicNumber;
	unsigned short rows, cols;
	unsigned char depth;
	long int pixelPointer;

	// Get all the data in the header of the file (Magic number, rows, cols and depth)
	pixelPointer = getHeader(fname, &rows, &cols, &depth, &magicNumber);

	return depth;
}

// Get all the data in the header of the given file (Magic number, rows, cols and depth)
// Return the rows, cols, depth and identifical number through the given pointers (change them)
// Return the place where the pixels is worte through the file's pointer
long int getHeader(char* fname, unsigned short* rows, unsigned short* cols, unsigned char* depth, char* magicNumberId)
{
	FILE* fp;
	char magicNumber[MAGIC_SIZE];
	long int filePointer;

	// Open ascii file
	fp = fopen(fname, "r");
	openFileFail(fp);

	// Get the magic number from the file
	skipUntilData(fp);
	fgets(magicNumber, MAGIC_SIZE, fp);

	// Get cols from the file
	skipUntilData(fp);
	fscanf(fp, "%hu", cols);

	// Get rows from the file
	skipUntilData(fp);
	fscanf(fp, "%hu", rows);

	// Get depth from the file
	skipUntilData(fp);
	fscanf(fp, "%hhu", depth);
	skipUntilData(fp);

	filePointer = ftell(fp);
	fclose(fp);

	// Return the identifical number of the current magic
	*magicNumberId = magicNumber[ID_MAGIC_INDEX];

	// Return the current place of the file's pointer (pixel's start place)
	return filePointer;
}

// Print to a given file the header with the given data (rows, cols, depth, magicNumber)
// Return the place where the header end
long int printHeader(char* fname, unsigned short rows, unsigned short cols, unsigned char depth, char magicNumber)
{
	FILE* fp;
	long int filePointer;

	// Open ascii file
	fp = fopen(fname, "w");
	openFileFail(fp);

	// Print the magic Number
	fprintf(fp, "P%c\n", magicNumber);

	// Print the cols, rows and depth
	fprintf(fp, "%hu %hu %hhu\n", cols, rows, depth);

	filePointer = ftell(fp);
	fclose(fp);

	// Return the current place of the file's pointer (pixel's start place)
	return filePointer;
}

// If the file's pointer is on a comment - skip it and all the comments that follow it until new data
// Change the given file's pointer place
void skipUntilData(FILE* fp)
{
	bool isComment = false;
	char ch;
	long int saver;
	
	saver = ftell(fp);
	ch = fgetc(fp);

	// As long as the chars are withspaces or a comment, skip them
	while (ch == TAB || ch == NEW_LINE || ch == SPACE || ch == CR || ch == V_TAB || isComment || ch == COMMENT)
	{
		// Check if a comment line is starting or ending
		if (ch == NEW_LINE || ch == CR)
		{
			isComment = false;

			// Prevent form enter to "eat" another char
			if (ftell(fp) - saver >1)
			{
				fseek(fp, -1, SEEK_CUR);
			}
		}
		else if (ch == COMMENT)
		{
			isComment = true;
		}

		saver = ftell(fp);
		ch = fgetc(fp);
	}

	// Loop ends - the last char scaned is an useful data, go back to its place in the file
	fseek(fp, saver, SEEK_SET);
}

// Free color pixels table
// Free the given image
void freeColorImage(COLOR_IMAGE* colorImg)
{
	// Free the arrays of color pixels
	for (int i = 0; i < colorImg->rows; i++)
	{
		free(colorImg->pixels[i]);
	}

	// Free the array of pixel's pointers 
	free(colorImg->pixels);

	// Free image
	free(colorImg);
}

// Free gray pixels table
// Free the given image
void freeGrayImage(GRAY_IMAGE* grayImg)
{
	// Free the arrays of gray pixels 
	for (int i = 0; i < grayImg->rows; i++)
	{
		free(grayImg->pixels[i]);
	}

	// Free the array of pointers of pixels
	free(grayImg->pixels);

	// Free image
	free(grayImg);
}




#include "imageCompress.h"

// Create a new binary file with the data of the given gray image in a compress way
// Save the pixels acordding to the given max gray level (max gray level < 128)
void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel)
{
	unsigned char* pixelArr;
	BYTE* compressedPixels;
	int bitsPerPixel;
	unsigned short sizeCompPixels;

	// Create an array of pixels with new values for every pixel from the given image - acordding to the given max gray level 
	// The values of the pixels are smaller than 128 (128 = 2^7), therefore, occupates less than BYTE (maximum 7 bits)
	pixelArr = convertPixelsMaxGray(image, maxGrayLevel);

	// Get the minimal numbers of bits required for save the pixels acordding to the given max gray level
	bitsPerPixel = getBitsPerPixel(maxGrayLevel);

	// Copmress the array of pixels acordding to the bits required per pixel
	compressedPixels = compressGrayImagePixels(pixelArr, (image->rows * image->cols), bitsPerPixel, &sizeCompPixels);

	// Create a new binary file and save in it the compressed image 
	createCompressFile(fname, compressedPixels, sizeCompPixels, image->rows, image->cols, maxGrayLevel);

	free(pixelArr);
	free(compressedPixels);
}

// Create a new PGM file with magic number P2 with the data of the given compressed image after extract it
void convertCompressedImageToPGM(char* fname)
{
	GRAY_IMAGE* grayImg;
	BYTE* pixelArr;
	unsigned short rows, cols;
	unsigned char maxGrayLevel, * extractPixels;
	int bitsPerPixel, indexExtract;
	char* newFname;

	// Read all the data from the given file
	pixelArr = readCompreesImage(fname, &rows, &cols, &maxGrayLevel, &bitsPerPixel);

	// Extract the pixels from the compressed array
	extractPixels = extractGrayImagePixels(pixelArr, (cols * rows), bitsPerPixel);

	// Create new gray image from the compressed image data
	grayImg = createGrayImage(rows, cols);

	indexExtract = 0;

	// Init the pixels in the gray image
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grayImg->pixels[i][j] = extractPixels[indexExtract];
			indexExtract++;
		}
	}

	// Create name for the new PGM file from the compressed image file's name
	newFname = createPGMNameFromCompressedImge(fname);

	// Create PGM file with P2 magic number from the gray image
	createPGMFile(newFname, grayImg, maxGrayLevel, P2);

	free(pixelArr);
	free(extractPixels);
	free(newFname);
	freeGrayImage(grayImg);
}

// Copmress the given array of pixels acordding to the bits required per pixel
// The values of the pixels are smaller than 128 (128 = 2^7), therefore, occupates less than BYTE (maximum 7 bits)
// Return to the compressed pixels (BYTE array) and its size (by refernce - pSizeRes)
// Dosent free the BYTE array! Must happend after use.
BYTE* compressGrayImagePixels(unsigned char* pixelArr, unsigned short sizePixel, int bitsPerPixel, unsigned short* pSizeRes)
{
	BYTE* res;
	unsigned short sizeRes;
	int indexRes, indexPixels, bitsRemaining, bitsPerPixelTemp;

	indexRes = indexPixels = 0;
	bitsRemaining = BITS_PER_BYTE;
	bitsPerPixelTemp = bitsPerPixel;

	// Calculate the size of the compressed array
	sizeRes = ((sizePixel * bitsPerPixel) / BITS_PER_BYTE);

	if ((sizePixel * bitsPerPixel) % BITS_PER_BYTE != 0)
	{
		sizeRes++;
	}

	// Create an array of compress pixels and init it with zero
	res = (BYTE*)calloc(sizeRes, sizeof(BYTE));
	mallocFail(res);

	// Compress all the pixels in the given array
	while (indexPixels < sizePixel)
	{
		unsigned char  compressPixel, mask = 0xFF;

		// There is place for all the pixel's bits in the current Byte
		if (bitsRemaining >= bitsPerPixel)
		{
			// Move the bits to the first availabe bit in the current byte 
			compressPixel = pixelArr[indexPixels] << (bitsRemaining - bitsPerPixel);
			mask << (BITS_PER_BYTE - bitsRemaining);
			compressPixel &= mask;
			res[indexRes] |= compressPixel;

			// Update the number of the remaning bits in the current Byte
			bitsRemaining -= bitsPerPixel;

			// Move to the next pixel
			indexPixels++;
			bitsPerPixel = bitsPerPixelTemp;
		}

		// There is no place in the current Byte
		else if (bitsRemaining == EMPTY_BYTE)
		{
			// Move to the next Byte
			bitsRemaining = BITS_PER_BYTE;
			indexRes++;
		}

		else
		{
			// Move the bits that we didnt use to the first availabe bit in the current byte 
			compressPixel = pixelArr[indexPixels] << (BITS_PER_BYTE - bitsPerPixel);
			compressPixel >>= (BITS_PER_BYTE - bitsRemaining);

			res[indexRes] |= compressPixel;

			// Update the remaning place in the current Byte
			bitsPerPixel -= bitsRemaining;

			// Move to the next pixel
			bitsRemaining = BITS_PER_BYTE;
			indexRes++;
		}
	}

	*pSizeRes = sizeRes;
	return res;
}

// Extract the given array of pixels acordding to the bits required per pixel
// The values of the pixels are smaller than 128 (128 = 2^7), therefore, occupates less than BYTE (maximum 7 bits)
// Return to the extraced pixels (unsigned char array).
// Dosent free the unsigned char array! Must happend after use.
unsigned char* extractGrayImagePixels(BYTE* pixelArr, unsigned short sizeRes, int bitsPerPixel)
{
	unsigned char* res;
	int indexRes, indexPixels, bitsRemaining, bitsPerPixelTemp;

	indexRes = indexPixels = 0;
	bitsRemaining = BITS_PER_BYTE;
	bitsPerPixelTemp = bitsPerPixel;

	// Create an array of extract pixels and init it with zero
	res = (unsigned char*)calloc(sizeRes, sizeof(unsigned char));
	mallocFail(res);

	// Compress all the pixels in the array
	while (indexRes < sizeRes)
	{
		unsigned char  extractPixel, mask = 0xFF;

		// The all bits of the pixel's are in the current Byte
		if (bitsRemaining >= bitsPerPixel)
		{
			// Move the bits that we didnt use to the first availabe bit in the current extract array 
			extractPixel = pixelArr[indexPixels] >> (bitsRemaining - bitsPerPixel);
			res[indexRes] |= extractPixel;

			if (bitsRemaining == bitsPerPixel)
			{
				mask >>= (BITS_PER_BYTE - bitsRemaining);
				res[indexRes] &= mask;
			}

			// Update the number of the remaning bits in the current Byte
			bitsRemaining -= bitsPerPixel;

			// Move to the next extract pixel
			indexRes++;
			bitsPerPixel = bitsPerPixelTemp;
		}

		// There is no place in the current pixel
		else if (bitsRemaining == EMPTY_BYTE)
		{
			// Move to the next pixel
			bitsRemaining = BITS_PER_BYTE;
			indexPixels++;
		}

		else
		{
			// Move the bits that we didnt use to the first availabe bit in the current res 
			extractPixel = pixelArr[indexPixels] << (BITS_PER_BYTE - bitsRemaining);
			extractPixel >>= (BITS_PER_BYTE - bitsPerPixel);

			res[indexRes] |= extractPixel;

			// Update the remaning place in the current Byte
			bitsPerPixel -= bitsRemaining;

			// Move to the next pixel
			bitsRemaining = BITS_PER_BYTE;
			indexPixels++;
		}
	}

	return res;
}

// Create a new binary file with the data of the given compressed pixels, rows, cols and max gray level
// Assume that the given compressed pixels array is large enough for the given size
void createCompressFile(char* fname, BYTE* compressedPixels, unsigned short sizeCompPixels, unsigned short rows, unsigned short cols, unsigned char maxGrayLevel)
{
	FILE* fp;
	
	// Create new file
	fp = fopen(fname, "wb");
	openFileFail(fp);

	// Write the given rows, cols and max gray level to the file
	fwrite(&rows, sizeof(unsigned short), 1, fp);
	fwrite(&cols, sizeof(unsigned short), 1, fp);
	fwrite(&maxGrayLevel, sizeof(unsigned char), 1, fp);

	// Write the given compressed pixels to file
	fwrite(compressedPixels, sizeof(BYTE), sizeCompPixels, fp);

	// Close the file:
	fclose(fp);
}

// Get all the data from the given comprees file (compressed pixels, rows, cols and maxGrayLevel)
// Return the rows, cols, maxGrayLevel and the bitsPerPixel through the given pointers (change them)
// Create and return a BYTE array of compressed pixels 
// Dosent free the new array! Must happend after use.
BYTE* readCompreesImage(char* fname, unsigned short* rows, unsigned short* cols, unsigned char* maxGrayLevel, int* bitsPerPixel)
{
	FILE* fp;
	unsigned short sizePixel;
	BYTE* pixelArr;

	// Open binary file
	fp = fopen(fname, "rb");
	openFileFail(fp);

	// Read from file rows, cols, max gray level
	fread(rows, sizeof(short int), 1, fp);
	fread(cols, sizeof(short int), 1, fp);
	fread(maxGrayLevel, sizeof(unsigned char), 1, fp);

	// Get the minimal numbers of bits required for save the pixels acordding to the given max gray level
	*bitsPerPixel = getBitsPerPixel(*maxGrayLevel);

	// Calculate the size of the compressed array
	sizePixel = (((*rows) * (*cols) * (*bitsPerPixel)) / BITS_PER_BYTE);

	if (((*rows) * (*cols) * (*bitsPerPixel)) % BITS_PER_BYTE != 0)
	{
		sizePixel++;
	}

	// Get the compressed pixels
	pixelArr = (BYTE*)malloc(sizeof(BYTE) * sizePixel);
	mallocFail(pixelArr);

	fread(pixelArr, sizeof(BYTE), sizePixel, fp);

	fclose(fp);
	return pixelArr;
}

// Create an array of pixels with new values for every pixel from the given image - acordding to the given max gray level and return it
// Dosent free the new array! Must happend after use.
unsigned char* convertPixelsMaxGray(GRAY_IMAGE* image, unsigned char maxGrayLevel)
{
	unsigned char* res;
	int indexRes = 0;

	// Create an array of pixels
	res = (unsigned char*)malloc(sizeof(unsigned char) * (image->rows * image->cols));
	mallocFail(res);

	// Init the array with the values of the given image's pixels acordding to the given max gray level 
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			res[indexRes] = (image->pixels[i][j] * maxGrayLevel) / MAX_DEPTH;
			indexRes++;
		}
	}

	return res;
}

// Return the minimal number of bits required for save the pixels acordding to the given max gray level
int getBitsPerPixel(unsigned char maxGrayChar)
{
	int count, maxGray;

	count = 0;
	maxGray = (int)maxGrayChar;

	// Count bit requierd
	while (maxGray > 0)
	{
		maxGray = maxGray / BIN_BASE;
		count++;
	}

	return count;
}

// Create name for a PGM file from compressed image file's name
// Dosent free the new name that is created! Must happend after use.
char* createPGMNameFromCompressedImge(char* fname)
{
	char* res;
	unsigned int lenFname;

	lenFname = strlen(fname);

	// Size of the original name, ".pgm" and \0
	res = (char*)malloc(sizeof(char) * (lenFname + 5));
	mallocFail(res);

	// Update the name to be the same as the given name with ".pgm" end
	sprintf(res, "%s.pgm", fname);

	res[lenFname + 4] = END_STR;

	return res;
}
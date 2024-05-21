#include "imageConvert.h"

// Convert a PPM file with P3 Magic Number (given as name of the file) to a new PGM file with P2 Magic Number.
// The new PGM file contain the gray version of the image given in the PPM file
void convertPPMToPGM(char* fname)
{
	COLOR_IMAGE* colorImg;

	// Read the color imgae with P3 magic number
	colorImg = readPPM(fname);

	// Create PGM file with P2 magic number from the image color data
	convertColorImageToPGM(fname, colorImg, P2);

	freeColorImage(colorImg);
}

// Convert a PGM file with P2 Magic Number (given as name of the file) to a new BW files (for every k) with P2 Magic Number.
// The new BW file contain the black and white version of the image given in the PGM file (acordding to dithering's algorithm)
void convertPGMToBW(char* fname)
{
	// Read a gray imgae with P2
	GRAY_IMAGE* grayImg = readPGM(fname);

	// Create a file for every k (2,3,4)
	for (int k = 2; k <= 4; k++)
	{
		convertGrayImageToBw(fname, grayImg, k, P2);
	}

	freeGrayImage(grayImg);
}

// Convert a PPM file with P6 Magic Number (given as name of the file) to a new PGM file with P5 Magic Number.
// The new PGM file contain the gray version of the image given in the PPM file
void convertPPMToPGM_Bin(char* fname)
{
	COLOR_IMAGE* colorImg;

	// Read the color imgae with P6 magic number
	colorImg = readPPM_Bin(fname);

	// Create PGM file with P5 magic number from the image color data
	convertColorImageToPGM(fname, colorImg, P5);

	freeColorImage(colorImg);
}

// Convert a PGM file with P5 Magic Number (given as name of the file) to a new BW files (for every k) with P5 Magic Number.
// The new BW file contain the black and white version of the image given in the PGM file (acordding to dithering's algorithm)
void convertPGMToBW_Bin(char* fname)
{
	// Read a gray imgae with P5
	GRAY_IMAGE* grayImg = readPGM_Bin(fname);

	// Create a file for every k (2,3,4)
	for (int k = 2; k <= 4; k++)
	{
		convertGrayImageToBw(fname, grayImg, k, P5);
	}

	freeGrayImage(grayImg);
}

// Convert the given color image to a gray image and create a PGM file with the given Magic Number.
void convertColorImageToPGM(char* orgFname, COLOR_IMAGE* colorImg, char magicNumber)
{
	GRAY_IMAGE* grayImg;
	char* fnamePgm;
	unsigned char depth;

	// Create new gray image from the data of the color image
	grayImg = createGrayImage(colorImg->rows, colorImg->cols);

	// Init the pixels table in the gray image to be an average of the match pixel in the color image's RGB
	for (int i = 0; i < colorImg->rows; i++)
	{
		for (int j = 0; j < colorImg->cols; j++)
		{
			grayImg->pixels[i][j] = colorToGreyPixel(colorImg->pixels[i][j]);
		}
	}

	// Create name for the new PGM file from the PPM file's name
	fnamePgm = createPGMNameFromPPM(orgFname);

	// Get the depth from the PPM file
	depth = getDeptFromFile(orgFname);

	// Create a new PGM file with the gray image's data 
	createPGMFile(fnamePgm, grayImg, depth, magicNumber);

	free(fnamePgm);
	freeGrayImage(grayImg);
}

// Create a new BW file for the given k matrix with the given Magic Number from the given gray image's data
// The new BW file contain the black and white version of the image given in the PGM file (acordding to dithering's algorithm)
void convertGrayImageToBw(char* orgFname, GRAY_IMAGE* grayImg, int k, char magicNumber)
{
	char* fnamek;
	unsigned char depth, threshold;
	GRAY_IMAGE* bwImage;

	unsigned char mask[3][4][4] = { { {0,2, NULL, NULL},{3,1, NULL, NULL},{NULL,NULL, NULL, NULL}, {NULL,NULL, NULL, NULL} },
									{ {2,6,4, NULL},{5,0,1, NULL},{8,3,7, NULL}, {NULL,NULL, NULL, NULL} },
									{ {0,8,2,10},{12,4,14,6},{3,11,1,9},{15,7,13,5} }
	};

	// Get the depth of the gray image
	depth = getDeptFromFile(orgFname);

	// Calculate the threshold
	threshold = (depth / (k * k));

	// Create new bw image from the data of the gray image
	bwImage = createGrayImage(grayImg->rows, grayImg->cols);

	// Convert every pixel in the gray image to BW acordding to dithering's algorithm (with the given k) and print it
	for (int i = 0; i < grayImg->rows; i++)
	{
		for (int j = 0; j < grayImg->cols; j++)
		{
			unsigned char numByPart;

			// Find for the current pixel the number it belong to
			numByPart = (grayImg->pixels[i][j] / threshold);

			// Compere the number in the k matrix to the part number
			if (numByPart > mask[k - 2][i % k][j % k])
			{
				bwImage->pixels[i][j] = WHITE;
			}
			else
			{
				bwImage->pixels[i][j] = BLACK;
			}
		}
	}

	// Create name for the new BW file from PGM
	fnamek = createBWNameFromPGM(orgFname, k);

	// Create a new PGM file with the bw image's data 
	createPGMFile(fnamek, bwImage, BW_DEPTH, magicNumber);

	free(fnamek);
	freeGrayImage(bwImage);
}

// Return the average of the given RGB
unsigned char colorToGreyPixel(RGB colorPixel)
{
	return ((colorPixel.r + colorPixel.g + colorPixel.b) / RGB_NUM);
}

// Create name for a BW file from PGM file's name
// Dosent free the new name that is created! Must happend after use.
char* createBWNameFromPGM(char* fname, int k)
{
	char* res;
	unsigned int lenFname;

	lenFname = strlen(fname);

	// Size of the original name, "bw", k and \0
	res = (char*)malloc(sizeof(char) * (lenFname + 4));
	mallocFail(res);

	// Short the given name to be without the .pgm
	fname[lenFname - 4] = END_STR;

	// Update the name to be the same as the given name with bw and the given k
	sprintf(res, "%sbw%d.pgm", fname, k);
	res[lenFname + 3] = END_STR;

	// Return the name to original 
	fname[lenFname - 4] = '.';

	return res;
}

// Create name for a PGM file from PMM file's name
// Dosent free the new name that is created! Must happend after use.
char* createPGMNameFromPPM(char* fname)
{
	char* res;
	unsigned int lenFname;

	lenFname = strlen(fname);

	// Create name for the new file PGM
	res = (char*)malloc(sizeof(char) * (lenFname + 1));
	mallocFail(res);

	// Update the name to be the same as the given name with differnt end
	sprintf(res, "%s", fname);
	fname[lenFname] = END_STR;

	res[lenFname - 2] = 'g';

	return res;
}

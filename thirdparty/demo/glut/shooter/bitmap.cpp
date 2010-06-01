#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"


/* 
	This function writes out an 8-bit Windows bitmap file that is readable by Microsoft Paint.  
   The image has an arbitrary palette, consisting of up to 256 unique colors.  The image data
	consists of values that index into the palette.
  
   The input to the function is:
	   char *filename:						A string representing the filename that will be written
		unsigned int width:					The width, in pixels, of the bitmap
		unsigned int height:					The height, in pixels, of the bitmap
		unsigned char *image:					The image data, where the value indicates an index into the palette
		unsigned int numPaletteEntries	The number of entries used in the palette
		RGBQUAD *palette:						The palette

   Written by Greg Slabaugh (slabaugh@ece.gatech.edu), 10/19/00
*/
int write8BitBmpFile(char *filename, unsigned int width, unsigned int height, 
							unsigned char *image, unsigned int numPaletteEntries, RGBQUAD *palette)
{
	BITMAPINFOHEADER bmpInfoHeader;
	BITMAPFILEHEADER bmpFileHeader;
	FILE *filep;
	unsigned int row;
	unsigned int extrabytes, bytesize;
	unsigned char *paddedImage = NULL;

	/* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 8 - bit bitmaps,
	   this means that the width of the bitmap must be a multiple of 4. This code determines
	   the extra padding needed to meet this requirement. */
   extrabytes = (4 - width % 4) % 4;

	// This is the size of the padded bitmap
	bytesize = (width + extrabytes) * height;

	// Fill the bitmap file header structure
	bmpFileHeader.bfType = 'MB';   // Bitmap header
	bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * numPaletteEntries;

	// Fill the bitmap info structure
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 8;            // 8 - bit bitmap
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bytesize;     // includes padding for 4 byte alignment
	bmpInfoHeader.biXPelsPerMeter = 2952;
	bmpInfoHeader.biYPelsPerMeter = 2952;
	bmpInfoHeader.biClrUsed = numPaletteEntries;
	bmpInfoHeader.biClrImportant = 0;


	// Open file
	if ((filep = fopen(filename, "wb")) == NULL) {
		printf("Error opening file %s\n", filename);
		return FALSE;
	}

	// Write bmp file header
	if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), filep) < sizeof(BITMAPFILEHEADER)) {
		printf("Error writing bitmap file header\n");
		fclose(filep);
		return FALSE;
	}

	// Write bmp info header
	if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), filep) < sizeof(BITMAPINFOHEADER)) {
		printf("Error writing bitmap info header\n");
		fclose(filep);
		return FALSE;
	}

	// Write bmp palette
	if (fwrite(palette, 1, numPaletteEntries * sizeof(RGBQUAD), filep) < numPaletteEntries * sizeof(RGBQUAD)) {
		printf("Error writing bitmap palette\n");
		fclose(filep);
		return FALSE;
	}

	// Allocate memory for some temporary storage
	paddedImage = (unsigned char *)calloc(sizeof(unsigned char), bytesize);
	if (paddedImage == NULL) {
		printf("Error allocating memory \n");
		fclose(filep);
		return FALSE;
	}

	/* Flip image - bmp format is upside down.  Also pad the paddedImage array so that the number
	   of pixels is aligned on a 4 byte boundary. */
	for (row = 0; row < height; row++)
		memcpy(&paddedImage[row * (width + extrabytes)], &image[(height - 1 - row) * width], width);
	
	// Write bmp data
	if (fwrite(paddedImage, 1, bytesize, filep) < bytesize) {
		printf("Error writing bitmap data\n");
		free(paddedImage);
		fclose(filep);
		return FALSE;
	}

	// Close file
	fclose(filep);
	free(paddedImage);
	return TRUE;
}

/* 
	This function writes out a 24-bit Windows bitmap file that is readable by Microsoft Paint.  
   The image data	is a 1D array of (r, g, b) triples, where individual (r, g, b) values can 
	each take on values between 0 and 255, inclusive.
  
   The input to the function is:
	   char *filename:						A string representing the filename that will be written
		unsigned int width:					The width, in pixels, of the bitmap
		unsigned int height:					The height, in pixels, of the bitmap
		unsigned char *image:				The image data, where each pixel is 3 unsigned chars (r, g, b)

   Written by Greg Slabaugh (slabaugh@ece.gatech.edu), 10/19/00
*/
int write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image)
{
	BITMAPINFOHEADER bmpInfoHeader;
	BITMAPFILEHEADER bmpFileHeader;
	FILE *filep;
	unsigned int row, column;
	unsigned int extrabytes, bytesize;
	unsigned char *paddedImage = NULL, *paddedImagePtr, *imagePtr;

	/* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps,
	   this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines
	   the extra padding needed to meet this requirement. */
   extrabytes = (4 - (width * 3) % 4) % 4;

	// This is the size of the padded bitmap
	bytesize = (width * 3 + extrabytes) * height;

	// Fill the bitmap file header structure
	bmpFileHeader.bfType = 'MB';   // Bitmap header
	bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Fill the bitmap info structure
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;            // 8 - bit bitmap
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bytesize;     // includes padding for 4 byte alignment
	bmpInfoHeader.biXPelsPerMeter = 2952;
	bmpInfoHeader.biYPelsPerMeter = 2952;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;


	// Open file
	if ((filep = fopen(filename, "wb")) == NULL) {
		printf("Error opening file %s\n", filename);
		return FALSE;
	}

	// Write bmp file header
	if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), filep) < sizeof(BITMAPFILEHEADER)) {
		printf("Error writing bitmap file header\n");
		fclose(filep);
		return FALSE;
	}

	// Write bmp info header
	if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), filep) < sizeof(BITMAPINFOHEADER)) {
		printf("Error writing bitmap info header\n");
		fclose(filep);
		return FALSE;
	}

	// Allocate memory for some temporary storage
	paddedImage = (unsigned char *)calloc(sizeof(unsigned char), bytesize);
	if (paddedImage == NULL) {
		printf("Error allocating memory \n");
		fclose(filep);
		return FALSE;
	}

	/* This code does three things.  First, it flips the image data upside down, as the .bmp
	   format requires an upside down image.  Second, it pads the image data with extrabytes 
		number of bytes so that the width in bytes of the image data that is written to the
		file is a multiple of 4.  Finally, it swaps (r, g, b) for (b, g, r).  This is another
		quirk of the .bmp file format. */
	for (row = 0; row < height; row++) {
		imagePtr = image + (height - 1 - row) * width * 3;
		paddedImagePtr = paddedImage + row * (width * 3 + extrabytes);
		for (column = 0; column < width; column++) {
			*paddedImagePtr = *(imagePtr + 2);
			*(paddedImagePtr + 1) = *(imagePtr + 1);
			*(paddedImagePtr + 2) = *imagePtr;
			imagePtr += 3;
			paddedImagePtr += 3;
		}
	}

	// Write bmp data
	if (fwrite(paddedImage, 1, bytesize, filep) < bytesize) {
		printf("Error writing bitmap data\n");
		free(paddedImage);
		fclose(filep);
		return FALSE;
	}

	// Close file
	fclose(filep);
	free(paddedImage);
	return TRUE;
}


/* 
	This function writes out a grayscale image as a 8-bit Windows bitmap file that is readable by Microsoft Paint.
	It creates a palette and then calls write8BitBmpFile to output the bitmap file.
  
   The input to the function is:
	   char *filename:						A string representing the filename that will be written
		unsigned int width:					The width, in pixels, of the bitmap
		unsigned int height:					The height, in pixels, of the bitmap
		unsigned char *image:				The image data, where the value indicates a color between 0 (black) and 255 (white)


   Written by Greg Slabaugh (slabaugh@ece.gatech.edu), 10/19/00
*/
int writeGrayScaleDataToBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image)
{
	RGBQUAD palette[256];
	unsigned int i;
   unsigned int numPaletteEntries = 256;

   // Create the palette - each pixel is an index into the palette
	for (i = 0; i < numPaletteEntries; i++) {
		palette[i].rgbRed = i;
		palette[i].rgbGreen = i;
		palette[i].rgbBlue = i;
		palette[i].rgbReserved = 0;
	}

   return write8BitBmpFile(filename, width, height, image, numPaletteEntries, palette);
	
}




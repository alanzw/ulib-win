/*
	This set of bmp writing functions was:
	Written by Greg Slabaugh (slabaugh@ece.gatech.edu), 10/19/00
*/

#ifndef _BITMAP_H_
#define _BITMAP_H_

int write8BitBmpFile(char *filename, unsigned int width, unsigned int height, 
							unsigned char *image, unsigned int numPaletteEntries, RGBQUAD *palette);

int write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image);

int writeGrayScaleDataToBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image);


#endif

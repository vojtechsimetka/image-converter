/*
 *  File name: bmp.c
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Source file
 *  Description: Include functions to create BMP file
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <inttypes.h>
#include "constant.h"
#include "gif2bmp.h"


/**
 * Create and write BITMAPFILEHEADER and BITMAPINFOHEADER into output file
 *
 * @param bmpFile File pointer
 * @param pic Input image property
 * @return 0 on success, 1 on failure
 */
u_int8_t writeBmpHeaders(FILE *bmpFile, tPIC_PROPERTY *pic) {

	int i;

	/* BITMAPFILEHEADER */

	u_int8_t c = 0x42;	// ASCI B
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x4d;			// ASCI M
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	// File size will be set next time
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	// Reserved bytes should by zero
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	// Offset 54 B
	c = 0x36;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	c = 0x00;
	for (i = 0; i < 3; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	/* BITMAPINFOHEADER */

	// Struct size 40B
	c = 0x28;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	for (i = 0; i < 3; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	// Picture width in pix
	c = pic->widthInPixLowByte;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = pic->widthInPixHighByte;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	// Picture height in pix
	c = pic->heightInPixLowByte;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = pic->heightInPixHighByte;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	// BitPlanes - always 1
	c = 0x01;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	// Bits per pixel - 256 colors
	c = 0x18;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;
	c = 0x00;
	if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
		return EXIT_FAILURE;

	// No compression
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	// Image size in bytes - optional
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}


	// biXPelsPerMeter - optional
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	//biYPelsPerMeter - optional
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	//biClrUsed - optional
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	//biClrImportant - all colors are important
	c = 0x00;
	for (i = 0; i < 4; i++) {
		if (fwrite(&c, 1, sizeof c, bmpFile) != (sizeof c))
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/**
 * Get BMP file size in bytes and save it into BMP header value
 *
 * @param bmpFile File pointer
 * @param logStruc Struct for transformation - save bmp file size for log file
 * @return 0 on success, 1 on failure
 */
u_int8_t setBMPFileSize(FILE *bmpFile, tGIF2BMP *logStruc) {

	// Get file size in bytes
	fseek(bmpFile, 0, SEEK_SET);
	logStruc->bmpSize = getFileSize(bmpFile);
	u_int32_t fileSize = (u_int32_t)logStruc->bmpSize;
	fseek(bmpFile, 2, SEEK_SET);

	// Split file size into 4 B and save it into BMP header
	u_int8_t byte = (u_int8_t)(fileSize & 0x000000FF);

	if (fwrite(&byte, 1, sizeof byte, bmpFile) != (sizeof byte))
		return EXIT_FAILURE;

	byte = (u_int8_t)((fileSize & 0x0000FF00) >> 8);

	if (fwrite(&byte, 1, sizeof byte, bmpFile) != (sizeof byte))
		return EXIT_FAILURE;

	byte = (u_int8_t)((fileSize & 0x00FF0000) >> 16);

	if (fwrite(&byte, 1, sizeof byte, bmpFile) != (sizeof byte))
		return EXIT_FAILURE;

	byte = (u_int8_t)((fileSize & 0xFF000000) >> 24);

	if (fwrite(&byte, 1, sizeof byte, bmpFile) != (sizeof byte))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

/**
 * Write color buffer into BMP file
 *
 * @param bmpFile File pointer
 * @param bmpBuffer	Two dimension color buffer
 * @param pic Input image property
 * @return 0 on success, 1 on failure
 */
u_int8_t writeBmpData(FILE *bmpFile, tBGR **bmpBuffer, tPIC_PROPERTY *pic) {

	int j;
	int moduloRet = 0;
	u_int8_t color;

	// Get buffer size
	u_int32_t nrows = pic->heightInPixHighByte*256 + pic->heightInPixLowByte;
	u_int32_t ncolumns = pic->widthInPixHighByte*256 + pic->widthInPixLowByte;

	// Write buffer into file - pixel format BGR
	for (int i = (nrows - 1); i >= 0; i--) {
		for (j = 0; j < ncolumns; j++) {

			color = bmpBuffer[i][j].blue;
			if (fwrite(&color, 1, sizeof color, bmpFile) != (sizeof color))
				return EXIT_FAILURE;

			color = bmpBuffer[i][j].green;
			if (fwrite(&color, 1, sizeof color, bmpFile) != (sizeof color))
				return EXIT_FAILURE;

			color = bmpBuffer[i][j].red;
			if (fwrite(&color, 1, sizeof color, bmpFile) != (sizeof color))
				return EXIT_FAILURE;
		}
		// Line alignment to 4B
		moduloRet = ncolumns % 4;
		if (moduloRet != 0) {
			for (int tmp = 0; tmp < moduloRet; tmp++) {
				color = 0x00;
				if (fwrite(&color, 1, sizeof color, bmpFile) != (sizeof color))
					return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
}


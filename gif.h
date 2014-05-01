/*
 * gif.h
 *
 *  Created on: 15. 3. 2014
 *      Author: Adam
 */

#include "constant.h"

#ifndef GIF_H_
#define GIF_H_

int checkGifVersion(FILE *gifFile);
int parseGifHeader(FILE *gifFile, tPIC_PROPERTY *pic);
int getColorTable(FILE *gifFile, tRGB globalColorTable [], int colorTableSize);
int getGraphicControlExt(FILE *inputFile);
int getPlainTextExt(FILE *inputFile);
int getApplicationExt(FILE *inputFile);
int getCommentExt(FILE *inputFile);
int getImageDescriptor(FILE *inputFile, tIMAGE_DESCRIPTOR *imageDescriptor);
void processColorList(tDIC_ITEM *list, tBGR **bmpOutputBuffer, tRGB *colorTable, tBMPWRITER *bmpWriter);
void processColor(int color, tBGR **bmpOutputBuffer, tRGB *colorTable, tBMPWRITER *bmpWriter);
int getImageData(FILE *inputFile, FILE *outputFile, tGIFREADER *reader, tBGR **bitMap, 	tBMPWRITER *bmpWriter);

#endif /* GIF_H_ */

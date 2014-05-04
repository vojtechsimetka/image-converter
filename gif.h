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
void processColorList(tDIC_ITEM *list, Mat &bitMap, tRGB *colorTable, tBITMAPWRITER *bitMapWriter);
void processColor(int color, Mat &bitMap, tRGB *colorTable, tBITMAPWRITER *bitMapWriter);
int getImageData(FILE *inputFile, tGIFREADER *reader, Mat &bitMap, tBITMAPWRITER *bitMapWriter);

#endif /* GIF_H_ */

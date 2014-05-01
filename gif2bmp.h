/*
 * gif2bmp.h
 *
 *  Created on: 15. 3. 2014
 *      Author: Adam
 */

#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <cv.h>
#include <iostream>
#include "constant.h"
#include "dictionary.h"

using namespace std;
using namespace cv;

#ifndef GIF2BMP_H_
#define GIF2BMP_H_

u_int8_t readByteFromFile(FILE *ptr_file, u_int8_t *readedByte);
u_int8_t writeByteToFile(FILE *ptr_file, u_int8_t *Byte);
u_int8_t writeByteToFileOffset(FILE *ptr_file, u_int8_t *Byte, int offset);
u_int8_t readBitsStreamFromFile (FILE *gifFile, tGIFREADER *reader, u_int32_t *readedBits, u_int8_t subBlockState);
u_int8_t reverseByte(u_int8_t byte);
int64_t getFileSize(FILE *file);
int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile);
int createLogFile(tGIF2BMP *gif2bmp, char *logFileName);
void dealocateBitMapBuffer (tBGR **bitMap, u_int32_t nrows, u_int32_t ncolumns);
u_int8_t readStdInIntoBuffer(u_int8_t *buffer);
cv::Mat loadGif(const string &filename);


#endif /* GIF2BMP_H_ */

/*
 *  File name: gif2bmp.h
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Header file
 *  Description: Include functions definitions to convert gif 2 bmp
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
cv::Mat gif2bmp(FILE *inputFile);
u_int8_t readStdInIntoBuffer(u_int8_t *buffer);
cv::Mat loadGif(const string &filename);


#endif /* GIF2BMP_H_ */

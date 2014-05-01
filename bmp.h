/*
 *  File name: bmp.h
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Header file
 *  Description: Include functions prototypes to create BMP file
 */

#ifndef BMP_H_
#define BMP_H_

u_int8_t writeBmpHeaders(FILE *bmpFile, tPIC_PROPERTY *pic);
u_int8_t setBMPFileSize(FILE *bmpFile, tGIF2BMP *logStruc);
u_int8_t writeBmpData(FILE *bmpFile, tBGR **colorTable, tPIC_PROPERTY *pic);

#endif /* BMP_H_ */

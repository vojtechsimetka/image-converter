/*
 *  File name: constant.h
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Header file
 *  Description: Include constants and data types declaration for gif2bmp library
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <sys/types.h>
#include <stdint.h>

#define READ_WRITE_OK  						0
#define READ_WRITE_ERR 						1
#define END_OF_FILE 						2

#define LOGICAL_SCREEN_DESCRIPTOR_SIZE 		7

#define IMAGE_DESCRIPTOR_INTRODUCER 		0x2C
#define IMAGE_DESCRIPTOR_SIZE 				10

#define GRAPHICS_CONTROL_INTRODUCER		 	0x21
#define GRAPHICS_CONTROL_LABEL			 	0xF9
#define GRAPHICS_CONTROL_EXTENSION_SIZE 	4

#define COMMENT_EXTENSION_INTRODUCER		0x21
#define COMMENT_EXTENSION_LABEL			 	0xFE

#define APPLICATION_EXTENSION_INTRODUCER	0x21
#define APPLICATION_EXTENSION_LABEL			0xFF

#define PLAINTEXT_EXTENSION_INTRODUCER		0x21
#define PLAINTEXT_EXTENSION_LABEL			0x01

#define EXTENSION_INTRODUCER				0x21
#define BLOCK_TERMINATOR 					0x00
#define GIF_END_OF_FILE 					0x3B

#define _2_BIT_MASK  						0xC0000000
#define _3_BIT_MASK  						0xE0000000
#define _4_BIT_MASK  						0xF0000000
#define _5_BIT_MASK  						0xF8000000
#define _6_BIT_MASK  						0xFC000000
#define _7_BIT_MASK  						0xFE000000
#define _8_BIT_MASK  						0xFF000000
#define _9_BIT_MASK  						0xFF800000
#define _10_BIT_MASK 						0xFFC00000
#define _11_BIT_MASK 						0xFFE00000
#define _12_BIT_MASK 						0xFFF00000
#define _13_BIT_MASK 						0xFFF80000
#define NEW_SUBBLOCK 						0
#define UNFINISHED_SUBBLOCK 				1
#define FINISHING_SUBBLOCK 				    2

#define _2_BITS_MAX_CODE 					3
#define _3_BITS_MAX_CODE 					7
#define _4_BITS_MAX_CODE 					15
#define _5_BITS_MAX_CODE 					31
#define _6_BITS_MAX_CODE 					63
#define _7_BITS_MAX_CODE 					127
#define _8_BITS_MAX_CODE 					255
#define _9_BITS_MAX_CODE 					511
#define _10_BITS_MAX_CODE 					1023
#define _11_BITS_MAX_CODE 					2047
#define _12_BITS_MAX_CODE 					4095
#define DICTIONARY_MAX_SIZE 				4097
#define DICTIONARY_FULL		 				-1
#define NUMBER_OF_COLORS		 			256

#define GIFMASK_LOCAL_COLOR_PALETTE    		0x80
#define GIFMASK_LOCAL_COLOR_PALETTE_SIZE    0x07
#define GIFMASK_LOCAL_COLOR_PALETTE_SORT    0x20
#define GIFMASK_LOCAL_INTERLACE			    0x40

#define GIFMASK_GLOBAL_COLOR_PALETTE    	0x80
#define GIFMASK_COLOR_BITS_PER_PIXEL    	0x70
#define GIFMASK_COLOR_PALETTE_SORT      	0x08
#define GIFMASK_GLOBAL_COLOR_PALETTE_SIZE   0x07
#define GIF_MAX_CODE_WORD_LENGTH_IN_BITS    12

#define M_EXIT_FAILURE 						-1

/**
 * @brief GIF pixel structure
 */

typedef struct {
	u_int8_t red;
	u_int8_t green;
	u_int8_t blue;
} tRGB;

/**
 * @brief BMP pixel structure
 */
typedef struct {
	u_int8_t blue;
	u_int8_t green;
	u_int8_t red;
} tBGR;

/**
 * @brief Dictionary list item
 */
typedef struct colorListItem {
	int colorTableIndex;
	struct colorListItem *nextColor;
} tLIST_ITEM;

/**
 * @brief Dictionary item
 */
typedef struct colorsList {
	tLIST_ITEM *first;
	tLIST_ITEM *last;
} tDIC_ITEM;

/**
 * @brief Dictionary
 */
typedef struct {
	tDIC_ITEM colors[DICTIONARY_MAX_SIZE];
	int clearCode;
	int endOfInformationCode;
	u_int32_t previousCode;
	int firstEmptyCode;
	int curMaxCode;
} tDICTIONARY;

/**
 * @brief GIF reader struct
 */
typedef struct{
	u_int8_t Byte1;
	u_int8_t Byte2;
	u_int8_t Byte3;
	u_int8_t Byte1ValidFlag;
	u_int8_t Byte2ValidFlag;
	u_int8_t Byte3ValidFlag;
	u_int32_t dataBlockSize;
	int usedBitsFromByte1LastTime;
	int lzwSize;
	int initLzwSize;
	u_int8_t BytesToReadInSubBlock;
	int activeColorTableSize;
	tRGB *activeColorTable;
} tGIFREADER;

/**
 * @brief BMP writer struct
 */
typedef struct{
	u_int32_t actualRow;
	u_int32_t actualColumn;
	u_int16_t actualHeight;
	u_int16_t actualWidth;
	u_int32_t actualX;
	u_int32_t actualY;
} tBITMAPWRITER;

/**
 * @brief Conversion property struct
 */
typedef struct{
	int64_t bmpSize;

	int64_t gifSize;
} tGIF2BMP;

/**
 * @brief Input picture property struct
 */
typedef struct{
	u_int8_t widthInPixHighByte;
	u_int8_t widthInPixLowByte;
	u_int8_t heightInPixHighByte;
	u_int8_t heightInPixLowByte;
	u_int8_t globalColorTable;
	u_int8_t bitsPerPixel;
	u_int8_t colorTableSorted;
	int colorTableLong;
	int backgroundColor;
	u_int8_t pixelAspectRatio;
} tPIC_PROPERTY;

/**
 * @brief Image descriptor struct
 */
typedef struct{
	u_int8_t leftPosHighByte;
	u_int8_t leftPosLowByte;
	u_int8_t topPosHighByte;
	u_int8_t topPosLowByte;
	u_int8_t widthHighByte;
	u_int8_t widthLowByte;
	u_int8_t heightHighByte;
	u_int8_t heightLowByte;
	u_int8_t localColorTableFlag;
	u_int8_t interlaceFlag;
	u_int8_t sortFlag;
	int localColorTableSize;
	u_int32_t sizeInPixels;
} tIMAGE_DESCRIPTOR;


#endif /* CONSTANT_H_ */

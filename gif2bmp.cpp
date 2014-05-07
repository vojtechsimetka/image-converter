/*
 *  File name: gif2bmp.c
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Source file
 *  Description: Include functions for gif decompression
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <errno.h>
#include "gif2bmp.h"
#include "gif.h"
#include "bmp.h"
#include "constant.h"

/**
 * Function read byte from input file
 *
 * @param ptr_file Pointer to input file
 * @param Byte Pointer to byte buffer 
 * @return status code READ_WRITE_OK on success, READ_WRITE_ERR on failure and END_OF_FILE when EOF is readed
 */
u_int8_t readByteFromFile(FILE *ptr_file, u_int8_t *Byte){

	// Read byte from input file
	if (fread (Byte, sizeof(u_int8_t), 1, ptr_file)) {
		return READ_WRITE_OK;
	}
	else {
		if (ferror(ptr_file))
			return READ_WRITE_ERR;
		else
			return END_OF_FILE;
	}
}

/**
 * Function reverse bits in byte
 *
 * @param byte Byte to reverse
 * @return reversed byte
 */

u_int8_t reverseByte(u_int8_t byte)
{
    int i;
    u_int8_t tmp = 0;

    for ( i = 0 ; i < 8 ; i ++)
    {
    	tmp <<= 1;
    	tmp |=  ( (byte & (1 << i)) >> i);
    }
    return tmp;
}

/**
 * Function reverse bits in double world
 *
 * @param dword Souble world to reverse
 * @return reversed double world
 */
u_int32_t reverseDWord(u_int32_t dword)
{
    int i;
    u_int32_t tmp = 0;

    for ( i = 0 ; i < 32 ; i ++)
    {
    	tmp <<= 1;
    	tmp |=  ( (dword & (1 << i)) >> i);
    }
    return tmp;
}

/**
 * Function read n (lzw size) bits from input file
 *
 * @param gifFile Pointer to input file
 * @param reader Reader struct
 * @param readedBits Pointer to readed value buffer
 * @param u_int8_t Current block status
 * @return status code EXIT_FAILURE on failure and EXIT_SUCCESS on success
 */
u_int8_t readBitsStreamFromFile (FILE *gifFile, tGIFREADER *reader, u_int32_t *readedBits, u_int8_t subBlockState) {

	u_int8_t readRetVal;

	// Init return value
	*readedBits = 0;

	// Init reader - read bytes to internal buffer

	if (reader->usedBitsFromByte1LastTime == -1) {
		reader->usedBitsFromByte1LastTime = 0;

		// Byte 1
		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte1);
			reader->BytesToReadInSubBlock--;
			reader->Byte1ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte1ValidFlag = 0;

		// Byte 2
		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte2);
			reader->BytesToReadInSubBlock--;
			reader->Byte2ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte2ValidFlag = 0;

		// Byte 3
		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte3);
			reader->BytesToReadInSubBlock--;
			reader->Byte3ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte3ValidFlag = 0;
	}

	// Start new subblock, read missing bytes
	if (subBlockState == NEW_SUBBLOCK) {
		if (reader->Byte1ValidFlag != 1) {
			if (reader->BytesToReadInSubBlock != 0) {
				readRetVal = readByteFromFile(gifFile, &reader->Byte1);
				reader->BytesToReadInSubBlock--;
				reader->Byte1ValidFlag = 1;
				if (readRetVal == READ_WRITE_ERR) {
					fprintf(stderr, "%s", "Can not read input file.");
					return EXIT_FAILURE;
				}
				else if (readRetVal == END_OF_FILE) {
					fprintf(stderr, "%s", "Incorrect input file.");
					return EXIT_FAILURE;
				}
			}
		}
		else
			reader->Byte1ValidFlag = 0;
		if (reader->Byte2ValidFlag != 1) {
			if (reader->BytesToReadInSubBlock != 0) {
				readRetVal = readByteFromFile(gifFile, &reader->Byte2);
				reader->BytesToReadInSubBlock--;
				reader->Byte2ValidFlag = 1;
				if (readRetVal == READ_WRITE_ERR) {
					fprintf(stderr, "%s", "Can not read input file.");
					return EXIT_FAILURE;
				}
				else if (readRetVal == END_OF_FILE) {
					fprintf(stderr, "%s", "Incorrect input file.");
					return EXIT_FAILURE;
				}
			}
		}
		else
			reader->Byte2ValidFlag = 0;

		if (reader->Byte3ValidFlag != 1) {
			if (reader->BytesToReadInSubBlock != 0) {
				readRetVal = readByteFromFile(gifFile, &reader->Byte3);
				reader->BytesToReadInSubBlock--;
				reader->Byte3ValidFlag = 1;
				if (readRetVal == READ_WRITE_ERR) {
					fprintf(stderr, "%s", "Can not read input file.");
					return EXIT_FAILURE;
				}
				else if (readRetVal == END_OF_FILE) {
					fprintf(stderr, "%s", "Incorrect input file.");
					return EXIT_FAILURE;
				}
			}
		}
		else
			reader->Byte3ValidFlag = 0;
	}

	// Create value from bytes
	(*readedBits) = (((u_int32_t)reverseByte(reader->Byte1)) << (24 + reader->usedBitsFromByte1LastTime));
	(*readedBits) = (*readedBits) | (((u_int32_t)reverseByte(reader->Byte2)) << (16 + reader->usedBitsFromByte1LastTime));
	(*readedBits) = (*readedBits) | (((u_int32_t)reverseByte(reader->Byte3)) << (8 + reader->usedBitsFromByte1LastTime));

	// Update used bits from first and second internal buffered bytes and read new values to internal buffer

	int bitsPerBytes2_3 = reader->lzwSize + reader->usedBitsFromByte1LastTime - 8;
	if (bitsPerBytes2_3 >= 8) {

		reader->usedBitsFromByte1LastTime = bitsPerBytes2_3 - 8;
		reader->Byte1 = reader->Byte3;
		reader->Byte1ValidFlag = reader->Byte3ValidFlag;

		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte2);
			reader->BytesToReadInSubBlock--;
			reader->Byte2ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte2ValidFlag = 0;

		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte3);
			reader->BytesToReadInSubBlock--;
			reader->Byte3ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte3ValidFlag = 0;
	}
	// Get only one new byte
	else if(bitsPerBytes2_3 > 0) {
		reader->usedBitsFromByte1LastTime = bitsPerBytes2_3;
		reader->Byte1 = reader->Byte2;
		reader->Byte1ValidFlag = reader->Byte2ValidFlag;
		reader->Byte2 = reader->Byte3;
		reader->Byte2ValidFlag = reader->Byte3ValidFlag;

		if (reader->BytesToReadInSubBlock != 0) {
			readRetVal = readByteFromFile(gifFile, &reader->Byte3);
			reader->BytesToReadInSubBlock--;
			reader->Byte3ValidFlag = 1;
			if (readRetVal == READ_WRITE_ERR) {
				fprintf(stderr, "%s", "Can not read input file.");
				return EXIT_FAILURE;
			}
			else if (readRetVal == END_OF_FILE) {
				fprintf(stderr, "%s", "Incorrect input file.");
				return EXIT_FAILURE;
			}
		}
		else
			reader->Byte3ValidFlag = 0;
	}
	else {
		if(reader->lzwSize == 8) {
			reader->usedBitsFromByte1LastTime = bitsPerBytes2_3;
			reader->Byte1 = reader->Byte2;
			reader->Byte1ValidFlag = reader->Byte2ValidFlag;
			reader->Byte2 = reader->Byte3;
			reader->Byte2ValidFlag = reader->Byte3ValidFlag;

			if (reader->BytesToReadInSubBlock != 0) {
				readRetVal = readByteFromFile(gifFile, &reader->Byte3);
				reader->BytesToReadInSubBlock--;
				reader->Byte3ValidFlag = 1;
				if (readRetVal == READ_WRITE_ERR) {
					fprintf(stderr, "%s", "Can not read input file.");
					return EXIT_FAILURE;
				}
				else if (readRetVal == END_OF_FILE) {
					fprintf(stderr, "%s", "Incorrect input file.");
					return EXIT_FAILURE;
				}
			}
			else
				reader->Byte3ValidFlag = 0;
		}
	}

	// Apply current mask, whitch is set by current lzw size to readed value 
	switch (reader->lzwSize) {
		case(2):
				(*readedBits) = (reverseDWord(*readedBits & _2_BIT_MASK));
				break;
		case(3):
				(*readedBits) = (reverseDWord(*readedBits & _3_BIT_MASK));
				break;
		case(4):
				(*readedBits) = (reverseDWord(*readedBits & _4_BIT_MASK));
				break;
		case(5):
				(*readedBits) = (reverseDWord(*readedBits & _5_BIT_MASK));
				break;
		case(6):
				(*readedBits) = (reverseDWord(*readedBits & _6_BIT_MASK));
				break;
		case(7):
				(*readedBits) = (reverseDWord(*readedBits & _7_BIT_MASK));
				break;
		case(8):
				(*readedBits) = (reverseDWord(*readedBits & _8_BIT_MASK));
				break;
		case(9):
				(*readedBits) = (reverseDWord(*readedBits & _9_BIT_MASK));
				break;
		case(10):
				(*readedBits) = (reverseDWord(*readedBits & _10_BIT_MASK));
				break;
		case(11):
				(*readedBits) = (reverseDWord(*readedBits & _11_BIT_MASK));
				break;
		case(12):
				(*readedBits) = (reverseDWord(*readedBits & _12_BIT_MASK));
				break;
		case(13):
				(*readedBits) = (reverseDWord(*readedBits & _13_BIT_MASK));
				break;
		default:
				fprintf(stderr, "%s", "Decompression error.");
				break;

	}

	return EXIT_SUCCESS;
}

/**
 * Function save byte to file
 *
 * @param ptr_file Pointer to file
 * @param Byte Pointer to byte buffer 
 * @return status code READ_WRITE_OK on success and READ_WRITE_ERR on failure
 */
u_int8_t writeByteToFile(FILE *ptr_file, u_int8_t *Byte){

	// Write byte
	if (fwrite (Byte, sizeof(u_int8_t), sizeof(u_int8_t), ptr_file))
		return READ_WRITE_OK;
	else
		return READ_WRITE_ERR;
}

/**
 * Function save byte to file on offset address
 *
 * @param ptr_file Pointer to file
 * @param Byte Pointer to byte buffer 
 * @param offset Offset to file
 * @return status code READ_WRITE_OK on success and READ_WRITE_ERR on failure
 */
u_int8_t writeByteToFileOffset(FILE *ptr_file, u_int8_t *Byte, int offset){

	// Go to position and write byte
	fseek ( ptr_file , offset , SEEK_SET );
	if (!writeByteToFile(ptr_file, Byte))
		return READ_WRITE_OK;
	else
		return READ_WRITE_ERR;
}

/**
 * Function return image size in bytes
 *
 * @param file Pointer to file
 * @return Image size in bytes
 */
int64_t getFileSize(FILE *file) {
	fseek(file, 0, SEEK_END);
	int64_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

/**
 * Function init structures whitch is used for converion
 *
 * @param globalColorTable Global color table
 * @param localColorTable Local color table
 * @param reader Pointer to reader struct
 */
void initStructures (tRGB globalColorTable [], tRGB localColorTable [], tGIFREADER *reader) {

	// Init color tables
	for (int i = 0; i < 256; i++) {
		globalColorTable[i].blue = 0;
		globalColorTable[i].red = 0;
		globalColorTable[i].green = 0;
		localColorTable[i].blue = 0;
		localColorTable[i].red = 0;
		localColorTable[i].green = 0;
	}

	// Init reader struct
	reader->Byte1 = 0;
	reader->Byte2 = 0;
	reader->Byte3 = 0;
	reader->lzwSize = 8;
	reader->usedBitsFromByte1LastTime = -1;
	reader->BytesToReadInSubBlock = 0;
	reader->activeColorTable = NULL;
	reader->activeColorTableSize = 0;

}


/**
 * Function decode GIF89a
 *
 * @param inputFile Pointer to input file
 * @return color matrix of pixels
 */

cv::Mat gif2bmp(FILE *inputFile){

	tPIC_PROPERTY pic;
	tRGB globalColorTable [256];
	tRGB localColorTable [256];
	tIMAGE_DESCRIPTOR imageDescriptor;
	tGIFREADER reader;
	u_int8_t readRetVal = 0;
	u_int8_t Byte = 0;
    tBITMAPWRITER bitMapWriter;

	// Init used structures
	initStructures (globalColorTable, localColorTable, &reader);

	// Check gif version
	if (checkGifVersion(inputFile))
        throw;

	// Parse gif header
	if (parseGifHeader(inputFile, &pic))
        throw;

	// Get global table
	if (pic.globalColorTable) {
		reader.activeColorTableSize = (u_int8_t)pic.colorTableLong;
		if (getColorTable(inputFile, globalColorTable, pic.colorTableLong))
            throw;
	}

    // Init output matrix
    Mat bitMap(pic.heightInPixHighByte*256 + pic.heightInPixLowByte, pic.widthInPixHighByte*256 + pic.widthInPixLowByte, CV_8UC3, Scalar(255,255,255));

	// Get background color
	if (pic.backgroundColor != -1) {
        //bitMap = Scalar(globalColorTable[pic.backgroundColor].red, globalColorTable[pic.backgroundColor].green, globalColorTable[pic.backgroundColor].blue);
	}

	// Get gif body
	while (1) {

		// Get block introducer from gif
		readRetVal = readByteFromFile(inputFile, &Byte);
		if (readRetVal == READ_WRITE_ERR) {
            throw "Can not read input file.";
		}
		else if (readRetVal == END_OF_FILE) {
            throw "Incorrect gif file.";
		}
		else { // End of file
			if (Byte == GIF_END_OF_FILE) {
				break;
			}
			else if (Byte == EXTENSION_INTRODUCER) { // Get extension label
				readRetVal = readByteFromFile(inputFile, &Byte);
				if (readRetVal == READ_WRITE_ERR) {
                    throw "Can not read input file.";
				}
				else if (readRetVal == END_OF_FILE) {
                    throw "Incorrect gif file.";
				}
				else {
					switch (Byte) {
						case(APPLICATION_EXTENSION_LABEL):	// Application ext.
								if (getApplicationExt(inputFile)) {
                                    throw;
								}
								break;
						case(COMMENT_EXTENSION_LABEL):		// Comment ext.
								if (getCommentExt(inputFile)) {
                                    throw;
								}
								break;
						case(PLAINTEXT_EXTENSION_LABEL):	// Plain text ext.
								if (getPlainTextExt(inputFile)) {
                                    throw;
								}
								break;
						case(GRAPHICS_CONTROL_LABEL):		// Graphic control ext.
								if (getGraphicControlExt(inputFile)) {
                                    throw;
								}

								// Read next byte
								readRetVal = readByteFromFile(inputFile, &Byte);
								if (readRetVal == READ_WRITE_ERR) {
                                    throw "Can not read input file.";
								}
								else if (readRetVal == END_OF_FILE) {
                                    throw "Incorrect gif file.";
								}
								else {
									switch (Byte) {
										// Image descriptor
										case(IMAGE_DESCRIPTOR_INTRODUCER):

												// Read image descriptor
												if (getImageDescriptor(inputFile, &imageDescriptor)) {
                                                    throw;
												}

												// Init bmpWritter
                                                bitMapWriter.actualColumn = imageDescriptor.leftPosHighByte*256 + imageDescriptor.leftPosLowByte;
                                                bitMapWriter.actualRow = imageDescriptor.topPosHighByte*256 + imageDescriptor.topPosLowByte;
                                                bitMapWriter.actualWidth = imageDescriptor.widthHighByte*256 + imageDescriptor.widthLowByte;
                                                bitMapWriter.actualHeight = imageDescriptor.heightHighByte*256 + imageDescriptor.heightLowByte;
                                                bitMapWriter.actualX = bitMapWriter.actualColumn;
                                                bitMapWriter.actualY = bitMapWriter.actualRow;

												// Set and read color table
												if (imageDescriptor.localColorTableFlag) {
													reader.activeColorTable = localColorTable;
													reader.activeColorTableSize = imageDescriptor.localColorTableSize;
													if (getColorTable(inputFile, localColorTable, imageDescriptor.localColorTableSize)) {
                                                        throw;
													}
												}
												else {
													reader.activeColorTable = globalColorTable;
													reader.activeColorTableSize = pic.colorTableLong;
												}

												// Get image data
												reader.dataBlockSize = imageDescriptor.sizeInPixels;
                                                if (getImageData(inputFile, &reader, bitMap, &bitMapWriter)) {
                                                    throw;
												}

												break;

										// Plain text extension
										case(PLAINTEXT_EXTENSION_LABEL):

												// Get plain text extension
												if (getPlainTextExt(inputFile)) {
                                                    throw;
												}
												break;
										default:
                                            throw "Incorrect gif file.";
									}
								}

								break;
						default:
                            throw "Incorrect gif file.";
					}

				}
			}
			else if (Byte == IMAGE_DESCRIPTOR_INTRODUCER) {
				if (getImageDescriptor(inputFile, &imageDescriptor)) {
                    throw;
				}

				// Init bmpWritter
                bitMapWriter.actualColumn = imageDescriptor.leftPosHighByte*256 + imageDescriptor.leftPosLowByte;
                bitMapWriter.actualRow = imageDescriptor.topPosHighByte*256 + imageDescriptor.topPosLowByte;
                bitMapWriter.actualWidth = imageDescriptor.widthHighByte*256 + imageDescriptor.widthLowByte;
                bitMapWriter.actualHeight = imageDescriptor.heightHighByte*256 + imageDescriptor.heightLowByte;
                bitMapWriter.actualX = bitMapWriter.actualColumn;
                bitMapWriter.actualY = bitMapWriter.actualRow;

				// Set and read color table
				if (imageDescriptor.localColorTableFlag) {
					reader.activeColorTable = localColorTable;
					reader.activeColorTableSize = imageDescriptor.localColorTableSize;
					if (getColorTable(inputFile, localColorTable, imageDescriptor.localColorTableSize)) {
                        throw;
					}
				}
				else {
					reader.activeColorTable = globalColorTable;
					reader.activeColorTableSize = pic.globalColorTable;
				}

				reader.dataBlockSize = imageDescriptor.sizeInPixels;
                if (getImageData(inputFile, &reader, bitMap, &bitMapWriter)) {
                    throw;
				}

			}
			else {
                throw "Incorrect gif file.";
			}
		}
	}

    return bitMap;
}

cv::Mat loadGif(const string &filename)
{
    FILE *fgif = fopen(filename.c_str(), "rb");

    if (fgif == NULL)
        throw "Unable to open input file";

    Mat m = gif2bmp(fgif);

    fclose(fgif);

    return m;
}


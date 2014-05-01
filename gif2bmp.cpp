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

u_int8_t readByteFromFile(FILE *ptr_file, u_int8_t *Byte){

	if (fread (Byte, sizeof(u_int8_t), 1, ptr_file)) {
		return READ_WRITE_OK;
	}
	else {printf("%s\n", strerror(errno));
		if (ferror(ptr_file))
			return READ_WRITE_ERR;
		else
			return END_OF_FILE;
	}
}


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

u_int8_t readBitsStreamFromFile (FILE *gifFile, tGIFREADER *reader, u_int32_t *readedBits, u_int8_t subBlockState) {

	u_int8_t readRetVal;

	// Init return value
	*readedBits = 0;

	// Init reader
	if (reader->usedBitsFromByte1LastTime == -1) {
		reader->usedBitsFromByte1LastTime = 0;

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

	(*readedBits) = (((u_int32_t)reverseByte(reader->Byte1)) << (24 + reader->usedBitsFromByte1LastTime));
	(*readedBits) = (*readedBits) | (((u_int32_t)reverseByte(reader->Byte2)) << (16 + reader->usedBitsFromByte1LastTime));
	(*readedBits) = (*readedBits) | (((u_int32_t)reverseByte(reader->Byte3)) << (8 + reader->usedBitsFromByte1LastTime));


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

u_int8_t writeByteToFile(FILE *ptr_file, u_int8_t *Byte){

	if (fwrite (Byte, sizeof(u_int8_t), sizeof(u_int8_t), ptr_file))
		return READ_WRITE_OK;
	else
		return READ_WRITE_ERR;
}

u_int8_t writeByteToFileOffset(FILE *ptr_file, u_int8_t *Byte, int offset){

	fseek ( ptr_file , offset , SEEK_SET );
	if (!writeByteToFile(ptr_file, Byte))
		return READ_WRITE_OK;
	else
		return READ_WRITE_ERR;
}

int createLogFile(tGIF2BMP *logStruc, char *logFileName){

	if (logFileName == NULL)
		return EXIT_SUCCESS;

	FILE *ptr_log_file;

	ptr_log_file = fopen(logFileName, "w+");

	if (!ptr_log_file){
		fprintf(stderr, "%s", "Can not create output log file.");
		return M_EXIT_FAILURE;
	}

	char buffer[100];
	memset(buffer, 0, sizeof(buffer));

	int cx = snprintf ( buffer, 100, "login = xsirok07\nuncodedSize = %"PRId64"\ncodedSize = %"PRId64"\n", (*logStruc).gifSize, (*logStruc).bmpSize);
	if (cx < 0){
		fprintf(stderr, "%s", "Can not write into log file.");
		return M_EXIT_FAILURE;
	}

	fwrite (buffer , sizeof(char), strlen(buffer), ptr_log_file);

	fclose(ptr_log_file);

	return EXIT_SUCCESS;
}

int64_t getFileSize(FILE *file) {
	fseek(file, 0, SEEK_END);
	int64_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

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

void dealocateBitMapBuffer (tBGR **bitMap, u_int32_t nrows, u_int32_t ncolumns) {

	for(int i = 0; i < nrows; i++)
		free(bitMap[i]);

	free(bitMap);
}


int gif2bmp(tGIF2BMP *logStruc, FILE *inputFile, FILE *outputFile){

	tPIC_PROPERTY pic;
	tRGB globalColorTable [256];
	tRGB localColorTable [256];
	//tGRAPHIC_CONTROL_EXT graphicControlExt;
	tIMAGE_DESCRIPTOR imageDescriptor;
	tGIFREADER reader;
	u_int8_t readRetVal = 0;
	u_int8_t Byte = 0;
	tBGR **bitMap;
	tBMPWRITER bmpWriter;
	tRGB backgroundColor = {0, 0, 0};

	// Init used structures
	initStructures (globalColorTable, localColorTable, &reader);

	// Get gif file size
	logStruc->gifSize = getFileSize(inputFile);

	// Check gif version
	if (checkGifVersion(inputFile))
		return M_EXIT_FAILURE;

	// Parse gif header
	if (parseGifHeader(inputFile, &pic))
		return M_EXIT_FAILURE;

	// Get global table
	if (pic.globalColorTable) {
		reader.activeColorTableSize = (u_int8_t)pic.colorTableLong;
		if (getColorTable(inputFile, globalColorTable, pic.colorTableLong))
			return M_EXIT_FAILURE;
	}

	// Allocate bmp data buffer
	u_int32_t nrows = pic.heightInPixHighByte*256 + pic.heightInPixLowByte;
	u_int32_t ncolumns = pic.widthInPixHighByte*256 + pic.widthInPixLowByte;

	// Get background color
	if (pic.backgroundColor != -1) {
		backgroundColor = globalColorTable[pic.backgroundColor];
	}

    bitMap = (tBGR **)malloc(nrows * sizeof(tBGR *));
	if(bitMap == NULL) {
		fprintf(stderr, "Allocation error\n");
		return M_EXIT_FAILURE;
	}
	for(int i = 0; i < nrows; i++) {
        bitMap[i] = (tBGR *)malloc(ncolumns * sizeof(tBGR));
		if(bitMap[i] == NULL) {
			fprintf(stderr, "out of memory\n");
			return M_EXIT_FAILURE;
		}
		for (int j = 0; j < ncolumns; j++) {
			bitMap[i][j].blue = backgroundColor.blue;
			bitMap[i][j].red = backgroundColor.red;
			bitMap[i][j].green = backgroundColor.green;
		}
	}

	// Get gif body
	while (1) {

		// Get block introducer from gif
		readRetVal = readByteFromFile(inputFile, &Byte);
		if (readRetVal == READ_WRITE_ERR) {
			fprintf(stderr, "%s", "Can not read input file.");
			return M_EXIT_FAILURE;
		}
		else if (readRetVal == END_OF_FILE) {
			fprintf(stderr, "%s", "Incorrect gif file.");
			return M_EXIT_FAILURE;
		}
		else { // End of file
			if (Byte == GIF_END_OF_FILE) {
				break;
			}
			else if (Byte == EXTENSION_INTRODUCER) { // Get extension label
				readRetVal = readByteFromFile(inputFile, &Byte);
				if (readRetVal == READ_WRITE_ERR) {
					fprintf(stderr, "%s", "Can not read input file.");
					return M_EXIT_FAILURE;
				}
				else if (readRetVal == END_OF_FILE) {
					fprintf(stderr, "%s", "Incorrect gif file.");
					return M_EXIT_FAILURE;
				}
				else {
					switch (Byte) {
						case(APPLICATION_EXTENSION_LABEL):	// Application ext.
								if (getApplicationExt(inputFile))
									return M_EXIT_FAILURE;
								break;
						case(COMMENT_EXTENSION_LABEL):		// Comment ext.
								if (getCommentExt(inputFile))
									return M_EXIT_FAILURE;
								break;
						case(PLAINTEXT_EXTENSION_LABEL):	// Plain text ext.
								if (getPlainTextExt(inputFile))
									return M_EXIT_FAILURE;
								break;
						case(GRAPHICS_CONTROL_LABEL):		// Graphic control ext.
								if (getGraphicControlExt(inputFile))
									return M_EXIT_FAILURE;

								// Read next byte
								readRetVal = readByteFromFile(inputFile, &Byte);
								if (readRetVal == READ_WRITE_ERR) {
									fprintf(stderr, "%s", "Can not read input file.");
									return M_EXIT_FAILURE;
								}
								else if (readRetVal == END_OF_FILE) {
									fprintf(stderr, "%s", "Incorrect gif file.");
									return M_EXIT_FAILURE;
								}
								else {
									switch (Byte) {
										// Image descriptor
										case(IMAGE_DESCRIPTOR_INTRODUCER):

												// Read image descriptor
												if (getImageDescriptor(inputFile, &imageDescriptor))
													return M_EXIT_FAILURE;

												// Init bmpWritter
												bmpWriter.actualColumn = imageDescriptor.leftPosHighByte*256 + imageDescriptor.leftPosLowByte;
												bmpWriter.actualRow = imageDescriptor.topPosHighByte*256 + imageDescriptor.topPosLowByte;
												bmpWriter.actualWidth = imageDescriptor.widthHighByte*256 + imageDescriptor.widthLowByte;
												bmpWriter.actualHeight = imageDescriptor.heightHighByte*256 + imageDescriptor.heightLowByte;
												bmpWriter.actualX = bmpWriter.actualColumn;
												bmpWriter.actualY = bmpWriter.actualRow;

												// Set and read color table
												if (imageDescriptor.localColorTableFlag) {
													reader.activeColorTable = localColorTable;
													reader.activeColorTableSize = imageDescriptor.localColorTableSize;
													if (getColorTable(inputFile, localColorTable, imageDescriptor.localColorTableSize))
														return M_EXIT_FAILURE;
												}
												else {
													reader.activeColorTable = globalColorTable;
													reader.activeColorTableSize = pic.colorTableLong;
												}

												// Get image data
												reader.dataBlockSize = imageDescriptor.sizeInPixels;
												if (getImageData(inputFile, outputFile, &reader, bitMap, &bmpWriter))
													return M_EXIT_FAILURE;

												break;

										// Plain text extension
										case(PLAINTEXT_EXTENSION_LABEL):

												// Get plain text extension
												if (getPlainTextExt(inputFile))
													return M_EXIT_FAILURE;
												break;
										default:
											fprintf(stderr, "%s", "Incorrect gif file.");
											return M_EXIT_FAILURE;
									}
								}

								break;
						default:
							fprintf(stderr, "%s", "Incorrect gif file.");
							return M_EXIT_FAILURE;
					}

				}
			}
			else if (Byte == IMAGE_DESCRIPTOR_INTRODUCER) {
				if (getImageDescriptor(inputFile, &imageDescriptor))
					return M_EXIT_FAILURE;

				// Init bmpWritter
				bmpWriter.actualColumn = imageDescriptor.leftPosHighByte*256 + imageDescriptor.leftPosLowByte;
				bmpWriter.actualRow = imageDescriptor.topPosHighByte*256 + imageDescriptor.topPosLowByte;
				bmpWriter.actualWidth = imageDescriptor.widthHighByte*256 + imageDescriptor.widthLowByte;
				bmpWriter.actualHeight = imageDescriptor.heightHighByte*256 + imageDescriptor.heightLowByte;
				bmpWriter.actualX = bmpWriter.actualColumn;
				bmpWriter.actualY = bmpWriter.actualRow;

				// Set and read color table
				if (imageDescriptor.localColorTableFlag) {
					reader.activeColorTable = localColorTable;
					reader.activeColorTableSize = imageDescriptor.localColorTableSize;
					if (getColorTable(inputFile, localColorTable, imageDescriptor.localColorTableSize))
						return M_EXIT_FAILURE;
				}
				else {
					reader.activeColorTable = globalColorTable;
					reader.activeColorTableSize = pic.globalColorTable;
				}

				reader.dataBlockSize = imageDescriptor.sizeInPixels;
				if (getImageData(inputFile, outputFile, &reader, bitMap, &bmpWriter))
					return M_EXIT_FAILURE;
			}
			else {
				fprintf(stderr, "%s", "Incorrect gif file.");
				return M_EXIT_FAILURE;
			}
		}
	}

	// Write BMP header into file
	if (writeBmpHeaders(outputFile, &pic)) {
		dealocateBitMapBuffer (bitMap, nrows, ncolumns);
		return M_EXIT_FAILURE;
	}

	// Write BMP data stream into file
	if (writeBmpData(outputFile, bitMap, &pic)) {
		dealocateBitMapBuffer (bitMap, nrows, ncolumns);
		return M_EXIT_FAILURE;
	}

	// Set BMP header file size
	if (setBMPFileSize(outputFile, logStruc)) {
		dealocateBitMapBuffer (bitMap, nrows, ncolumns);
		return M_EXIT_FAILURE;
	}

	// Free BMP buffer
	dealocateBitMapBuffer (bitMap, nrows, ncolumns);

	return EXIT_SUCCESS;
}

cv::Mat loadGif(const string &filename)
{
    FILE *f = fopen(filename.c_str(), "rb");
    FILE *fout = fopen("test.bmp", "wb+");
    Mat m = cv::Mat::zeros(100, 100, CV_32FC1);

    if (f == NULL)
        throw "Unable to open input file";

    if (fout == NULL)
        throw "Unable to open output file";

    // TODO: Predelat aby brala jako vstup cv::Mat a do te ulozit vysledek misto BMP
    // TODO: Misto return throw vyjimka tak jak je tomu o par radku vys
    tGIF2BMP log_struc;
    if (gif2bmp(&log_struc, f, fout) != EXIT_SUCCESS);
        throw "gif2bmp exited incorrectly";

    fclose(f);
    fclose(fout);

    return m;
}


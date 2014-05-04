/*
 *  File name: bmp.c
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Source file
 *  Description: Include functions for dictionary
 */

#include "gif2bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <inttypes.h>
#include "gif.h"
#include "dictionary.h"
#include "constant.h"

/**
 * Create and initialize new dictionary item
 *
 * @param colorIndex Color table index of new list item
 * @return Pointer to new list item
 */
tLIST_ITEM *newDicItem(int colorIndex)
{
  tLIST_ITEM *item = (tLIST_ITEM*)malloc(sizeof(tLIST_ITEM));
  if (item != NULL)
	  item->colorTableIndex = colorIndex;

  item->nextColor = NULL;

  return item;
}

/**
 * Dealocate dictionary list item
 *
 * @param list Pointer to list item
 */
void freeFirstListItem(tDIC_ITEM *list)
{
	tLIST_ITEM *item = list->first;
	if(item != NULL) {
		list->first = item->nextColor;
		free(item);
  }
}

/**
 * Dealocate dictionary color list
 * @param list
 */
void dealocateColorList(tDIC_ITEM *list) {

	while (list->first != NULL)
		freeFirstListItem(list);

	list->last = NULL;
}

/**
 * Get color list length (number of colors)
 *
 * @param list Dictionary color list
 * @return List length
 */
int listLength(tDIC_ITEM *list) {

	int length = 0;
	tLIST_ITEM *item = list->first;

	while (item != NULL) {
		length++;
		item = item->nextColor;
	}

	return length;
}

/**
 * Copy dictionary source list to dictionary destination list
 *
 * @param sourcelist Pointer to dictionary source list
 * @param destlist Pointer to dictionary destination list
 * @return 0 on success, 1 on failure
 */
int copyLists(tDIC_ITEM *sourcelist, tDIC_ITEM *destlist)
{
	// Get source list first item
	tLIST_ITEM *sourceItem = sourcelist->first;

	// Copy first item
	if (sourceItem != NULL) {
		destlist->first = newDicItem(sourceItem->colorTableIndex);
		if (destlist->first == NULL) {
			fprintf(stderr, "%s", "Can not create dictionary.");
			return EXIT_FAILURE;
		}
		destlist->last = destlist->first;
	}
	else // Empty source list
		return EXIT_SUCCESS;

	// Copy rest of list
	tLIST_ITEM *destItem = destlist->first;
	sourceItem = sourceItem->nextColor;
	while (sourceItem != NULL) {
		destItem->nextColor = newDicItem(sourceItem->colorTableIndex);
		destlist->last = destItem->nextColor;
		if (destItem->nextColor == NULL) {
			fprintf(stderr, "%s", "Can not create dictionary.");
			return EXIT_FAILURE;
		}
		destItem = destItem->nextColor;
		sourceItem = sourceItem->nextColor;
	}

	return EXIT_SUCCESS;
}

/**
 * Dealocate created dictionary records (from index EOI + 1)
 *
 * @param dictionary Pointer to dictionary
 */
void freeDictionary (tDICTIONARY *dictionary) {

	//for (int i = (dictionary->endOfInformationCode + 1); i < DICTIONARY_MAX_SIZE; i++) {
	for (int i = 0; i < DICTIONARY_MAX_SIZE; i++) {
		dealocateColorList(&(dictionary->colors[i]));
	}
}

/**
 * Init dictionary from active color palette (reader->activeColorTable)
 *
 * @param dictionary Pointer to dictionary
 * @param reader Pointer to GIF reader
 * @return 0 on success, 1 on failure
 */
int initDictionary (tDICTIONARY *dictionary, tGIFREADER *reader) {
	// Init dictionary pointers
	for (int i = 0; i < DICTIONARY_MAX_SIZE; i++) {
		dictionary->colors[i].first = NULL;
		dictionary->colors[i].last = NULL;
	}
	// Insert color table into dictionary
	int i;
	for (i = 0; i < reader->activeColorTableSize; i++) {
		dictionary->colors[i].first = newDicItem(i);
		if (dictionary->colors[i].first == NULL) {
			fprintf(stderr, "%s", "Can not create dictionary.");
			return EXIT_FAILURE;
		}
		dictionary->colors[i].last = dictionary->colors[i].first;
	}

	// Init dictionary control values
	dictionary->clearCode = i;
	dictionary->endOfInformationCode = i + 1;
	dictionary->previousCode = -1;
	dictionary->firstEmptyCode = i + 2;

	// Set dictionary max value for current LZW size
	switch (reader->lzwSize) {
		case(2):
				dictionary->curMaxCode = _2_BITS_MAX_CODE;
				break;
		case(3):
				dictionary->curMaxCode = _3_BITS_MAX_CODE;
				break;
		case(4):
				dictionary->curMaxCode = _4_BITS_MAX_CODE;
				break;
		case(5):
				dictionary->curMaxCode = _5_BITS_MAX_CODE;
				break;
		case(6):
				dictionary->curMaxCode = _6_BITS_MAX_CODE;
				break;
		case(7):
				dictionary->curMaxCode = _7_BITS_MAX_CODE;
				break;
		case(8):
				dictionary->curMaxCode = _8_BITS_MAX_CODE;
				break;
		case(9):
				dictionary->curMaxCode = _9_BITS_MAX_CODE;
				break;
		case(10):
				dictionary->curMaxCode = _10_BITS_MAX_CODE;
				break;
		case(11):
				dictionary->curMaxCode = _11_BITS_MAX_CODE;
				break;
		case(12):
				dictionary->curMaxCode = _12_BITS_MAX_CODE;
				break;
		default:
				fprintf(stderr, "%s", "Unsupported LZW size.");
				return EXIT_FAILURE;
	}

	// Check dictionary capacity for current LZW size
	if ((dictionary->firstEmptyCode > dictionary->curMaxCode) && (reader->lzwSize < 12)) {

		// Increase LZW size and max code value
		reader->lzwSize++;
		switch (reader->lzwSize) {
				case(2):
						dictionary->curMaxCode = _2_BITS_MAX_CODE;
						break;
				case(3):
						dictionary->curMaxCode = _3_BITS_MAX_CODE;
						break;
				case(4):
						dictionary->curMaxCode = _4_BITS_MAX_CODE;
						break;
				case(5):
						dictionary->curMaxCode = _5_BITS_MAX_CODE;
						break;
				case(6):
						dictionary->curMaxCode = _6_BITS_MAX_CODE;
						break;
				case(7):
						dictionary->curMaxCode = _7_BITS_MAX_CODE;
						break;
				case(8):
						dictionary->curMaxCode = _8_BITS_MAX_CODE;
						break;
				case(9):
						dictionary->curMaxCode = _9_BITS_MAX_CODE;
						break;
				case(10):
						dictionary->curMaxCode = _10_BITS_MAX_CODE;
						break;
				case(11):
						dictionary->curMaxCode = _11_BITS_MAX_CODE;
						break;
				case(12):
						dictionary->curMaxCode = _12_BITS_MAX_CODE;
						break;
		}
	}

	return EXIT_SUCCESS;
}

/**
 * reInit dictionary from active color palette (reader->activeColorTable)
 *
 * @param dictionary Pointer to dictionary
 * @param reader Pointer to GIF reader
 * @return 0 on success, 1 on failure
 */
int reInitDictionary (tDICTIONARY *dictionary, tGIFREADER *reader) {

	// set new dictionary variable
	int i = reader->activeColorTableSize;
	dictionary->clearCode = i;
	dictionary->endOfInformationCode = i + 1;
	dictionary->firstEmptyCode = i + 2;

	// Set dictionary max value
	switch (reader->lzwSize) {
		case(2):
				dictionary->curMaxCode = _2_BITS_MAX_CODE;
				break;
		case(3):
				dictionary->curMaxCode = _3_BITS_MAX_CODE;
				break;
		case(4):
				dictionary->curMaxCode = _4_BITS_MAX_CODE;
				break;
		case(5):
				dictionary->curMaxCode = _5_BITS_MAX_CODE;
				break;
		case(6):
				dictionary->curMaxCode = _6_BITS_MAX_CODE;
				break;
		case(7):
				dictionary->curMaxCode = _7_BITS_MAX_CODE;
				break;
		case(8):
				dictionary->curMaxCode = _8_BITS_MAX_CODE;
				break;
		case(9):
				dictionary->curMaxCode = _9_BITS_MAX_CODE;
				break;
		case(10):
				dictionary->curMaxCode = _10_BITS_MAX_CODE;
				break;
		case(11):
				dictionary->curMaxCode = _11_BITS_MAX_CODE;
				break;
		case(12):
				dictionary->curMaxCode = _12_BITS_MAX_CODE;
				break;
		default:
				fprintf(stderr, "%s", "Unsupported LZW size.");
				return EXIT_FAILURE;
	}

	// Check dictionary capacity for current LZW size
	if ((dictionary->firstEmptyCode > dictionary->curMaxCode) && (reader->lzwSize < 12)) {

		// Increase LZW size and set new current max code
		reader->lzwSize++;
		switch (reader->lzwSize) {
				case(2):
						dictionary->curMaxCode = _2_BITS_MAX_CODE;
						break;
				case(3):
						dictionary->curMaxCode = _3_BITS_MAX_CODE;
						break;
				case(4):
						dictionary->curMaxCode = _4_BITS_MAX_CODE;
						break;
				case(5):
						dictionary->curMaxCode = _5_BITS_MAX_CODE;
						break;
				case(6):
						dictionary->curMaxCode = _6_BITS_MAX_CODE;
						break;
				case(7):
						dictionary->curMaxCode = _7_BITS_MAX_CODE;
						break;
				case(8):
						dictionary->curMaxCode = _8_BITS_MAX_CODE;
						break;
				case(9):
						dictionary->curMaxCode = _9_BITS_MAX_CODE;
						break;
				case(10):
						dictionary->curMaxCode = _10_BITS_MAX_CODE;
						break;
				case(11):
						dictionary->curMaxCode = _11_BITS_MAX_CODE;
						break;
				case(12):
						dictionary->curMaxCode = _12_BITS_MAX_CODE;
						break;
		}
	}

	// Reinit rest of dictionary indexes
	for (i = i + 2; i < DICTIONARY_MAX_SIZE; i++) {
		dealocateColorList(&(dictionary->colors[i]));
		dictionary->colors[i].first = NULL;
		dictionary->colors[i].last = NULL;
	}

	return EXIT_SUCCESS;
}

/**
 * Look for code in dictionary
 *
 * @param dictionary Pointer to dictionary
 * @param code Code to look for
 * @return 0 on success, 1 on failure
 */
int codeInDictionary(tDICTIONARY *dictionary, u_int32_t *code) {

	// Check code range
	if (*code >= DICTIONARY_MAX_SIZE) {
		fprintf(stderr, "%s", "Can not create dictionary.");
		return EXIT_FAILURE;
	}

	// Code in dictionary?
	if (dictionary->colors[*code].first == NULL)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}


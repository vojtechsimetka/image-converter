/*
 *  File name: dictionary.h
 *  Created on: 15. 3. 2014
 *  Author: Adam Siroky
 *  Login: xsirok07
 *  Type: Header file
 *  Description: Include functions declarations for dictionary
 */

#ifndef RGB_H_
#define RGB_H_

#include "constant.h"

void freeDictionary (tDICTIONARY *dictionary);
int reInitDictionary (tDICTIONARY *dictionary, tGIFREADER *reader);
int initDictionary (tDICTIONARY *dictionary, tGIFREADER *reader);
int codeInDictionary(tDICTIONARY *dictionary, u_int32_t *code);
int copyLists(tDIC_ITEM *sourcelist, tDIC_ITEM *destlist);
int listLength(tDIC_ITEM *list);
tLIST_ITEM *newDicItem(int colorIndex);

#endif /* RGB_H_ */


/*
* 
* Tue Jun  9 01:30:16 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _SECONDARYINDEX_H_
#define _SECONDARYINDEX_H_

#include <stdlib.h>
#include <stdio.h>

#include "ArrayList.h"
#include "Field.h"

#include "StringConverter.h"
#include "NumberConverter.h"
#include "PointersComparator.h"

#define NO_NEXT -1

typedef struct SecondaryIndex {
	void *value;
	long recordOffset;
	long nextOffset;
} SecondaryIndex;

SecondaryIndex *newSecondaryIndex(void *value, long recordOffset, long nextOffset);

void deleteSecondaryIndex(SecondaryIndex *si);

#endif

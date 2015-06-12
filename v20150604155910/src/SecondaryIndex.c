
/*
* 
* Tue Jun  9 01:35:20 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/SecondaryIndex.h"

SecondaryIndex *newSecondaryIndex(void *value, long recordOffset, long nextOffset) {
	SecondaryIndex *si = (SecondaryIndex *) malloc(sizeof(SecondaryIndex));
	si->value = value;
	si->recordOffset = recordOffset;
	si->nextOffset = nextOffset;
	return si;
}

void deleteSecondaryIndex(SecondaryIndex *si) {
	if(si->value != NULL) free(si->value);
	if(si != NULL) free(si);
}

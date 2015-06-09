
/*
* 
* Tue Jun  9 01:35:20 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/SecondaryIndex.h"

SecondaryIndex *newSecondaryIndex(void *value, long offset) {
	SecondaryIndex *si = (SecondaryIndex *) malloc(sizeof(SecondaryIndex));
	si->value = value;
	si->offset = offset;
	return si;
}

void deleteSecondaryIndex(SecondaryIndex *si) {
	if(si->value != NULL) free(si->value);
	if(si != NULL) free(si);
}


/*
* 
* Sun Jun  7 16:04:28 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _SELECTIONHANDLER_H_
#define _SELECTIONHANDLER_H_

#include "Table.h"

typedef struct SelectionHandler {
	Table *t;
	BinaryFileWriter *bfw;
	BinaryFileReader *bfr;
} SelectionHandler;

SelectionHandler *newSelectionHandler(Table *t);

void deleteSelectionHandler(SelectionHandler *sh);

ArrayList *selectAll(SelectionHandler *sh);

ArrayList *selectBySecondaryIndex(SelectionHandler *sh, int position, SecondaryIndex *si);

ArrayList *selectByOffset(SelectionHandler *sh, long offset);

int *selectInt(SelectionHandler *sh);

long *selectLong(SelectionHandler *sh);

float *selectFloat(SelectionHandler *sh);

double *selectDouble(SelectionHandler *sh);

char *selectChar(SelectionHandler *sh);

char *selectString(SelectionHandler *sh);

#endif

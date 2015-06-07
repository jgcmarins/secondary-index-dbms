
/*
* 
* Sun Jun  7 00:54:32 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _INSERTIONHANDLER_H_
#define _INSERTIONHANDLER_H_

#include "Table.h"

typedef struct InsertionHandler {
	Table *t;
	BinaryFileWriter *bfw;
	BinaryFileReader *bfr;
} InsertionHandler;

InsertionHandler *newInsertionHandler(Table *t);

void deleteInsertionHandler(InsertionHandler *ih);

void insert(InsertionHandler *ih, ArrayList *records);

long insertInt(InsertionHandler *ih, char *record, long offset);

long insertLong(InsertionHandler *ih, char *record, long offset);

long insertFloat(InsertionHandler *ih, char *record, long offset);

long insertDouble(InsertionHandler *ih, char *record, long offset);

long insertChar(InsertionHandler *ih, char *record, long offset);

long insertString(InsertionHandler *ih, char *record, long offset);

int findBestFit(InsertionHandler *ih, ArrayList *records);

int calculateRecordSize(InsertionHandler *ih, ArrayList *records);

#endif

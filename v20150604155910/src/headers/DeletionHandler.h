
/*
* 
* Tue Jun 16 04:17:34 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _DELETIONHANDLER_H_
#define _DELETIONHANDLER_H_

#include "Table.h"

typedef struct DeletionHandler {
	Table *t;
	BinaryFileWriter *bfw;
	BinaryFileReader *bfr;
} DeletionHandler;

DeletionHandler *newDeletionHandler(Table *t);

void deleteDeletionHandler(DeletionHandler *dh);

void deleteByOffset(DeletionHandler *dh, long offset);

#endif

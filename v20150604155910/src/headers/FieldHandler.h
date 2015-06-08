
/*
* 
* Fri Jun  5 13:45:18 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _FIELDHANDLER_H_
#define _FIELDHANDLER_H_

#include "Field.h"
#include "BinaryFileWriter.h"
#include "BinaryFileReader.h"
#include "ArrayList.h"

typedef struct FieldHandler {
	BinaryFileWriter *bfw;
	BinaryFileReader *bfr;
	ArrayList *fields;
	int numberOfFields;
} FieldHandler;

FieldHandler *newFieldHandler(BinaryFile *bf, char delimiter);

void deleteFieldHandler(FieldHandler *fh);

void buildFieldHandler(FieldHandler *fh);

void addNewField(FieldHandler *fh, Field *f);

Field *getField(FieldHandler *fh, int position);

char *getFieldName(FieldHandler *fh, int position);

char *getFieldType(FieldHandler *fh, int position);

char *getFieldKey(FieldHandler *fh, int position);

#endif


/*
* 
* Sat Jun  6 12:19:09 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _TABLE_H_
#define _TABLE_H_

#include "FieldHandler.h"
#include "NumberConverter.h"
#include "StringConverter.h"
#include "TableView.h"

#define DELIMITER '|'
#define DELETE '*'
#define TABLE ".table"
#define FIELDS ".fields"

typedef struct Table {
	char *tableFileName;
	char *fieldsFileName;
	BinaryFile *tableFile;
	BinaryFile *fieldsFile;
	FieldHandler *fh;
} Table;

Table *newTable(char *fileName);

void deleteTable(Table *t);

char *buildExtension(char *fileName, const char *extension);

BinaryFile *getTableFile(Table *t);

void displayFields(Table *t);

#endif

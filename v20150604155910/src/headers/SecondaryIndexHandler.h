
/*
* 
* Tue Jun  9 18:07:47 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _SECONDARYINDEXHANDLER_H_
#define _SECONDARYINDEXHANDLER_H_

#include "SecondaryIndex.h"
#include "FieldHandler.h"

#define INDEXEXTENSION ".sindex"
#define INVERTEDLISTEXTENSION ".ilist"

typedef struct SecondaryIndexHandler {
	char *fileName;
	ArrayList *fields; //list of all fields that are secondary key
	ArrayList *index; //list of lists with secondary keys
	ArrayList *files; //list of binary files with secondary keys
	ArrayList *invertedLists; //list of binary files with duplicated (or more) secondary keys
} SecondaryIndexHandler;

SecondaryIndexHandler *newSecondaryIndexHandler(char *fileName, ArrayList *fields);

void deleteSecondaryIndexHandler(SecondaryIndexHandler *sih);

void buildNameToSecondaryIndexFiles(SecondaryIndexHandler *sih, const char *extension);

void buildIndex(SecondaryIndexHandler *sih);

void buildFiles(SecondaryIndexHandler *sih);

void buildInvertedLists(SecondaryIndexHandler *sih);

void addIndex(SecondaryIndexHandler *sih, Field *f, SecondaryIndex *si);

#endif

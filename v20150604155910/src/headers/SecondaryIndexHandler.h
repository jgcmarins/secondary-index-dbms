
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
#include "PointersComparator.h"
#include "TableView.h"

#define SEPARATOR "_"
#define INDEXEXTENSION ".sindex"
#define INVERTEDLISTEXTENSION ".ilist"

#define RECORDOFFSET "Record offset"
#define NEXTOFFSET "Next offset"

typedef struct SecondaryIndexHandler {
	char *fileName;
	ArrayList *fields; //list of all fields that are secondary key
	ArrayList *index; //list of lists with secondary keys
	ArrayList *files; //list of binary files with secondary keys
	ArrayList *invertedLists; //list of binary files with duplicated (or more) secondary keys
} SecondaryIndexHandler;

SecondaryIndexHandler *newSecondaryIndexHandler(char *fileName, ArrayList *fields);

void deleteSecondaryIndexHandler(SecondaryIndexHandler *sih);

void deleteIndex(SecondaryIndexHandler *sih);

void deleteInvertedLists(SecondaryIndexHandler *sih);

void deleteFiles(SecondaryIndexHandler *sih);

void deleteFields(SecondaryIndexHandler *sih);

char *buildFilesNames(SecondaryIndexHandler *sih, const char *extension, int position);

void buildIndex(SecondaryIndexHandler *sih);

void buildFiles(SecondaryIndexHandler *sih);

void buildInvertedLists(SecondaryIndexHandler *sih);

void insertSecondaryIndex(SecondaryIndexHandler *sih, ArrayList *secondaryKeys, long recordOffset);

ArrayList *createNewSecondaryIndex(SecondaryIndexHandler *sih, ArrayList *secondaryKeys, long recordOffset);

void addIndex(SecondaryIndexHandler *sih, SecondaryIndex *si, int position);

int compareIntSecondaryIndex(void *o1, void *o2);

int compareLongSecondaryIndex(void *o1, void *o2);

int compareFloatSecondaryIndex(void *o1, void *o2);

int compareDoubleSecondaryIndex(void *o1, void *o2);

int compareCharSecondaryIndex(void *o1, void *o2);

int compareStringSecondaryIndex(void *o1, void *o2);

SecondaryIndex *selectSecondaryIndex(BinaryFile *bf, long offset, char* type);

void displayIndex(SecondaryIndexHandler *sih);

void displayIndexRow(ArrayList *columnName, ArrayList *rows);

ArrayList *secondaryIndexToString(SecondaryIndex *si, char *type);

#endif

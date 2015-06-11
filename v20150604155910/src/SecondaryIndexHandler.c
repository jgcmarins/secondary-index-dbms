
/*
* 
* Thu Jun 11 05:32:07 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/SecondaryIndexHandler.h"

SecondaryIndexHandler *newSecondaryIndexHandler(char *fileName, ArrayList *fields) {
	SecondaryIndexHandler *sih = (SecondaryIndexHandler *) malloc(sizeof(SecondaryIndexHandler));
	sih->fileName = fileName;

	sih->fields = fields;
	buildFiles(sih);
	buildInvertedLists(sih);
	buildIndex(sih);

	return sih;
}

void deleteSecondaryIndexHandler(SecondaryIndexHandler *sih) {
	deleteIndex(sih);
	deleteInvertedLists(sih);
	deleteFiles(sih);
	deleteFields(sih);

	if(sih->fileName != NULL) sih->fileName = NULL;
	if(sih != NULL) free(sih);
}

void deleteIndex(SecondaryIndexHandler *sih) {
	if(sih->index != NULL) {
		while(sih->index->length > 0) {
			ArrayList *index = getArrayListObject(sih->index, sih->index->length - 1);
			removeArrayListObjectFromPosition(sih->index, sih->index->length - 1);
			while(index->length > 0) {
				SecondaryIndex *si = getArrayListObject(index, index->length - 1);
				removeArrayListObjectFromPosition(index, index->length - 1);
				deleteSecondaryIndex(si);
			}
			deleteArrayList(index);
		}
		deleteArrayList(sih->index);
	}
}

void deleteInvertedLists(SecondaryIndexHandler *sih) {
	if(sih->invertedLists != NULL) {
		while(sih->invertedLists->length > 0) {
			BinaryFile *bf = (BinaryFile *) getArrayListObject(sih->invertedLists, sih->invertedLists->length - 1);
			char *fileName = bf->fileName;
			removeArrayListObjectFromPosition(sih->invertedLists, sih->invertedLists->length - 1);
			deleteBinaryFile(bf);
			free(fileName);
		}
		deleteArrayList(sih->invertedLists);
	}
}

void deleteFiles(SecondaryIndexHandler *sih) {
	if(sih->files != NULL) {
		while(sih->files->length > 0) {
			BinaryFile *bf = (BinaryFile *) getArrayListObject(sih->files, sih->files->length - 1);
			char *fileName = bf->fileName;
			removeArrayListObjectFromPosition(sih->files, sih->files->length - 1);
			deleteBinaryFile(bf);
			free(fileName);
		}
		deleteArrayList(sih->files);
	}
}

void deleteFields(SecondaryIndexHandler *sih) {
	if(sih->fields != NULL) {
		while(sih->fields->length > 0) removeArrayListObjectFromPosition(sih->fields, sih->fields->length - 1);
		deleteArrayList(sih->fields);
	}
}

char *buildFilesNames(SecondaryIndexHandler *sih, const char *extension, int position) {
	char *fileName = sih->fileName;
	Field *f = (Field *) getArrayListObject(sih->fields, position);
	char *field = f->name;
	char *string = (char *) malloc(sizeof(char)*(strlen(fileName) + strlen(SEPARATOR) +
									strlen(field) + strlen(extension) + 1));
	memcpy(string, fileName, sizeof(char)*(strlen(fileName)));
	string[strlen(fileName)] = '\0';
	string = strcat(string, SEPARATOR);
	string = strcat(string, field);
	string = strcat(string, extension);
	return string;
}

void buildIndex(SecondaryIndexHandler *sih) {
	sih->index = newArrayList();
	int i;
	for(i = 0 ; i < sih->fields->length ; i++){
		ArrayList *si = newArrayList();
		setArrayListObject(sih->index, (ArrayList *) si, i);
	}
}

void buildFiles(SecondaryIndexHandler *sih) {
	sih->files = newArrayList();
	int i;
	for(i = 0 ; i < sih->fields->length ; i++){
		char *fileName = buildFilesNames(sih, INDEXEXTENSION, i);
		BinaryFile *bf = newBinaryFile(fileName);
		setArrayListObject(sih->files, (BinaryFile *) bf, i);
	}
}

void buildInvertedLists(SecondaryIndexHandler *sih) {
	sih->invertedLists = newArrayList();
	int i;
	for(i = 0 ; i < sih->fields->length ; i++){
		char *fileName = buildFilesNames(sih, INVERTEDLISTEXTENSION, i);
		BinaryFile *bf = newBinaryFile(fileName);
		setArrayListObject(sih->files, (BinaryFile *) bf, i);
	}
}

void addIndex(SecondaryIndexHandler *sih, SecondaryIndex *si, int position) {
	Field *f = (Field *) getArrayListObject(sih->fields, position);
	ArrayList *index = (ArrayList *) getArrayListObject(sih->index, position);
	char *type = f->type;
	if(!strcmp(type, INT)) addArrayListObject(index, si, compareIntSecondaryIndex);
	else if(!strcmp(type, LONG)) addArrayListObject(index, si, compareLongSecondaryIndex);
	else if(!strcmp(type, FLOAT)) addArrayListObject(index, si, compareFloatSecondaryIndex);
	else if(!strcmp(type, DOUBLE)) addArrayListObject(index, si, compareDoubleSecondaryIndex);
	else if(!strcmp(type, CHAR)) addArrayListObject(index, si, compareCharSecondaryIndex);
	else if(!strcmp(type, STRING)) addArrayListObject(index, si, compareStringSecondaryIndex);
}

int compareIntSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareInt(si1->value, si2->value);
}

int compareLongSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareLong(si1->value, si2->value);
}

int compareFloatSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareFloat(si1->value, si2->value);
}

int compareDoubleSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareDouble(si1->value, si2->value);
}

int compareCharSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareChar(si1->value, si2->value);
}

int compareStringSecondaryIndex(void *o1, void *o2) {
	SecondaryIndex *si1 = (SecondaryIndex *) o1;
	SecondaryIndex *si2 = (SecondaryIndex *) o2;

	return compareString(si1->value, si2->value);
}

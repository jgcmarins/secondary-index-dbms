
/*
* 
* Sat Jun  6 12:20:14 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Table.h"

Table *newTable(char *fileName) {
	Table *t = (Table *) malloc(sizeof(Table));

	t->tableFileName = buildNameToTableFiles(fileName, TABLE);
	t->fieldsFileName = buildNameToTableFiles(fileName, FIELDS);

	t->tableFile = newBinaryFile(t->tableFileName);
	t->fieldsFile = newBinaryFile(t->fieldsFileName);

	t->fh = newFieldHandler(t->fieldsFile);
	t->sih = newSecondaryIndexHandler(fileName, buildSecondaryIndexList(t));
	buildSecondaryIndex(t);

	return t;
}

void deleteTable(Table *t) {
	if(t->sih != NULL) deleteSecondaryIndexHandler(t->sih);
	if(t->fh != NULL) deleteFieldHandler(t->fh);
	if(t->fieldsFile != NULL) deleteBinaryFile(t->fieldsFile);
	if(t->tableFile != NULL) deleteBinaryFile(t->tableFile);
	if(t->fieldsFileName != NULL) free(t->fieldsFileName);
	if(t->tableFileName != NULL) free(t->tableFileName);
	if(t != NULL) free(t);
}

char *buildNameToTableFiles(char *fileName, const char *extension) { // tenso
	char *string = (char *) malloc(sizeof(char)*(strlen(fileName) + strlen(extension) + 1));
	memcpy(string, fileName, sizeof(char)*(strlen(fileName)));
	string[strlen(fileName)] = '\0';
	string = strcat(string, extension);
	return string;
}

ArrayList *buildSecondaryIndexList(Table *t) {
	ArrayList *secondaryFields = newArrayList();
	int i;
	for(i = 0 ; i < getNumberOfFields(t->fh) ; i++) {
		if(!strcmp(SECONDARY_KEY, getFieldKey(t->fh, i)))
			setArrayListObject(secondaryFields, (Field *) getField(t->fh, i), secondaryFields->length);
	}
	
	return secondaryFields;
}

void buildSecondaryIndex(Table *t) {
	int i;
	for(i = 0 ; i < t->sih->fields->length ; i++) {
		Field *f = (Field *) getArrayListObject(t->sih->fields, i);
		BinaryFile *bfFiles = (BinaryFile *) getArrayListObject(t->sih->files, i);
		seekBinaryFile(bfFiles, 0L);

		while(getStreamOffset(bfFiles) < getBinaryFileSize(bfFiles)) { // each secondary index
			//printf("1.Checando offset \"%ld\" do arquivos \"%s\"\n", getStreamOffset(bfFiles), bfFiles->fileName);
			SecondaryIndex *si = selectSecondaryIndex(bfFiles, getStreamOffset(bfFiles), f->type);
			addIndex(t->sih, si, i);

			while(si->nextOffset != -1) { // each duplicated
				BinaryFile *bfLists = (BinaryFile *) getArrayListObject(t->sih->invertedLists, i);
				//printf("2.Checando offset \"%ld\" do arquivos \"%s\"\n", si->nextOffset, bfLists->fileName);
				si = selectSecondaryIndex(bfLists, si->nextOffset, f->type);
				addIndex(t->sih, si, i);
			}
		}
	}
}

BinaryFile *getTableFile(Table *t) {
	return t->tableFile;
}

void displayFields(Table *t) {
	ArrayList *names = newArrayList();
	ArrayList *types = newArrayList();
	ArrayList *keys = newArrayList();
	int i;
	for(i = 0 ; i < t->fh->fields->length ; i++) {
		setArrayListObject(names, (char *) getFieldName(t->fh, i), i);
		setArrayListObject(types, (char *) getFieldType(t->fh, i), i);
		setArrayListObject(keys, (char *) getFieldKey(t->fh, i), i);
	}

	TableView *tv = newTableView(names);
	printTableHeader(tv);
	printTableRow(tv, types);
	printTableRow(tv, keys);

	while(names->length > 0) removeArrayListObjectFromPosition(names, names->length - 1);
	deleteArrayList(names);

	while(types->length > 0) removeArrayListObjectFromPosition(types, types->length - 1);
	deleteArrayList(types);

	while(keys->length > 0) removeArrayListObjectFromPosition(keys, keys->length - 1);
	deleteArrayList(keys);

	deleteTableView(tv);
}

ArrayList *rowToString(Table *t, ArrayList *row) {
	ArrayList *strings = newArrayList();
	int i;
	for(i = 0 ; i < row->length ; i++) {
		char *type = getFieldType(t->fh, i);
		if(!strcmp(type, INT)) {
			char *string = intToString(*((int *) getArrayListObject(row, i)));
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, LONG)) {
			char *string = longToString(*((long *) getArrayListObject(row, i)));
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, FLOAT)) {
			char *string = floatToString(*((float *) getArrayListObject(row, i)));
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, DOUBLE)) {
			char *string = doubleToString(*((double *) getArrayListObject(row, i)));
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, CHAR)) {
			char *p = (char *) getArrayListObject(row, i);
			char *string = (char *) malloc(sizeof(char)*(strlen(p) + 1));
			memcpy(string, p, sizeof(char)*(strlen(p)));
			string[strlen(p)] = '\0';
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, STRING)) {
			char *p = (char *) getArrayListObject(row, i);
			char *string = (char *) malloc(sizeof(char)*(strlen(p) + 1));
			memcpy(string, p, sizeof(char)*(strlen(p)));
			string[strlen(p)] = '\0';
			setArrayListObject(strings, (char *) string, strings->length);
		}
	}
	return strings;
}

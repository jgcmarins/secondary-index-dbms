
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

	t->tableFileName = buildExtension(fileName, TABLE);
	t->fieldsFileName = buildExtension(fileName, FIELDS);

	t->tableFile = newBinaryFile(t->tableFileName);
	t->fieldsFile = newBinaryFile(t->fieldsFileName);

	t->fh = newFieldHandler(t->fieldsFile, DELIMITER);

	return t;
}

void deleteTable(Table *t) {
	if(t->fh != NULL) deleteFieldHandler(t->fh);
	if(t->fieldsFile != NULL) deleteBinaryFile(t->fieldsFile);
	if(t->tableFile != NULL) deleteBinaryFile(t->tableFile);
	if(t->fieldsFileName != NULL) free(t->fieldsFileName);
	if(t->tableFileName != NULL) free(t->tableFileName);
	if(t != NULL) free(t);
}

char *buildExtension(char *fileName, const char *extension) { // tenso
	char *string = (char *) malloc(sizeof(char)*(strlen(fileName) + strlen(extension) + 1));
	memcpy(string, fileName, sizeof(char)*(strlen(fileName)));
	string[strlen(fileName)] = '\0';
	string = strcat(string, extension);
	return string;
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
			int *p = (int *) getArrayListObject(row, i);
			char *string = intToString(*p);
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, LONG)) {
			long *p = (long *) getArrayListObject(row, i);
			char *string = longToString(*p);
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, FLOAT)) {
			float *p = (float *) getArrayListObject(row, i);
			char *string = floatToString(*p);
			setArrayListObject(strings, (char *) string, strings->length);
		} else if(!strcmp(type, DOUBLE)) {
			double *p = (double *) getArrayListObject(row, i);
			char *string = doubleToString(*p);
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

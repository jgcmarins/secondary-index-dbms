
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

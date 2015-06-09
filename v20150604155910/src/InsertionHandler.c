
/*
* 
* Sun Jun  7 00:59:36 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/InsertionHandler.h"

InsertionHandler *newInsertionHandler(Table *t) {
	InsertionHandler *ih = (InsertionHandler *) malloc(sizeof(InsertionHandler));
	ih->t = t;
	ih->bfw = newBinaryFileWriter(getTableFile(t), DELIMITER);
	ih->bfr = newBinaryFileReader(getTableFile(t), DELIMITER);

	return ih;
}

void deleteInsertionHandler(InsertionHandler *ih) {
	if(ih->bfr != NULL) deleteBinaryFileReader(ih->bfr);
	if(ih->bfw != NULL) deleteBinaryFileWriter(ih->bfw);
	if(ih->t != NULL) ih->t = NULL;
	if(ih != NULL) free(ih);
}

long insert(InsertionHandler *ih, ArrayList *records) {
	long offset = findBestFit(ih, records);
	long recordOffset = offset;
	int i;
	for(i = 0 ; i < records->length ; i++) {
		char *type = getFieldType(ih->t->fh, i);
		//printf("type: \"%s\"\n", type);
		if(!strcmp(type, INT)) offset = insertInt(ih, (char *) getArrayListObject(records, i), offset);
		else if(!strcmp(type, LONG)) offset = insertLong(ih, (char *) getArrayListObject(records, i), offset);
		else if(!strcmp(type, FLOAT)) offset = insertFloat(ih, (char *) getArrayListObject(records, i), offset);
		else if(!strcmp(type, DOUBLE)) offset = insertDouble(ih, (char *) getArrayListObject(records, i), offset);
		else if(!strcmp(type, CHAR)) offset = insertChar(ih, (char *) getArrayListObject(records, i), offset);
		else if(!strcmp(type, STRING)) offset = insertString(ih, (char *) getArrayListObject(records, i), offset);
		//printf("\n");
	}
	return recordOffset;
}

long insertInt(InsertionHandler *ih, char *record, long offset) {
	int number = stringToInt(record);
	writeInt(ih->bfw, number, offset);
	//printf("int: inserting \"%d\" at \"%ld\"\n", number, offset);
	return offset += sizeof(int);
}

long insertLong(InsertionHandler *ih, char *record, long offset) {
	long number = stringToLong(record);
	writeLong(ih->bfw, number, offset);
	//printf("long: inserting \"%ld\" at \"%ld\"\n", number, offset);
	return offset += sizeof(long);
}

long insertFloat(InsertionHandler *ih, char *record, long offset) {
	float number = stringToFloat(record);
	writeFloat(ih->bfw, number, offset);
	//printf("float: inserting \"%f\" at \"%ld\"\n", number, offset);
	return offset += sizeof(float);
}

long insertDouble(InsertionHandler *ih, char *record, long offset) {
	double number = stringToDouble(record);
	writeDouble(ih->bfw, number, offset);
	//printf("double: inserting \"%lf\" at \"%ld\"\n", number, offset);
	return offset += sizeof(double);
}

long insertChar(InsertionHandler *ih, char *record, long offset) {
	char character = record[0];
	writeChar(ih->bfw, character, offset);
	//printf("char: inserting \"%c\" at \"%ld\"\n", character, offset);
	return offset += sizeof(char);
}

long insertString(InsertionHandler *ih, char *record, long offset) {
	writeString(ih->bfw, record, offset);
	//printf("string: inserting \"%s\" at \"%ld\"\n", record, offset);
	return offset += (strlen(record) + 1);
}

long findBestFit(InsertionHandler *ih, ArrayList *records) {
	return getBinaryFileSize(ih->t->tableFile);
}

int calculateRecordSize(InsertionHandler *ih, ArrayList *records) {
	return 0;
}

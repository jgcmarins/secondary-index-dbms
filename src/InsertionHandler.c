
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
	ih->fragment = 0;
	ih->previousOffset = 0L;

	return ih;
}

void deleteInsertionHandler(InsertionHandler *ih) {
	if(ih->bfr != NULL) deleteBinaryFileReader(ih->bfr);
	if(ih->bfw != NULL) deleteBinaryFileWriter(ih->bfw);
	if(ih->t != NULL) ih->t = NULL;
	if(ih != NULL) free(ih);
}

void insert(InsertionHandler *ih, ArrayList *record) {
	long offset = findWorstFit(ih, record);

	long recordOffset = offset;
	ArrayList *secondaryKeys = newArrayList();

	int recordSize = calculateRecordSize(ih, record);
	writeInt(ih->bfw, recordSize, offset);
	offset += sizeof(int);
	long next;
	if(ih->fragment > 0) next = readLong(ih->bfr, offset);

	int i;
	for(i = 0 ; i < record->length ; i++) {
		char *type = getFieldType(ih->t->fh, i);
		if(!strcmp(type, INT)) offset = insertInt(ih, (char *) getArrayListObject(record, i), offset);
		else if(!strcmp(type, LONG)) offset = insertLong(ih, (char *) getArrayListObject(record, i), offset);
		else if(!strcmp(type, FLOAT)) offset = insertFloat(ih, (char *) getArrayListObject(record, i), offset);
		else if(!strcmp(type, DOUBLE)) offset = insertDouble(ih, (char *) getArrayListObject(record, i), offset);
		else if(!strcmp(type, CHAR)) offset = insertChar(ih, (char *) getArrayListObject(record, i), offset);
		else if(!strcmp(type, STRING)) offset = insertString(ih, (char *) getArrayListObject(record, i), offset);
		if(!strcmp(getFieldKey(ih->t->fh, i), SECONDARY_KEY))
			setArrayListObject(secondaryKeys, getArrayListObject(record, i), secondaryKeys->length);
	}

	if(ih->fragment > 0) {
		ih->fragment -= sizeof(int);
		ih->fragment *= -1;
		writeInt(ih->bfw, ih->fragment, offset);
		writeLong(ih->bfw, next, (offset + sizeof(int)));
		if(ih->previousOffset != 0L) writeLong(ih->bfw, offset, (ih->previousOffset + sizeof(int)));
		else writeLong(ih->bfw, offset, ih->previousOffset);
		ih->fragment = 0;
	}

	insertSecondaryIndex(ih->t->sih, secondaryKeys, recordOffset);

	while(secondaryKeys->length > 0) removeArrayListObjectFromPosition(secondaryKeys, secondaryKeys->length - 1);
	deleteArrayList(secondaryKeys);
}

long insertInt(InsertionHandler *ih, char *record, long offset) {
	int number = stringToInt(record);
	writeInt(ih->bfw, number, offset);
	return offset += sizeof(int);
}

long insertLong(InsertionHandler *ih, char *record, long offset) {
	long number = stringToLong(record);
	writeLong(ih->bfw, number, offset);
	return offset += sizeof(long);
}

long insertFloat(InsertionHandler *ih, char *record, long offset) {
	float number = stringToFloat(record);
	writeFloat(ih->bfw, number, offset);
	return offset += sizeof(float);
}

long insertDouble(InsertionHandler *ih, char *record, long offset) {
	double number = stringToDouble(record);
	writeDouble(ih->bfw, number, offset);
	return offset += sizeof(double);
}

long insertChar(InsertionHandler *ih, char *record, long offset) {
	char character = record[0];
	writeChar(ih->bfw, character, offset);
	return offset += sizeof(char);
}

long insertString(InsertionHandler *ih, char *record, long offset) {
	writeString(ih->bfw, record, offset);
	return offset += (strlen(record) + 1);
}

long findWorstFit(InsertionHandler *ih, ArrayList *record) {
	long head = readLong(ih->bfr, 0L);
	long previousOffset = 0L;
	if(head == -1) return getBinaryFileSize(ih->t->tableFile);
	else {
		int size = calculateRecordSize(ih, record);
		size += ih->t->fh->min;
		int biggestSize = 0;
		long worst = -1;
		while(head != -1) {
			seekBinaryFile(ih->bfr->bf, head);
			int currentSize = readInt(ih->bfr, getStreamOffset(ih->bfr->bf));
			currentSize *= (-1);
			if(currentSize > size) {
				if(currentSize > biggestSize) {
					biggestSize = currentSize;
					worst = head;
					ih->fragment = biggestSize - size;
					ih->previousOffset = previousOffset;
				}
			}
			getchar();
			getchar();
			previousOffset = head;
			seekBinaryFile(ih->bfr->bf, (head + sizeof(int)));
			head = readLong(ih->bfr, getStreamOffset(ih->bfr->bf));
		}

		if(worst != -1) return worst;
	}
	return getBinaryFileSize(ih->t->tableFile);
}

int calculateRecordSize(InsertionHandler *ih, ArrayList *record) {
	int i, size = 0;
	for(i = 0 ; i < record->length ; i++) {
		char *type = getFieldType(ih->t->fh, i);
		if(!strcmp(type, INT)) size += sizeof(int);
		else if(!strcmp(type, LONG)) size += sizeof(long);
		else if(!strcmp(type, FLOAT)) size += sizeof(float);
		else if(!strcmp(type, DOUBLE)) size += sizeof(double);
		else if(!strcmp(type, CHAR)) size += sizeof(char);
		else if(!strcmp(type, STRING)) size += (sizeof(char)*(strlen((char *) getArrayListObject(record, i)) + 1));
	}
	return size;
}

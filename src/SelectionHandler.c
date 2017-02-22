
/*
* 
* Sun Jun  7 16:12:23 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/SelectionHandler.h"

SelectionHandler *newSelectionHandler(Table *t) {
	SelectionHandler *sh = (SelectionHandler *) malloc(sizeof(SelectionHandler));
	sh->t = t;
	sh->bfw = newBinaryFileWriter(getTableFile(t), DELIMITER);
	sh->bfr = newBinaryFileReader(getTableFile(t), DELIMITER);

	return sh;
}

void deleteSelectionHandler(SelectionHandler *sh) {
	if(sh->bfr != NULL) deleteBinaryFileReader(sh->bfr);
	if(sh->bfw != NULL) deleteBinaryFileWriter(sh->bfw);
	if(sh->t != NULL) sh->t = NULL;
	if(sh != NULL) free(sh);
}

ArrayList *selectAll(SelectionHandler *sh) {
	long fileSize = getBinaryFileSize(getTableFile(sh->t));
	seekBinaryFile(getTableFile(sh->t), sizeof(long)); //jumping deletion list head
	ArrayList *records = newArrayList();
	while(getStreamOffset(getTableFile(sh->t)) < fileSize) {
		ArrayList *record = selectByOffset(sh, getStreamOffset(getTableFile(sh->t)));
		if(record != NULL) setArrayListObject(records, (ArrayList *) record, records->length);
	}
	return records;
}

ArrayList *selectBySecondaryIndex(SelectionHandler *sh, int position, SecondaryIndex *si) {
	ArrayList *index = searchSecondaryKey(sh->t->sih, position, si);
	ArrayList *records = newArrayList();
	int i;
	for(i = 0 ; i < index->length ; i++){
		SecondaryIndex *si = (SecondaryIndex *) getArrayListObject(index, i);
		ArrayList *record = selectByOffset(sh, si->recordOffset);
		if(record != NULL) setArrayListObject(records, (ArrayList *) record, records->length);
	}

	while(index->length > 0) {
		removeArrayListObjectFromPosition(index, index->length - 1);
	}
	deleteArrayList(index);

	return records;
}

ArrayList *selectByOffset(SelectionHandler *sh, long offset) {
	int deleted = readInt(sh->bfr, offset);
	if(deleted > 0) {
		seekBinaryFile(getTableFile(sh->t), (offset + sizeof(int)));
		ArrayList *record = newArrayList();
		int i;
		for(i = 0 ; i < getNumberOfFields(sh->t->fh) ; i++) {
			char *type = getFieldType(sh->t->fh, i);
			if(!strcmp(type, INT)) {
				int *p = selectInt(sh);
				setArrayListObject(record, (int *) p, record->length);
			} else if(!strcmp(type, LONG)) {
				long *p = selectLong(sh);
				setArrayListObject(record, (long *) p, record->length);
			} else if(!strcmp(type, FLOAT)) {
				float *p = selectFloat(sh);
				setArrayListObject(record, (float *) p, record->length);
			} else if(!strcmp(type, DOUBLE)) {
				double *p = selectDouble(sh);
				setArrayListObject(record, (double *) p, record->length);
			} else if(!strcmp(type, CHAR)) {
				char *character = selectChar(sh);
				setArrayListObject(record, (char *) character, record->length);
			} else if(!strcmp(type, STRING)) {
				char *string = selectString(sh);
				setArrayListObject(record, (char *) string, record->length);
			}
		}
		return record;
	} else {
		deleted *= -1;
		seekBinaryFile(getTableFile(sh->t), (offset + sizeof(int) + deleted));
	}
	return NULL;
}

int *selectInt(SelectionHandler *sh) {
	int number = readInt(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	int *p = (int *) malloc(sizeof(int));
	memcpy(p, &number, sizeof(int));
	return p;
}

long *selectLong(SelectionHandler *sh) {
	long number = readLong(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	long *p = (long *) malloc(sizeof(long));
	memcpy(p, &number, sizeof(long));
	return p;
}

float *selectFloat(SelectionHandler *sh) {
	float number = readFloat(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	float *p = (float *) malloc(sizeof(float));
	memcpy(p, &number, sizeof(float));
	return p;
}

double *selectDouble(SelectionHandler *sh) {
	double number = readDouble(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	double *p = (double *) malloc(sizeof(double));
	memcpy(p, &number, sizeof(double));
	return p;
}

char *selectChar(SelectionHandler *sh) {
	char character = readChar(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	char *p = (char *) malloc(sizeof(char));
	memcpy(p, &character, sizeof(char));
	return p;
}

char *selectString(SelectionHandler *sh) {
	char *string = readString(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	return string;
}

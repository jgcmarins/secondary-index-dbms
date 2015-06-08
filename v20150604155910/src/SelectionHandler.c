
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
	seekBinaryFile(getTableFile(sh->t), 0L);
	int i;
	ArrayList *records = newArrayList();
	while(getStreamOffset(getTableFile(sh->t)) < fileSize) {
		ArrayList *record = newArrayList();
		for(i = 0 ; i < sh->t->fh->numberOfFields ; i++) {
			char *type = getFieldType(sh->t->fh, i);
			//printf("type: \"%s\"\n", type);
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
		//printf("\n");
		setArrayListObject(records, (ArrayList *) record, records->length);
	}
	return records;
}

int *selectInt(SelectionHandler *sh) {
	int number = readInt(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	int *p = (int *) malloc(sizeof(int));
	memcpy(p, &number, sizeof(int));
	//printf("read: \"%d\"\n", number);
	return p;
}

long *selectLong(SelectionHandler *sh) {
	long number = readLong(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	long *p = (long *) malloc(sizeof(long));
	memcpy(p, &number, sizeof(long));
	//printf("read: \"%ld\"\n", number);
	return p;
}

float *selectFloat(SelectionHandler *sh) {
	float number = readFloat(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	float *p = (float *) malloc(sizeof(float));
	memcpy(p, &number, sizeof(float));
	//printf("read: \"%f\"\n", number);
	return p;
}

double *selectDouble(SelectionHandler *sh) {
	double number = readDouble(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	double *p = (double *) malloc(sizeof(double));
	memcpy(p, &number, sizeof(double));
	//printf("read: \"%lf\"\n", number);
	return p;
}

char *selectChar(SelectionHandler *sh) {
	char character = readChar(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	char *p = (char *) malloc(sizeof(char));
	memcpy(p, &character, sizeof(char));
	//printf("read: \"%c\"\n", character);
	return p;
}

char *selectString(SelectionHandler *sh) {
	char *string = readString(sh->bfr, getStreamOffset(getTableFile(sh->t)));
	//printf("read: \"%s\"\n", string);
	return string;
}

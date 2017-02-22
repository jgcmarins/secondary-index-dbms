
/*
* 
* Fri Jun  5 13:46:42 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/FieldHandler.h"

FieldHandler *newFieldHandler(BinaryFile *bf) {
	FieldHandler *fh = (FieldHandler *) malloc(sizeof(FieldHandler));
	fh->bfw = newBinaryFileWriter(bf, DELIMITER);
	fh->bfr = newBinaryFileReader(bf, DELIMITER);
	fh->fields = newArrayList();
	fh->min = 0;

	buildFieldHandler(fh);

	return fh;
}

void deleteFieldHandler(FieldHandler *fh) {
	if(fh->bfw != NULL) deleteBinaryFileWriter(fh->bfw);
	if(fh->bfr != NULL) deleteBinaryFileReader(fh->bfr);

	if(fh->fields != NULL) {
		while(fh->fields->length > 0) {
			Field *f = (Field *) getArrayListObject(fh->fields, fh->fields->length - 1);
			removeArrayListObjectFromPosition(fh->fields, fh->fields->length - 1);
			deleteField(f);
		}
		deleteArrayList(fh->fields);
	}

	if(fh != NULL) free(fh);
}

void buildFieldHandler(FieldHandler *fh) {
	if(getBinaryFileSize(fh->bfr->bf) > 0) {
		seekBinaryFile(fh->bfr->bf, 0L);
		while(getStreamOffset(fh->bfr->bf) < getBinaryFileSize(fh->bfr->bf)) {
			char *name = readString(fh->bfr, getStreamOffset(fh->bfr->bf));
			char *type = readString(fh->bfr, getStreamOffset(fh->bfr->bf));
			char *key = readString(fh->bfr, getStreamOffset(fh->bfr->bf));

			Field *f = newField(name, type, key);
			setArrayListObject(fh->fields, (Field *) f, fh->fields->length);
		}
	}
	calculateMin(fh);
}

void addNewField(FieldHandler *fh, Field *f) {
	setArrayListObject(fh->fields, (Field *) f, fh->fields->length);
	writeString(fh->bfw, f->name, getBinaryFileSize(fh->bfw->bf));
	writeString(fh->bfw, f->type, getBinaryFileSize(fh->bfw->bf));
	writeString(fh->bfw, f->key, getBinaryFileSize(fh->bfw->bf));
	calculateMin(fh);
}

Field *getField(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f;
}

Field *getFieldByName(FieldHandler *fh, char *fieldName) {
	int i;
	for(i = 0 ; i < fh->fields->length ; i++) {
		if(!strcmp(getFieldName(fh, i), fieldName)) return getField(fh, i);
	}
	return NULL;
}

char *getFieldName(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f->name;
}

char *getFieldType(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f->type;
}

char *getFieldKey(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f->key;
}

int getNumberOfFields(FieldHandler *fh) {
	return fh->fields->length;
}

void calculateMin(FieldHandler *fh) {
	fh->min = 0;
	int i;
	for(i = 0 ; i < fh->fields->length ; i++) {
		if(!strcmp(getFieldType(fh, i), INT)) fh->min += sizeof(int);
		else if(!strcmp(getFieldType(fh, i), LONG)) fh->min += sizeof(long);
		else if(!strcmp(getFieldType(fh, i), FLOAT)) fh->min += sizeof(float);
		else if(!strcmp(getFieldType(fh, i), DOUBLE)) fh->min += sizeof(double);
		else if(!strcmp(getFieldType(fh, i), CHAR)) fh->min += sizeof(char);
		else if(!strcmp(getFieldType(fh, i), STRING)) fh->min += (sizeof(char)*2);
	}
	fh->min += HEADER;
}

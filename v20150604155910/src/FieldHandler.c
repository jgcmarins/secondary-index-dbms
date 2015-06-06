
/*
* 
* Fri Jun  5 13:46:42 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/FieldHandler.h"

FieldHandler *newFieldHandler(BinaryFile *bf, char delimiter) {
	FieldHandler *fh = (FieldHandler *) malloc(sizeof(FieldHandler));
	fh->bfw = newBinaryFileWriter(bf, delimiter);
	fh->bfr = newBinaryFileReader(bf, delimiter);
	fh->fields = newArrayList();
	fh->numberOfFields = fh->fields->length;

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
		fh->bfr->bf->currentOffset = getStreamOffset(fh->bfr->bf);
		while(fh->bfr->bf->currentOffset < getBinaryFileSize(fh->bfr->bf)) {
			char *name = readString(fh->bfr, fh->bfr->bf->currentOffset);
			char *type = readString(fh->bfr, fh->bfr->bf->currentOffset);

			Field *f = newField(name, type);
			setArrayListObject(fh->fields, (Field *) f, fh->fields->length);
			fh->numberOfFields = fh->fields->length;
		}
	}
}

void addNewField(FieldHandler *fh, Field *f) {
	setArrayListObject(fh->fields, (Field *) f, fh->fields->length);
	fh->numberOfFields = fh->fields->length;
	writeString(fh->bfw, f->name, getBinaryFileSize(fh->bfw->bf));
	writeString(fh->bfw, f->type, getBinaryFileSize(fh->bfw->bf));
}

Field *getField(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f;
}

char *getFieldName(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f->name;
}

char *getFieldType(FieldHandler *fh, int position) {
	Field *f = (Field *) getArrayListObject(fh->fields, position);
	return f->type;
}

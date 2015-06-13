
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
		setArrayListObject(sih->invertedLists, (BinaryFile *) bf, i);
	}
}

void insertSecondaryIndex(SecondaryIndexHandler *sih, ArrayList *secondaryKeys, long recordOffset) {
	ArrayList *index = createNewSecondaryIndex(sih, secondaryKeys, recordOffset);
	int i;
	for(i = 0 ; i < index->length ; i++) {
		SecondaryIndex *si = (SecondaryIndex *) getArrayListObject(index, i);
		addIndex(sih, si, i);
	}
	while(index->length > 0) removeArrayListObjectFromPosition(index, index->length - 1);
	deleteArrayList(index);
	updateFiles(sih);
}

void updateFiles(SecondaryIndexHandler *sih) {
	int i, j;
	for(i = 0 ; i < sih->files->length ; i++) {
		ArrayList *index = (ArrayList *) getArrayListObject(sih->index, i);
		Field *f = (Field *) getArrayListObject(sih->fields, i);
		BinaryFile *bfFiles = (BinaryFile *) getArrayListObject(sih->files, i);
		overwriteBinaryFile(bfFiles);
		seekBinaryFile(bfFiles, 0L);
		if(index->length > 1) {
			for(j = 0 ; j < index->length - 1 ; j++) {
				SecondaryIndex *si1 = (SecondaryIndex *) getArrayListObject(index, j);
				SecondaryIndex *si2 = (SecondaryIndex *) getArrayListObject(index, j + 1);
				if(compareSecondaryIndex(si1, si2, f->type)) {
					saveIndex(sih, si1, i);
					if(j == index->length - 2) saveIndex(sih, si2, i);
				} else j = saveDuplicated(sih, si1, si2, i, j);
			}
		} else saveIndex(sih, (SecondaryIndex *) getArrayListObject(index, 0), i);
	}
}

void saveIndex(SecondaryIndexHandler *sih, SecondaryIndex *si, int i) {
	Field *f = (Field *) getArrayListObject(sih->fields, i);
	BinaryFile *bfFiles = (BinaryFile *) getArrayListObject(sih->files, i);
	BinaryFileWriter *bfw = newBinaryFileWriter(bfFiles, DELIMITER);
	long offset = getStreamOffset(bfFiles);
	if(!strcmp(f->type, INT)) {
		int *p = (int *) si->value;
		writeInt(bfw, *p, offset);
		offset += sizeof(int);
	} else if(!strcmp(f->type, LONG)) {
		long *p = (long *) si->value;
		writeLong(bfw, *p, offset);
		offset += sizeof(long);
	} else if(!strcmp(f->type, FLOAT)) {
		float *p = (float *) si->value;
		writeFloat(bfw, *p, offset);
		offset += sizeof(float);
	} else if(!strcmp(f->type, DOUBLE)) {
		double *p = (double *) si->value;
		writeDouble(bfw, *p, offset);
		offset += sizeof(double);
	} else if(!strcmp(f->type, CHAR)) {
		char *p = (char *) si->value;
		writeChar(bfw, *p, offset);
		offset += sizeof(char);
	} else if(!strcmp(f->type, STRING)) {
		char *p = (char *) si->value;
		writeString(bfw, p, offset);
		offset += (strlen(p) + 1);
	}

	writeLong(bfw, si->recordOffset, offset);
	offset += sizeof(long);
	writeLong(bfw, si->nextOffset, offset);
	offset += sizeof(long);

	deleteBinaryFileWriter(bfw);

	seekBinaryFile(bfFiles, offset);
}

int saveDuplicated(SecondaryIndexHandler *sih, SecondaryIndex *si1, SecondaryIndex *si2, int i, int j) {
	//Field *f = (Field *) getArrayListObject(sih->fields, i);
	//BinaryFile *bfFiles = (BinaryFile *) getArrayListObject(sih->invertedLists, i);
	return j;//(j - 1);
}

ArrayList *createNewSecondaryIndex(SecondaryIndexHandler *sih, ArrayList *secondaryKeys, long recordOffset){
	ArrayList *index = newArrayList();
	int i;
	for(i = 0 ; i < secondaryKeys->length ; i++) {
		Field *f = (Field *) getArrayListObject(sih->fields, i);
		char *type = f->type;
		void *p = NULL;
		char *string = (char *) getArrayListObject(secondaryKeys, i);
		if(!strcmp(type, INT)) {
			int number = stringToInt(string);
			p = (int *) realloc(p, sizeof(int));
			memcpy(p, &number, sizeof(int));
		} else if(!strcmp(type, LONG)) {
			long number = stringToLong(string);
			p = (long *) realloc(p, sizeof(long));
			memcpy(p, &number, sizeof(long));
		} else if(!strcmp(type, FLOAT)) {
			float number = stringToFloat(string);
			p = (float *) realloc(p, sizeof(float));
			memcpy(p, &number, sizeof(float));
		} else if(!strcmp(type, DOUBLE)) {
			double number = stringToDouble(string);
			p = (double *) realloc(p, sizeof(double));
			memcpy(p, &number, sizeof(double));
		} else if(!strcmp(type, CHAR)) {
			p = (char *) realloc(p, sizeof(char));
			memcpy(p, string, sizeof(char));
		} else if(!strcmp(type, STRING)) {
			char *temp = (char *) malloc(sizeof(char)*(strlen(string) + 1));
			memcpy(temp, string, sizeof(char)*(strlen(string)));
			temp[strlen(string)] = '\0';
			p = (char *) temp;
		}
		SecondaryIndex *si = newSecondaryIndex(p, recordOffset, NO_NEXT);
		setArrayListObject(index, (SecondaryIndex *) si, i);
	}
	return index;
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

int compareSecondaryIndex(SecondaryIndex *si1, SecondaryIndex *si2, char *type) {
	if(!strcmp(type, INT)) return(compareIntSecondaryIndex(si1, si2));
	else if(!strcmp(type, LONG)) return(compareLongSecondaryIndex(si1, si2));
	else if(!strcmp(type, FLOAT)) return(compareFloatSecondaryIndex(si1, si2));
	else if(!strcmp(type, DOUBLE)) return(compareDoubleSecondaryIndex(si1, si2));
	else if(!strcmp(type, CHAR)) return(compareCharSecondaryIndex(si1, si2));
	else if(!strcmp(type, STRING)) return(compareStringSecondaryIndex(si1, si2));
	return 0;
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

SecondaryIndex *selectSecondaryIndex(BinaryFile *bf, long offset, char* type) {
	BinaryFileReader *bfr = newBinaryFileReader(bf, DELIMITER);
	void *p = NULL;
	if(!strcmp(type, INT)) {
		int number = readInt(bfr, offset);
		p = (int *) realloc(p, sizeof(int));
		memcpy(p, &number, sizeof(int));
	} else if(!strcmp(type, LONG)) {
		long number = readLong(bfr, offset);
		p = (long *) realloc(p, sizeof(long));
		memcpy(p, &number, sizeof(long));
	} else if(!strcmp(type, FLOAT)) {
		float number = readFloat(bfr, offset);
		p = (float *) realloc(p, sizeof(float));
		memcpy(p, &number, sizeof(float));
	} else if(!strcmp(type, DOUBLE)) {
		double number = readDouble(bfr, offset);
		p = (double *) realloc(p, sizeof(double));
		memcpy(p, &number, sizeof(double));
	} else if(!strcmp(type, CHAR)) {
		char character = readChar(bfr, offset);
		p = (char *) realloc(p, sizeof(char));
		memcpy(p, &character, sizeof(char));
	} else if(!strcmp(type, STRING)) {
		p = (char *) readString(bfr, offset);
	}
	long recordOffset = readLong(bfr, getStreamOffset(bfr->bf));
	long nextOffset = readLong(bfr, getStreamOffset(bfr->bf));
	deleteBinaryFileReader(bfr);

	return newSecondaryIndex(p, recordOffset, nextOffset);
}

void displayIndex(SecondaryIndexHandler *sih) {
	ArrayList *columnName = newArrayList();
	setArrayListObject(columnName, (char *) RECORDOFFSET, columnName->length);
	setArrayListObject(columnName, (char *) NEXTOFFSET, columnName->length);
	ArrayList *rows = newArrayList();
	int i, j;
	for(i = 0 ; i < sih->fields->length ; i++) {
		Field *f = (Field *) getArrayListObject(sih->fields, i);
		setArrayListObject(columnName, (char *) f->name, 0);
		ArrayList *index = (ArrayList *) getArrayListObject(sih->index, i);
		for(j = 0 ; j < index->length ; j++) {
			SecondaryIndex *si = (SecondaryIndex *) getArrayListObject(index, j);
			ArrayList *row = secondaryIndexToString(si, f->type);
			setArrayListObject(rows, row, rows->length);
		}

		displayIndexRow(columnName, rows);

		removeArrayListObjectFromPosition(columnName, 0);
	}
	deleteArrayList(rows);
	while(columnName->length > 0) removeArrayListObjectFromPosition(columnName, columnName->length - 1);
	deleteArrayList(columnName);
}

void displayIndexRow(ArrayList *columnName, ArrayList *rows) {
	int j;
	TableView *tv = newTableView(columnName);
	printTableHeader(tv);
	for(j = 0 ; j < rows->length ; j++) printTableRow(tv, (ArrayList *) getArrayListObject(rows, j));
	deleteTableView(tv);

	while(rows->length > 0) {
		ArrayList *row = getArrayListObject(rows, rows->length - 1);
		removeArrayListObjectFromPosition(rows, rows->length - 1);
		while(row->length > 0) {
			char * string = getArrayListObject(row, row->length - 1);
			removeArrayListObjectFromPosition(row, row->length - 1);
			free(string);
		}
		deleteArrayList(row);
	}
}

ArrayList *secondaryIndexToString(SecondaryIndex *si, char *type) {
	char *string = NULL;
	if(!strcmp(type, INT)) {
		int *p = (int *) si->value;
		string = intToString(*p);
	}
	else if(!strcmp(type, LONG)) {
		long *p = (long *) si->value;
		string = longToString(*p);
	}
	else if(!strcmp(type, FLOAT)) {
		float *p = (float *) si->value;
		string = floatToString(*p);
	}
	else if(!strcmp(type, DOUBLE)) {
		double *p = (double *) si->value;
		string = doubleToString(*p);
	}
	else if(!strcmp(type, CHAR)) {
		string = (char *) realloc(string, sizeof(char)*(strlen(si->value) + 1));
		memcpy(string, si->value, sizeof(char)*strlen(si->value));
		string[strlen(si->value)] = '\0';
	}
	else if(!strcmp(type, STRING)) {
		string = (char *) realloc(string, sizeof(char)*(strlen(si->value) + 1));
		memcpy(string, si->value, sizeof(char)*strlen(si->value));
		string[strlen(si->value)] = '\0';
	}
	ArrayList *row = newArrayList();
	setArrayListObject(row, string, row->length);
	setArrayListObject(row, (char *) longToString(si->recordOffset), row->length);
	setArrayListObject(row, (char *) longToString(si->nextOffset), row->length);
	return row;
}

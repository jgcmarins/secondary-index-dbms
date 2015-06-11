
/*
* 
* Thu Jun  4 17:44:38 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/BinaryFileReader.h"

BinaryFileReader *newBinaryFileReader(BinaryFile *bf, char delimiter) {
	BinaryFileReader *bfr = (BinaryFileReader *) malloc(sizeof(BinaryFileReader));
	bfr->bf = bf;
	bfr->delimiter = delimiter;

	return bfr;
}

void deleteBinaryFileReader(BinaryFileReader *bfr) {
	if(bfr->bf != NULL) bfr->bf = NULL;
	if(bfr != NULL) free(bfr);
}

int readInt(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	int number;
	fread(&number, sizeof(int), 1, bfr->bf->stream);
	return number;
}

long readLong(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	long number;
	fread(&number, sizeof(long), 1, bfr->bf->stream);
	return number;
}

float readFloat(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	float number;
	fread(&number, sizeof(float), 1, bfr->bf->stream);
	return number;
}

double readDouble(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	double number;
	fread(&number, sizeof(double), 1, bfr->bf->stream);
	return number;
}

char readChar(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	char character;
	fread(&character, sizeof(char), 1, bfr->bf->stream);
	return character;
}

char *readString(BinaryFileReader *bfr, long offset) {
	seekBinaryFile(bfr->bf, offset);
	char *buffer = NULL;
	int c, i = 0;
	do {
		c = fgetc(bfr->bf->stream);
		buffer = (char *) realloc(buffer, sizeof(char)*(i+1));
		buffer[i++] = c;
	} while(c != bfr->delimiter);

	buffer[i-1] = '\0';

	return buffer;
}

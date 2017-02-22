
/*
* 
* Thu Jun  4 16:49:31 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/BinaryFileWriter.h"

BinaryFileWriter *newBinaryFileWriter(BinaryFile *bf, char delimiter) {
	BinaryFileWriter *bfw = (BinaryFileWriter *) malloc(sizeof(BinaryFileWriter));
	bfw->bf = bf;
	bfw->delimiter = delimiter;
	return bfw;
}

void deleteBinaryFileWriter(BinaryFileWriter *bfw) {
	if(bfw->bf != NULL) bfw->bf = NULL;
	if(bfw != NULL) free(bfw);
}

void writeInt(BinaryFileWriter *bfw, int number, long offset) {
	seekBinaryFile(bfw->bf, offset);
	fwrite(&number, sizeof(int), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

void writeLong(BinaryFileWriter *bfw, long number, long offset) {
	seekBinaryFile(bfw->bf, offset);
	fwrite(&number, sizeof(long), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

void writeFloat(BinaryFileWriter *bfw, float number, long offset) {
	seekBinaryFile(bfw->bf, offset);
	fwrite(&number, sizeof(float), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

void writeDouble(BinaryFileWriter *bfw, double number, long offset) {
	seekBinaryFile(bfw->bf, offset);
	fwrite(&number, sizeof(double), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

void writeChar(BinaryFileWriter *bfw, char character, long offset) {
	seekBinaryFile(bfw->bf, offset);
	fwrite(&character, sizeof(char), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

void writeString(BinaryFileWriter *bfw, char *string, long offset) {
	size_t length = strlen(string);
	seekBinaryFile(bfw->bf, offset);
	fwrite(string, sizeof(char)*length, 1, bfw->bf->stream);
	fwrite(&bfw->delimiter, sizeof(char), 1, bfw->bf->stream);
	rewind(bfw->bf->stream);
}

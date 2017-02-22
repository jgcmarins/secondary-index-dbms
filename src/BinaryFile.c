
/*
* 
* Thu Jun  4 16:18:39 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/BinaryFile.h"

BinaryFile *newBinaryFile(char *fileName) {
	BinaryFile *bf = (BinaryFile *) malloc(sizeof(BinaryFile));
	bf->stream = NULL;
	bf->fileName = fileName;
	bf->fileSize = 0L;

	openBinaryFile(bf);

	return bf;
}

void deleteBinaryFile(BinaryFile *bf) {
	if(bf->stream != NULL) fclose(bf->stream);
	if(bf->fileName != NULL) bf->fileName = NULL;
	if(bf != NULL) free(bf);
}

void openBinaryFile(BinaryFile *bf) {
	bf->stream = fopen((const char *) bf->fileName, "r+b");
	if(bf->stream == NULL) {
		bf->stream = fopen((const char *) bf->fileName, "w+b");
		if(bf->stream == NULL) exit(1); //error 1
	}
	updateBinaryFileSize(bf);
}

void updateBinaryFileSize(BinaryFile *bf) {
	long current = getStreamOffset(bf);
	fseek(bf->stream, 0L, SEEK_END);
	bf->fileSize = getStreamOffset(bf);
	seekBinaryFile(bf, current);
}

void seekBinaryFile(BinaryFile *bf, long offset) {
	fseek(bf->stream, offset, SEEK_SET);
}

long getBinaryFileSize(BinaryFile *bf) {
	updateBinaryFileSize(bf);
	return bf->fileSize;
}

long getStreamOffset(BinaryFile *bf) {
	return ftell(bf->stream);
}

void overwriteBinaryFile(BinaryFile *bf) {
	if(bf->stream != NULL) fclose(bf->stream);
	
	bf->stream = fopen((const char *) bf->fileName, "w+b");
	if(bf->stream == NULL) exit(1); //error 1
	updateBinaryFileSize(bf);
}


/*
* 
* Thu Jun  4 17:39:16 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _BINARYFILEREADER_H_
#define _BINARYFILEREADER_H_

#include "BinaryFile.h"

typedef struct BinaryFileReader {
	BinaryFile *bf;
	char delimiter;
} BinaryFileReader;

BinaryFileReader *newBinaryFileReader(BinaryFile *bf, char delimiter);

void deleteBinaryFileReader(BinaryFileReader *bfr);

int readInt(BinaryFileReader *bfr, long offset);

long readLong(BinaryFileReader *bfr, long offset);

float readFloat(BinaryFileReader *bfr, long offset);

double readDouble(BinaryFileReader *bfr, long offset);

char readChar(BinaryFileReader *bfr, long offset);

char *readString(BinaryFileReader *bfr, long offset);

#endif

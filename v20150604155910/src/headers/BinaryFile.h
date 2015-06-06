
/*
* 
* Thu Jun  4 16:12:58 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _BINARYFILE_H_
#define _BINARYFILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct BinaryFile {
	FILE *stream;
	char *fileName;
	long fileSize;
	long currentOffset;
} BinaryFile;

BinaryFile *newBinaryFile(char *fileName);

void deleteBinaryFile(BinaryFile *bf);

void openBinaryFile(BinaryFile *bf);

void updateBinaryFileSize(BinaryFile *bf);

void seekBinaryFile(BinaryFile *bf, long offset);

long getBinaryFileSize(BinaryFile *bf);

long getStreamOffset(BinaryFile *bf);

#endif


/*
* 
* Thu Jun  4 16:44:35 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _BINARYFILEWRITER_H_
#define _BINARYFILEWRITER_H_

#include "BinaryFile.h"

typedef struct BinaryFileWriter {
	BinaryFile *bf;
	char delimiter;
} BinaryFileWriter;

BinaryFileWriter *newBinaryFileWriter(BinaryFile *bf, char delimiter);

void deleteBinaryFileWriter(BinaryFileWriter *bfw);

void writeInt(BinaryFileWriter *bfw, int number, long offset);

void writeLong(BinaryFileWriter *bfw, long number, long offset);

void writeFloat(BinaryFileWriter *bfw, float number, long offset);

void writeDouble(BinaryFileWriter *bfw, double number, long offset);

void writeChar(BinaryFileWriter *bfw, char character, long offset);

void writeString(BinaryFileWriter *bfw, char *string, long offset);

#endif

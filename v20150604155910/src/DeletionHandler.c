
/*
* 
* Tue Jun 16 04:19:49 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/DeletionHandler.h"

DeletionHandler *newDeletionHandler(Table *t) {
	DeletionHandler *dh = (DeletionHandler *) malloc(sizeof(DeletionHandler));
	dh->t = t;
	dh->bfw = newBinaryFileWriter(getTableFile(t), DELIMITER);
	dh->bfr = newBinaryFileReader(getTableFile(t), DELIMITER);

	return dh;
}

void deleteDeletionHandler(DeletionHandler *dh) {
	if(dh->bfr != NULL) deleteBinaryFileReader(dh->bfr);
	if(dh->bfw != NULL) deleteBinaryFileWriter(dh->bfw);
	if(dh->t != NULL) dh->t = NULL;
	if(dh != NULL) free(dh);
}

void deleteByOffset(DeletionHandler *dh, long offset) {
	long head = readLong(dh->bfr, 0L); // read head

	seekBinaryFile(dh->bfr->bf, offset); // point to offset
	int size = readInt(dh->bfr, getStreamOffset(dh->bfr->bf));
	size *= -1; // delete

	seekBinaryFile(dh->bfw->bf, offset);
	writeInt(dh->bfw, size, getStreamOffset(dh->bfw->bf)); // write deleted

	seekBinaryFile(dh->bfw->bf, (offset + sizeof(int)));
	writeLong(dh->bfw, head, getStreamOffset(dh->bfw->bf)); // write head

	writeLong(dh->bfw, offset, 0L); // updated head with new offset
}

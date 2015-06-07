
/*
* 
* Thu Jun  4 16:00:03 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/InsertionHandler.h"

char *inputReader() {
	char *buffer = NULL;
	int c, i = 0;

	do {
		c = fgetc(stdin);
		buffer = (char *) realloc(buffer, sizeof(char)*(i+1));
		buffer[i++] = c;
	} while((c != 10) && !feof(stdin));

	buffer[i-1] = '\0';

	return buffer;
}

int main(int argc, char *argv[]) {

	char *fileName = inputReader();
	Table *t = newTable(fileName);
	InsertionHandler *ih = newInsertionHandler(t);

	/*while(1){
		char *name = inputReader();
		if(name[0] == '\0') {
			free(name);
			break;
		}

		char *type = inputReader();
		Field *f = newField(name, type);
		addNewField(ih->t->fh, f);
	}*/

	ArrayList *records = newArrayList();
	while(1){
		char *record = inputReader();
		if(record[0] == '\0') {
			free(record);
			break;
		}

		//printf("\"%s\"\n", record);

		setArrayListObject(records, (char *) record, records->length);
	}

	insert(ih, records);

	while(records->length > 0) {
		char *record = getArrayListObject(records, records->length - 1);
		removeArrayListObjectFromPosition(records, records->length - 1);
		free(record);
	}
	deleteArrayList(records);

	displayFields(t);

	deleteInsertionHandler(ih);
	deleteTable(t);
	free(fileName);

	return 0;
}

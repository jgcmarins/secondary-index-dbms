
/*
* 
* Thu Jun  4 16:00:03 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/InsertionHandler.h"
#include "headers/SelectionHandler.h"

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
	SelectionHandler *sh = newSelectionHandler(t);

	/*while(1) {
		char *name = inputReader();
		if(name[0] == '\0') {
			free(name);
			break;
		}

		char *type = inputReader();
		char *key = inputReader();
		Field *f = newField(name, type, key);
		addNewField(ih->t->fh, f);
	}*/

	/*ArrayList *records = newArrayList();
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
	deleteArrayList(records);*/

	ArrayList *records = selectAll(sh);
	int i = 0;
	while(i < records->length) {
		ArrayList *record = getArrayListObject(records, i);

		printf("\"%s\"\n", (char *) getArrayListObject(record, 0));
		printf("\"%s\"\n", (char *) getArrayListObject(record, 1));
		printf("\"%s\"\n", (char *) getArrayListObject(record, 2));
		printf("\"%s\"\n", (char *) getArrayListObject(record, 3));
		int *int1 = (int *) getArrayListObject(record, 4);
		int *int2 = (int *) getArrayListObject(record, 5);
		long *long1 = (long *) getArrayListObject(record, 6);
		printf("\"%d\"\n", *int1);
		printf("\"%d\"\n", *int2);
		printf("\"%ld\"\n", *long1);


		while(record->length > 0) {
			void *p = getArrayListObject(record, record->length - 1);
			removeArrayListObjectFromPosition(record, record->length - 1);
			free(p);
		}
		i++;
	}

	while(records->length > 0) {
		ArrayList *record = (ArrayList *) getArrayListObject(records, records->length - 1);
		removeArrayListObjectFromPosition(records, records->length - 1);
		deleteArrayList(record);
	}
	deleteArrayList(records);

	displayFields(t);

	deleteSelectionHandler(sh);
	deleteInsertionHandler(ih);
	deleteTable(t);
	free(fileName);

	return 0;
}

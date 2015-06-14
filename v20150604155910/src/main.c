
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

void insertFields(InsertionHandler *ih) {
	while(1) {
		char *name = inputReader();
		if(name[0] == '\0') {
			free(name);
			break;
		}

		char *type = inputReader();
		char *key = inputReader();
		Field *f = newField(name, type, key);
		addNewField(ih->t->fh, f);
	}
}

void insertRecord(InsertionHandler *ih) {
	ArrayList *record = newArrayList();
	while(1) {
		char *input = inputReader();
		if(input[0] == '\0') {
			free(input);
			break;
		}

		setArrayListObject(record, (char *) input, record->length);
	}

	insert(ih, record);

	while(record->length > 0) {
		char *string = getArrayListObject(record, record->length - 1);
		removeArrayListObjectFromPosition(record, record->length - 1);
		free(string);
	}
	deleteArrayList(record);
}

void printRecords(SelectionHandler *sh) {
	ArrayList *names = newArrayList();
	int i;
	for(i = 0 ; i < sh->t->fh->fields->length ; i++)
		setArrayListObject(names, (char *) getFieldName(sh->t->fh, i), i);
	TableView *tv = newTableView(names);
	printTableHeader(tv);

	ArrayList *records = selectAll(sh);
	i = 0;
	while(i < records->length) {
		ArrayList *record = getArrayListObject(records, i);

		ArrayList *strings = rowToString(sh->t, record);

		printTableRow(tv, strings);

		while(strings->length > 0) {
			void *p = getArrayListObject(strings, strings->length - 1);
			removeArrayListObjectFromPosition(strings, strings->length - 1);
			free(p);
		}
		deleteArrayList(strings);

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

	while(names->length > 0) removeArrayListObjectFromPosition(names, names->length - 1);
	deleteArrayList(names);

	deleteTableView(tv);
}

int main(int argc, char *argv[]) {

	char *fileName = inputReader();
	Table *t = newTable(fileName);
	InsertionHandler *ih = newInsertionHandler(t);
	SelectionHandler *sh = newSelectionHandler(t);

	//insertFields(ih);

	//displayFields(t);

	insertRecord(ih);

	printRecords(sh);

	displayIndex(t->sih);

	deleteSelectionHandler(sh);
	deleteInsertionHandler(ih);
	deleteTable(t);
	free(fileName);

	return 0;
}

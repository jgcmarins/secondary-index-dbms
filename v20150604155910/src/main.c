
/*
* 
* Thu Jun  4 16:00:03 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Database.h"

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
	Database *db = newDatabase(fileName);
	//createNewTable(db);

	insertNewRecordIntoTable(db);

	printRecords(getSelectionHandler(db->tm, "tweets"));

	Table *t = getTable(db->tm, "tweets");
	displayIndex(t->sih);

	deleteDatabase(db);
	free(fileName);

	return 0;
}

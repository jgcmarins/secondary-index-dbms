
/*
* 
* Sun Jun 14 22:35:49 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Database.h"

Database *newDatabase(char *dbName) {
	buildDirectory(dbName);
	Database *db = (Database *) malloc(sizeof(Database));
	db->dbName = buildDabataseName(dbName);
	db->tm = newTableManager(db->dbName);
	
	return db;
}

void deleteDatabase(Database *db) {
	if(db->tm != NULL) deleteTableManager(db->tm);
	if(db->dbName != NULL) free(db->dbName);

	if(db != NULL) free(db);
}

void buildDirectory(char *fileName) {
	char *string = (char *) malloc(sizeof(char)*(strlen(MKDIR) + strlen(fileName) + 1));
	memcpy(string, MKDIR, sizeof(char)*(strlen(MKDIR)));
	string[strlen(MKDIR)] = '\0';
	string = strcat(string, fileName);
	system(string);
	free(string);
}

char *buildDabataseName(char *fileName) { // tenso
	char *string = (char *) malloc(sizeof(char)*(strlen(PATH) + strlen(fileName) + strlen(SLASH) + 1));
	memcpy(string, PATH, sizeof(char)*(strlen(PATH)));
	string[strlen(PATH)] = '\0';
	string = strcat(string, fileName);
	string = strcat(string, SLASH);
	return string;
}

void createNewTable(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	createTable(db->tm, tableName);
	insertFields(getTable(db->tm, tableName));
	db->tm = updateTableManager(db->tm, db->dbName);
	free(tableName);
}

void insertFields(Table *t) {
	printf("Insert fields, using this order:\n");
	printf("name\ntype (int, long, float, double, char, string)\nkey (normal key, primary key, secondary key)\n");
	printf("Hit <ENTER> between name, type and key. Hit <ENTER> when done.\n\n");

	while(1) {
		char *name = inputReader();
		if(name[0] == '\0') {
			free(name);
			break;
		}

		char *type = inputReader();
		char *key = inputReader();
		Field *f = newField(name, type, key);
		addNewField(t->fh, f);
	}
}

void insertNewRecordIntoTable(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	Table *t = getTable(db->tm, tableName);
	if(t != NULL) {
		ArrayList *record = readRecord(t);
		insertIntoTable(db->tm, tableName, record);
		while(record->length > 0) {
			char *string = getArrayListObject(record, record->length - 1);
			removeArrayListObjectFromPosition(record, record->length - 1);
			free(string);
		}
		deleteArrayList(record);
	} else printf("Table does not exists.\n");

	free(tableName);
}

ArrayList *readRecord(Table *t) {
	displayFields(t);
	printf("Insert data following fields order. Hit <ENTER> between each data. Hit <ENTER> when done.\n");

	ArrayList *record = newArrayList();
	while(1) {
		char *input = inputReader();
		if(input[0] == '\0') {
			free(input);
			break;
		}
		setArrayListObject(record, (char *) input, record->length);
	}
	return record;
}

void browseAllRecords(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	SelectionHandler *sh = getSelectionHandler(db->tm, tableName);
	if(sh != NULL) {
		ArrayList *names = buildTableHeader(sh->t);
		TableView *tv = newTableView(names);
		printTableHeader(tv);
		ArrayList *records = selectAll(sh);
		printRows(tv, sh->t, records);

		while(records->length > 0) {
			ArrayList *record = (ArrayList *) getArrayListObject(records, records->length - 1);
			removeArrayListObjectFromPosition(records, records->length - 1);
			deleteArrayList(record);
		}
		deleteArrayList(records);

		while(names->length > 0) removeArrayListObjectFromPosition(names, names->length - 1);
		deleteArrayList(names);

		deleteTableView(tv);
	} else printf("Table does not exists.\n");
	free(tableName);
}

ArrayList *buildTableHeader(Table *t) {
	ArrayList *names = newArrayList();
	int i;
	for(i = 0 ; i < t->fh->fields->length ; i++) {
		setArrayListObject(names, (char *) getFieldName(t->fh, i), i);
	}
	return names;
}

void printRows(TableView *tv, Table *t, ArrayList *records) {
	int i;
	for(i = 0 ; i < records->length ; i++) printRow(tv, t, records, i);
}

void printRow(TableView *tv, Table *t, ArrayList *records, int i) {
	ArrayList *record = getArrayListObject(records, i);
	ArrayList *strings = rowToString(t, record);
	printTableRow(tv, strings);

	while(strings->length > 0) {
		char *string = (char *) getArrayListObject(strings, strings->length - 1);
		removeArrayListObjectFromPosition(strings, strings->length - 1);
		free(string);
	}
	deleteArrayList(strings);

	while(record->length > 0) {
		void *p = getArrayListObject(record, record->length - 1);
		removeArrayListObjectFromPosition(record, record->length - 1);
		free(p);
	}
}

void browseOneAtATime(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	SelectionHandler *sh = getSelectionHandler(db->tm, tableName);
	if(sh != NULL) {
		ArrayList *names = buildTableHeader(sh->t);
		TableView *tv = newTableView(names);
		ArrayList *records = selectAll(sh);

		int i;
		for(i = 0 ; i < records->length ; i++) {
			printTableHeader(tv);
			printRow(tv, sh->t, records, i);
			if(i < records->length - 1) {
				printf("Hit <ENTER> to next\n");
				char *input = inputReader();
				free(input);
			}
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
	} else printf("Table does not exists.\n");
	free(tableName);
}

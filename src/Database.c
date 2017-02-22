
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
	printf("Insert fields, using this order:\n\n");
	printf("name\ntype (int, long, float, double, char, string)\nkey (normal key, primary key, secondary key)\n\n");
	printf("Type one input per line. Hit <ENTER> when done.\n\n");

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
	printf("\n\nInsert data following fields order.\nType one data per line. Hit <ENTER> when done.\n\nInput:\n");

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
		ArrayList *records = selectAllFromTable(db->tm, tableName);

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

void browseByField(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	SelectionHandler *sh = getSelectionHandler(db->tm, tableName);
	if(sh != NULL) {
		displayFields(sh->t);
		printf("Insert field name: ");
		char *fieldName = inputReader();
		int position = indexOfField(sh->t->sih, fieldName);
		if(position != -1) {
			printf("Insert value: ");
			char *value = inputReader();
			SecondaryIndex *si = createTemporarySecondaryIndex(sh->t->sih, position, value);
			ArrayList *records = selectBySecondaryIndexFromTable(db->tm, tableName, position, si);

			ArrayList *names = buildTableHeader(sh->t);
			TableView *tv = newTableView(names);
			printTableHeader(tv);
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
			deleteSecondaryIndex(si);
			free(value);
		} else printf("Field does not apply.\n");
		free(fieldName);
	} else printf("Table does not exists.\n");
	free(tableName);
}

void findMultipleFields(Database *db, int op) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	SelectionHandler *sh = getSelectionHandler(db->tm, tableName);
	if(sh != NULL) {
		displayFields(sh->t);
		ArrayList *index = readMultiplesFields(sh);
		ArrayList *records = NULL;

		if(op == 6) records = match(sh, index);
		else if(op == 7) records = merge(sh, index);

		if(records->length > 0) {
			ArrayList *names = buildTableHeader(sh->t);
			TableView *tv = newTableView(names);
			printTableHeader(tv);
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
		} else {
			deleteArrayList(records);
			printf("No record found.\n");
		}

		while(index->length > 0) {
			SecondaryIndex *si = (SecondaryIndex *) getArrayListObject(index, index->length - 1);
			removeArrayListObjectFromPosition(index, index->length - 1);
			deleteSecondaryIndex(si);
		}
		deleteArrayList(index);

	} else printf("Table does not exists.\n");
	free(tableName);
}


ArrayList *readMultiplesFields(SelectionHandler *sh) {
	ArrayList *index = newArrayList();
	while(1) {
		printf("Insert field name (if done, hit <ENTER>): ");
		char *input = inputReader();
		if(input[0] == '\0') {
			free(input);
			break;
		}

		int position = indexOfField(sh->t->sih, input);
		if(position != -1) {
			printf("Insert value: ");
			char *value = inputReader();
			SecondaryIndex *si = createTemporarySecondaryIndex(sh->t->sih, position, value);
			ArrayList *keys = searchSecondaryKey(sh->t->sih, position, si);
			setArrayListObject(index, keys, index->length);
			deleteSecondaryIndex(si);
			free(value);
		} else printf("Field does not apply.\n");


		free(input);
	}

	return index;
}

void deleteByField(Database *db) {
	printf("Insert table name: ");
	char *tableName = inputReader();
	DeletionHandler *dh = getDeletionHandler(db->tm, tableName);
	if(dh != NULL) {
		displayFields(dh->t);
		printf("Insert field name: ");
		char *fieldName = inputReader();
		int position = indexOfField(dh->t->sih, fieldName);
		if(position != -1) {
			printf("Insert value: ");
			char *value = inputReader();
			SecondaryIndex *si = createTemporarySecondaryIndex(dh->t->sih, position, value);
			ArrayList *records = selectBySecondaryIndexFromTable(db->tm, tableName, position, si);

			ArrayList *names = buildTableHeader(dh->t);
			TableView *tv = newTableView(names);
			printTableHeader(tv);
			int i;
			for(i = 0 ; i < records->length ; i++) {
				printf("%d\n", i+1);
				printRow(tv, dh->t, records, i);
			}

			printf("\n\nInsert the number of the record to be deleted: ");
			char *chosen = inputReader();
			int number = stringToInt(chosen);
			free(chosen);

			if((number >= 1) && (number <= records->length)) {
				number--;
				executeDelete(db, tableName, position, si, number);
			} else printf("Invalid record.\n");

			while(records->length > 0) {
				ArrayList *record = (ArrayList *) getArrayListObject(records, records->length - 1);
				removeArrayListObjectFromPosition(records, records->length - 1);
				deleteArrayList(record);
			}
			deleteArrayList(records);

			while(names->length > 0) removeArrayListObjectFromPosition(names, names->length - 1);
			deleteArrayList(names);

			deleteTableView(tv);
			deleteSecondaryIndex(si);
			free(value);
		} else printf("Field does not apply.\n");
		free(fieldName);
	} else printf("Table does not exists.\n");
	free(tableName);
}

void executeDelete(Database *db, char *tableName, int position, SecondaryIndex *si, int i) {
	SelectionHandler *sh = getSelectionHandler(db->tm, tableName);
	ArrayList *index = searchSecondaryKey(sh->t->sih, position, si);
	SecondaryIndex *recordIndex = (SecondaryIndex *) getArrayListObject(index, i);
	deleteBySecondaryIndexFromTable(db->tm, tableName, position, recordIndex->recordOffset);
	while(index->length > 0) removeArrayListObjectFromPosition(index, index->length - 1);
	deleteArrayList(index);
}


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
	char *dbName = inputReader();
	createTable(db->tm, dbName);
	insertFields(getTable(db->tm, dbName));
	free(dbName);
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

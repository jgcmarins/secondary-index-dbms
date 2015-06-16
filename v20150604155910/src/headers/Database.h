
/*
* 
* Sun Jun 14 21:11:42 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "TableManager.h"

#define PATH "files/"
#define SLASH "/"
#define MKDIR "mkdir -p -v files/"

typedef struct Database {
	char *dbName;
	TableManager *tm;
} Database;

Database *newDatabase(char *dbName);

void deleteDatabase(Database *db);

void buildDirectory(char *fileName);

char *buildDabataseName(char *fileName);

void createNewTable(Database *db);

void insertFields(Table *t);

void insertNewRecordIntoTable(Database *db);

ArrayList *readRecord(Table *t);

void browseAllRecords(Database *db);

ArrayList *buildTableHeader(Table *t);

void printRows(TableView *tv, Table *t, ArrayList *records);

void printRow(TableView *tv, Table *t, ArrayList *records, int i);

void browseOneAtATime(Database *db);

void browseByField(Database *db);

void findMultipleFields(Database *db, int op);

ArrayList *readMultiplesFields();

void deleteByField(Database *db);

void executeDelete(Database *db, char *tableName, int position, SecondaryIndex *si, int number);

#endif

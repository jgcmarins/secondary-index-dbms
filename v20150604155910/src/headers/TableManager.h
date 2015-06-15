
/*
* 
* Sun Jun 14 17:50:52 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _TABLEMANAGER_H_
#define _TABLEMANAGER_H_

#include "Table.h"
#include "InsertionHandler.h"
#include "SelectionHandler.h"

#define TABLESEXTENSION ".tables"

typedef struct TableManager {
	char *fileName;
	char *path;
	ArrayList *names;
	ArrayList *tables;
	ArrayList *insert;
	ArrayList *select;
} TableManager;

TableManager *newTableManager(char *fileName);

void deleteTableManager(TableManager *tm);

char *buildNameToTablesFiles(char *fileName, const char *extension);

void selectTables(TableManager *tm);

void buildTableManager(TableManager *tm);

void createNewTable(TableManager *tm, char *tableName);

void saveTables(TableManager *tm);

Table *getTable(TableManager *tm, char *tableName);

InsertionHandler *getInsertionHandler(TableManager *tm, char *tableName);

SelectionHandler *getSelectionHandler(TableManager *tm, char *tableName);

void insertIntoTable(TableManager *tm, char *tableName, ArrayList *record);

ArrayList *selectAllFromTable(TableManager *tm, char *tableName);

ArrayList *selectBySecondaryIndexFromTable(TableManager *tm, char *tableName, int position, SecondaryIndex *si);

#endif

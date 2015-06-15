
/*
* 
* Sun Jun 14 19:09:52 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/TableManager.h"

TableManager *newTableManager(char *fileName) {
	TableManager *tm = (TableManager *) malloc(sizeof(TableManager));
	tm->fileName = buildNameToTablesFiles(fileName, TABLESEXTENSION);
	tm->path = buildNameToTablesFiles(fileName, "");
	tm->names = newArrayList();
	tm->tables = newArrayList();
	tm->insert = newArrayList();
	tm->select = newArrayList();

	buildTableManager(tm);

	return tm;
}

void deleteTableManager(TableManager *tm) {
	if(tm->select != NULL) {
		while(tm->select->length > 0) {
			SelectionHandler *sh = (SelectionHandler *) getArrayListObject(tm->select, tm->select->length - 1);
			removeArrayListObjectFromPosition(tm->select, tm->select->length - 1);
			deleteSelectionHandler(sh);
		}
		deleteArrayList(tm->select);
	}

	if(tm->insert != NULL) {
		while(tm->insert->length > 0) {
			InsertionHandler *ih = (InsertionHandler *) getArrayListObject(tm->insert, tm->insert->length - 1);
			removeArrayListObjectFromPosition(tm->insert, tm->insert->length - 1);
			deleteInsertionHandler(ih);
		}
		deleteArrayList(tm->insert);
	}

	if(tm->tables != NULL) {
		while(tm->tables->length > 0) {
			Table *t = (Table *) getArrayListObject(tm->tables, tm->tables->length - 1);
			removeArrayListObjectFromPosition(tm->tables, tm->tables->length - 1);
			deleteTable(t);
		}
		deleteArrayList(tm->tables);
	}

	if(tm->names != NULL) {
		while(tm->names->length > 0) {
			char *name = (char *) getArrayListObject(tm->names, tm->names->length - 1);
			removeArrayListObjectFromPosition(tm->names, tm->names->length - 1);
			free(name);
		}
		deleteArrayList(tm->names);
	}

	if(tm->path != NULL) free(tm->path);
	if(tm->fileName != NULL) free(tm->fileName);

	if(tm != NULL) free(tm);
}

char *buildNameToTablesFiles(char *fileName, const char *extension) { // tenso
	char *string = (char *) malloc(sizeof(char)*(strlen(fileName) + strlen(extension) + 1));
	memcpy(string, fileName, sizeof(char)*(strlen(fileName)));
	string[strlen(fileName)] = '\0';
	string = strcat(string, extension);
	return string;
}

void selectTables(TableManager *tm) {
	BinaryFile *bf = newBinaryFile(tm->fileName);
	if(getBinaryFileSize(bf) > 0) {
		BinaryFileReader *bfr = newBinaryFileReader(bf, DELIMITER);
		seekBinaryFile(bf, 0L);
		long offset = getStreamOffset(bf);
		while(offset < getBinaryFileSize(bf)) {
			char *fileName = readString(bfr, offset);
			setArrayListObject(tm->names, fileName, tm->names->length);
			offset += (sizeof(char)*(strlen(fileName) + 1));
		}
		deleteBinaryFileReader(bfr);
	}
	deleteBinaryFile(bf);
}

void buildTableManager(TableManager *tm) {
	selectTables(tm);
	if(tm->names > 0) {
		int i;
		for(i = 0 ; i < tm->names->length ; i++) {
			char *tableName = (char *) getArrayListObject(tm->names, i);
			Table *t = newTable(tableName);
			setArrayListObject(tm->tables, (Table *) t, tm->tables->length);
			InsertionHandler *ih = newInsertionHandler(t);
			setArrayListObject(tm->insert, (InsertionHandler *) ih, tm->insert->length);
			SelectionHandler *sh = newSelectionHandler(t);
			setArrayListObject(tm->select, (SelectionHandler *) sh, tm->select->length);
		}
	}
}

void createTable(TableManager *tm, char *tableName) {
	char *fileName = buildNameToTablesFiles(tm->path, tableName);
	setArrayListObject(tm->names, (char *) fileName, tm->names->length);
	Table *t = newTable(fileName);
	setArrayListObject(tm->tables, (Table *) t, tm->tables->length);
	InsertionHandler *ih = newInsertionHandler(t);
	setArrayListObject(tm->insert, (InsertionHandler *) ih, tm->insert->length);
	SelectionHandler *sh = newSelectionHandler(t);
	setArrayListObject(tm->select, (SelectionHandler *) sh, tm->select->length);
	saveTables(tm);
}

void saveTables(TableManager *tm) {
	if(tm->names->length > 0) {
		BinaryFile *bf = newBinaryFile(tm->fileName);
		BinaryFileWriter *bfw = newBinaryFileWriter(bf, DELIMITER);
		overwriteBinaryFile(bf);
		int i;
		for(i = 0 ; i < tm->names->length ; i++) {
			char *fileName = (char *) getArrayListObject(tm->names, i);
			writeString(bfw, fileName, getBinaryFileSize(bf));
		}
		deleteBinaryFileWriter(bfw);
		deleteBinaryFile(bf);
	}
}

Table *getTable(TableManager *tm, char *tableName) {
	char *fileName = buildNameToTablesFiles(tm->path, tableName);
	int i = indexOfArrayListObject(tm->names, (char *) fileName, compareString);
	free(fileName);
	if(i != -1) return (Table *) getArrayListObject(tm->tables, i);
	return NULL;
}

InsertionHandler *getInsertionHandler(TableManager *tm, char *tableName) {
	char *fileName = buildNameToTablesFiles(tm->path, tableName);
	int i = indexOfArrayListObject(tm->names, (char *) fileName, compareString);
	free(fileName);
	if(i != -1) return (InsertionHandler *) getArrayListObject(tm->insert, i);
	return NULL;
}

SelectionHandler *getSelectionHandler(TableManager *tm, char *tableName) {
	char *fileName = buildNameToTablesFiles(tm->path, tableName);
	int i = indexOfArrayListObject(tm->names, (char *) fileName, compareString);
	free(fileName);
	if(i != -1) return (SelectionHandler *) getArrayListObject(tm->select, i);
	return NULL;
}

void insertIntoTable(TableManager *tm, char *tableName, ArrayList *record) {
	InsertionHandler *ih = getInsertionHandler(tm, tableName);
	insert(ih, record);
}

ArrayList *selectAllFromTable(TableManager *tm, char *tableName) {
	SelectionHandler *sh = getSelectionHandler(tm, tableName);
	return selectAll(sh);
}

ArrayList *selectBySecondaryIndexFromTable(TableManager *tm, char *tableName, int position, SecondaryIndex *si) {
	SelectionHandler *sh = getSelectionHandler(tm, tableName);
	return selectBySecondaryIndex(sh, position, si);
}

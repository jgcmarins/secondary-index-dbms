
/*
* 
* Sat Jun  6 00:04:41 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _TABLEVIEW_H_
#define _TABLEVIEW_H_

#include "ArrayList.h"
#include <string.h>

#define BAR '|'
#define HIPHEN '-'
#define PLUS '+'
#define SPACE "     " //5

typedef struct TableView {
	ArrayList *columnsNames;
	ArrayList *columnsLength;
	int max;
} TableView;

TableView *newTableView(ArrayList *columnsNames);

void deleteTableView(TableView *tv);

void setLengths(TableView *tv);

void printSeparationLine(TableView *tv);

void printColumnsNames(TableView *tv);

void printTableHeader(TableView *tv);

void printTableRow(TableView *tv, ArrayList *row);

void calculateBiggestLength(TableView *tv, ArrayList *row);

void printColumn(char *string, int length, int turn);

void completeWithSpace(int n);

#endif

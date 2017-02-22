
/*
* 
* Sat Jun  6 00:23:41 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/TableView.h"

TableView *newTableView(ArrayList *columnsNames) {
	TableView *tv = (TableView *) malloc(sizeof(TableView));
	tv->columnsNames = columnsNames;
	tv->columnsLength = newArrayList();
	setLengths(tv);
	tv->max = 0;

	return tv;
}

void deleteTableView(TableView *tv) {
	if(tv->columnsNames != NULL) tv->columnsNames = NULL;

	if(tv->columnsLength != NULL) {
		while(tv->columnsLength->length > 0) {
			int *p = (int *) getArrayListObject(tv->columnsLength, tv->columnsLength->length - 1);
			removeArrayListObjectFromPosition(tv->columnsLength, tv->columnsLength->length -1);
			free(p);
		}
		deleteArrayList(tv->columnsLength);
	}
	if(tv != NULL) free(tv);
}

void setLengths(TableView *tv) {
	while(tv->columnsLength->length < tv->columnsNames->length) {
		char *columnName = (char *) getArrayListObject(tv->columnsNames, tv->columnsLength->length);
		int length = strlen(columnName);
		length += (2*(strlen(SPACE)));
		int *p = (int *) malloc(sizeof(int));
		memcpy(p, &length, sizeof(int));
		setArrayListObject(tv->columnsLength, (int *) p, tv->columnsLength->length);
	}
}

void printSeparationLine(TableView *tv) {
	int i;
	for(i = 0 ; i < tv->columnsLength->length ; i++) {
		int *p = (int *) getArrayListObject(tv->columnsLength, i);
		int length = *p;
		printf("%c", PLUS);
		int j;
		for(j = 0 ; j < length ; j++) printf("%c", HIPHEN);
	}
	printf("%c\n", PLUS);
}

void printColumnsNames(TableView *tv) {
	int i;
	for(i = 0 ; i < tv->columnsNames->length ; i++) {
		printf("%c%s%s%s", BAR, SPACE, (char *) getArrayListObject(tv->columnsNames, i), SPACE);
	}
	printf("%c\n", BAR);
}

void printTableHeader(TableView *tv) {
	printf("\n\n");
	printSeparationLine(tv);
	printColumnsNames(tv);
	printSeparationLine(tv);
}

void printTableRow(TableView *tv, ArrayList *row) {
	calculateBiggestLength(tv, row);
	int i, turn;
	for(i = 0, turn = 0 ; turn <= tv->max ; i++) {
		char *string = (char *) getArrayListObject(row, i);
		int *p = (int *) getArrayListObject(tv->columnsLength, i);
		int length = *p;
		printColumn(string, length, turn);
		if(i == row->length - 1) {
			i = -1;
			turn++;
			printf("%c\n", BAR);
		}
	}
	tv->max = 0;
	printSeparationLine(tv);
}

void calculateBiggestLength(TableView *tv, ArrayList *row) {
	if(row->length > 0) {
		int i;
		for(i = 0 ; i < row->length ; i++) {
			char *value = (char *) getArrayListObject(row, i);
			int *length = (int *) getArrayListObject(tv->columnsLength, i);
			int proportion = (strlen(value)/(*length));
			if(tv->max < proportion) tv->max = proportion;
		}
	}
}

void printColumn(char *string, int length, int turn) {
	int begin = (turn*length);
	int end = ((turn+1)*length);
	int i;

	printf("%c", BAR);
	if(begin > strlen(string)) completeWithSpace(end - begin);
	else if(end > strlen(string)) {
		for(i = begin ; i < strlen(string) ; i++) printf("%c", string[i]);
		completeWithSpace(end - strlen(string));
	} else {
		for(i = begin ; i < end ; i++) printf("%c", string[i]);
	}
}

void completeWithSpace(int n) {
	int i;
	for(i = 0 ; i < n ; i++) printf("%c", ' ');
}

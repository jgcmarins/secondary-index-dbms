
/*
* 
* Mon Jun 15 16:51:12 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Menu.h"

void runMenu(Database *db) {
	while(1) {
		printMenu();
		char *input = inputReader();
		int op = atoi(input);
		free(input);

		if(!op) break;

		switchOperation(op, db);
	}
}

void cleanScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void printMenu() {
	cleanScreen();
	printf("1. Create new table\n");
	printf("2. Insert into table\n");
	printf("3. Browse all from table\n");
	printf("0. Save and quit\n");


	printf("\n\nType a number: ");
}

void switchOperation(int op, Database *db) {
	cleanScreen();
	switch(op) {
		case 1: createNewTable(db);
				break;
		case 2: insertNewRecordIntoTable(db);
				break;
		case 3: browseAllRecords(db);
				break;
		default:	printf("Invalid operation.\n");
	}

	printf("Hit <ENTER> to get back to menu\n");
	char *input = inputReader();
	free(input);
}

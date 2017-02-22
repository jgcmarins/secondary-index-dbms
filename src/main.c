
/*
* 
* Thu Jun  4 16:00:03 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Database.h"
#include "headers/Menu.h"

int main(int argc, char *argv[]) {

	clearScreen();
	printf("Insert database name: ");
	char *fileName = inputReader();
	Database *db = newDatabase(fileName);

	runMenu(db);

	deleteDatabase(db);
	free(fileName);

	return 0;
}

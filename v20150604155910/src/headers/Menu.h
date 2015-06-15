
/*
* 
* Mon Jun 15 16:49:06 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _MENU_H_
#define _MENU_H_

#include "Database.h"

void runMenu(Database *db);

void cleanScreen();

void printMenu();

void switchOperation(int op, Database *db);

#endif

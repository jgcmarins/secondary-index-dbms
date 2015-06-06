
/*
*
* Thu Mar 19 17:06:05 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
*
*/

#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct ArrayList {
	void **object;
	int length;
} ArrayList;

ArrayList *newArrayList();

void deleteArrayList(ArrayList *list);

void addArrayListObject(ArrayList *list, void *object, int compare(void *, void *));

void setArrayListObject(ArrayList *list, void *object, int position);

void removeArrayListObject(ArrayList *list, void *object, int compare(void *, void *));

void removeArrayListObjectFromPosition(ArrayList *list, int position);

void *getArrayListObject(ArrayList *list, int position);

int indexOfArrayListObject(ArrayList *list, void *object, int compare(void *, void *));

#endif


/*
*
* Thu Mar 19 18:31:15 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
*
*/

#include "headers/ArrayList.h"

ArrayList *newArrayList() {
	ArrayList *list = (ArrayList *) malloc(sizeof(ArrayList));
	list->object = NULL;
	list->length = 0;

	return list;
}

void deleteArrayList(ArrayList *list) {
	if(list != NULL) free(list);
}

void addArrayListObject(ArrayList *list, void *object, int compare(void *, void *)) {
	int position = 0;
	while((position < list->length) && (compare(list->object[position], object) < 0)) position++;
	setArrayListObject(list, object, position);
}

void setArrayListObject(ArrayList *list, void *object, int position) {
	if((position >= 0) && (position <= list->length)) {
		list->object = (void **) realloc(list->object, sizeof(void *)*(list->length+1));
		int i = list->length++;
		while(i > position) {
			list->object[i] = list->object[i-1];
			i--;
		}
		list->object[position] = object;
	}
}

void removeArrayListObject(ArrayList *list, void *object, int compare(void *, void *)) {
	int position = indexOfArrayListObject(list, object, compare);
	if(position != -1) removeArrayListObjectFromPosition(list, position);
}

void removeArrayListObjectFromPosition(ArrayList *list, int position) {
	while((position >= 0) && (position < (list->length-1))) {
		list->object[position] = list->object[position+1];
		position++;
	}
	list->object = (void **) realloc(list->object, sizeof(void *)*(list->length-1));
	list->length--;
}

void *getArrayListObject(ArrayList *list, int position) {
	if((position >= 0) && (position < list->length)) return list->object[position];
	return NULL;
}

int indexOfArrayListObject(ArrayList *list, void *object, int compare(void *, void *)) {
	int position = 0;
	while(position < list->length) {
		if(compare(list->object[position], object) == 0) return position;
		position++;
	}
	return -1;
}

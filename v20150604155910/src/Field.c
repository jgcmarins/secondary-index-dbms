
/*
* 
* Fri Jun  5 13:18:13 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/Field.h"

Field *newField(char *name, char *type, char *key) {
	Field *f = (Field *) malloc(sizeof(Field));
	f->name = name;
	f->type = type;
	f->key = key;

	return f;
}

void deleteField(Field *f) {
	if(f->name != NULL) free(f->name);
	if(f->type != NULL) free(f->type);
	if(f->key != NULL) free(f->key);
	if(f != NULL) free(f);
}

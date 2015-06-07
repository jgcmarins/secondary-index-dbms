
/*
* 
* Fri Jun  5 13:15:57 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#ifndef _FIELD_H_
#define _FIELD_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT "int"
#define LONG "long"
#define FLOAT "float"
#define DOUBLE "double"
#define CHAR "char"
#define STRING "string"

typedef struct Field {
	char *name;
	char *type;
} Field;

Field *newField(char *name, char *type);

void deleteField(Field *f);

#endif

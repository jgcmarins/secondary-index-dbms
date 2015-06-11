
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

#define NORMAL_KEY "normal key"
#define PRIMARY_KEY "primary key"
#define SECONDARY_KEY "secondary key"

typedef struct Field {
	char *name;
	char *type;
	char *key;
} Field;

Field *newField(char *name, char *type, char *key);

void deleteField(Field *f);

#endif


/*
* 
* Sun Jun  7 00:36:42 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/StringConverter.h"

int stringToInt(char *string) {
	return atoi((const char *) string);
}

long stringToLong(char *string) {
	return atol((const char *) string);
}

float stringToFloat(char *string) {
	return (float) atof((const char *) string);
}

double stringToDouble(char *string) {
	return atof((const char *) string);
}


/*
* 
* Sat Jun  6 02:29:32 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/NumberConverter.h"

char *intToString(int number) {
	char *string = NULL;
	string = (char *) realloc(string, sizeof(char)*128);
	int length = sprintf(string, "%d", number);
	string = (char *) realloc(string, sizeof(char)*(length + 1));
	return string;
}

char *longToString(long number) {
	char *string = NULL;
	string = (char *) realloc(string, sizeof(char)*128);
	int length = sprintf(string, "%ld", number);
	string = (char *) realloc(string, sizeof(char)*(length + 1));
	return string;
}

char *floatToString(float number) {
	char *string = NULL;
	string = (char *) realloc(string, sizeof(char)*128);
	int length = sprintf(string, "%f", number);
	string = (char *) realloc(string, sizeof(char)*(length + 1));
	return string;
}

char *doubleToString(double number) {
	char *string = NULL;
	string = (char *) realloc(string, sizeof(char)*128);
	int length = sprintf(string, "%lf", number);
	string = (char *) realloc(string, sizeof(char)*(length + 1));
	return string;
}

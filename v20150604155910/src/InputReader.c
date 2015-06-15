
/*
* 
* Mon Jun 15 02:38:17 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/InputReader.h"

char *inputReader() {
	char *buffer = NULL;
	int c, i = 0;

	do {
		c = fgetc(stdin);
		buffer = (char *) realloc(buffer, sizeof(char)*(i+1));
		buffer[i++] = c;
	} while((c != 10) && !feof(stdin));

	buffer[i-1] = '\0';

	return buffer;
}

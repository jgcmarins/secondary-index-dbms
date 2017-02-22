
/*
* 
* Tue Jun  9 02:38:48 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/PointersComparator.h"

int compareInt(void *p1, void *p2) {
	int *n1 = (int *) p1;
	int *n2 = (int *) p2;

	return (*n1 - *n2);
}

int compareLong(void *p1, void *p2) {
	long *n1 = (long *) p1;
	long *n2 = (long *) p2;

	return (*n1 - *n2);
}

int compareFloat(void *p1, void *p2) {
	float *n1 = (float *) p1;
	float *n2 = (float *) p2;

	return (*n1 - *n2);
}

int compareDouble(void *p1, void *p2) {
	double *n1 = (double *) p1;
	double *n2 = (double *) p2;

	return (*n1 - *n2);
}

int compareChar(void *p1, void *p2) {
	char *c1 = (char *) p1;
	char *c2 = (char *) p2;

	return strcmp(c1, c2);
}

int compareString(void *p1, void *p2) {
	char *s1 = (char *) p1;
	char *s2 = (char *) p2;

	return strcmp(s1, s2);
}


/*
* 
* Thu Jun  4 16:00:03 BRT 2015
* author: Joao Gustavo Cabral de Marins
* e-mail: jgcmarins@gmail.com
* 
*/

#include "headers/FieldHandler.h"
#include "headers/TableView.h"
#include "headers/NumberConverter.h"

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

int main(int argc, char *argv[]) {

	char *fileName = inputReader();
	BinaryFile *bf = newBinaryFile(fileName);
	FieldHandler *fh = newFieldHandler(bf, '|');
	int i;

	/*while(1){
		char *name = inputReader();
		if(name[0] == '\0') {
			free(name);
			break;
		}

		char *type = inputReader();

		Field *f = newField(name, type);
		addNewField(fh, f);
	}*/

	ArrayList *names = newArrayList();
	ArrayList *types = newArrayList();
	i = 0;
	while(i < fh->fields->length) {
		setArrayListObject(names, (char *) getFieldName(fh, i), i);
		setArrayListObject(types, (char *) getFieldType(fh, i), i);
		i++;
	}

	TableView *tv = newTableView(names);
	printTableHeader(tv);
	printTableRow(tv, types);

	while(names->length > 0) removeArrayListObjectFromPosition(names, names->length - 1);
	deleteArrayList(names);

	while(types->length > 0) removeArrayListObjectFromPosition(types, types->length - 1);
	deleteArrayList(types);

	deleteTableView(tv);

	deleteFieldHandler(fh);
	deleteBinaryFile(bf);
	free(fileName);

	return 0;
}

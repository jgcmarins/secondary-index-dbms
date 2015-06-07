
##
# 
# Thu Jun  4 16:01:02 BRT 2015
# author: Joao Gustavo Cabral de Marins
# e-mail: jgcmarins@gmail.com
# 
##

#compiler
C=gcc

#flags
F=-Wall

#binary
BIN=build/tweet.db

#headers
H=./src/headers

#main
M=src/main.c

#library
LIB=src/ArrayList.c src/NumberConverter.c src/StringConverter.c src/TableView.c src/BinaryFile.c src/BinaryFileWriter.c src/BinaryFileReader.c src/Field.c src/FieldHandler.c src/Table.c src/InsertionHandler.c

all: clean compile
clean:
	rm -rf build
	mkdir build
compile:
	$(C) $(F) -o $(BIN) -I$(H) $(M) $(LIB)	
run:
	$(BIN)
valgrind:
	valgrind $(BIN)
cleanfiles:
	rm -rf files
	mkdir files
runin1:
	$(BIN) < in/1
valgrindin1:
	valgrind $(BIN) < in/1
runin2:
	$(BIN) < in/2
valgrindin2:
	valgrind $(BIN) < in/2

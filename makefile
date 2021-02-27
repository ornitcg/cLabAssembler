all: prog

prog: assembler.o utilsGeneral.o as_funcs.o
	gcc -ansi -g -Wall -pedantic -o prog assembler.o utilsGeneral.o as_funcs.o

#file1.o: file1.c headers.h
#	gcc -ansi -g -Wall -pedantic -c file1.c


as_funcs.o: as_funcs.c as_funcs.h
	gcc -ansi -g -Wall -pedantic -c as_funcs.c


assembler.o: assembler.c utilsGeneral.h
	gcc -ansi -g -Wall -pedantic  -c assembler.c


utilsGeneral.o: utilsGeneral.c
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #prog

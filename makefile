all: prog

prog: assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o
	gcc -ansi -g -Wall -pedantic -o prog assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o

utilsAssembler.o: utilsAssembler.c utilsAssembler.h structsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c utilsAssembler.c

as_funcs.o: as_funcs.c as_funcs.h structsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c as_funcs.c

assembler.o: assembler.c utilsGeneral.h structsAndMacros.h
	gcc -ansi -g -Wall -pedantic  -c assembler.c

structsAndMacros.o: structsAndMacros.c
	gcc -ansi -g -Wall -pedantic -c structsAndMacros.c


utilsGeneral.o: utilsGeneral.c
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #prog

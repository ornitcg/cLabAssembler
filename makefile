all: prog

prog: assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o linkedList.o asStructsAndMacros.o
	gcc -ansi -g -Wall -pedantic -o prog assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o linkedList.o asStructsAndMacros.o

assembler.o: assembler.c  asStructsAndMacros.h
		gcc -ansi -g -Wall -pedantic  -c assembler.c

as_funcs.o: as_funcs.c as_funcs.h utilsAssembler.h asStructsAndMacros.h linkedList.h utilsGeneral.h
		gcc -ansi -g -Wall -pedantic -c as_funcs.c

utilsAssembler.o: utilsAssembler.c utilsGeneral.h utilsAssembler.h asStructsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c utilsAssembler.c

asStructsAndMacros.o: asStructsAndMacros.c linkedList.h utilsGeneral.h utilsAssembler.h
	gcc -ansi -g -Wall -pedantic -c asStructsAndMacros.c

linkedList.o: linkedList.c linkedList.h
	gcc -ansi -g -Wall -pedantic -c linkedList.c

utilsGeneral.o: utilsGeneral.c utilsGeneral.h
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #prog

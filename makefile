all: prog

prog: assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o #linkedList.o
	gcc -ansi -g -Wall -pedantic -o prog assembler.o utilsGeneral.o as_funcs.o utilsAssembler.o #linkedList.o

utilsAssembler.o: utilsAssembler.c utilsAssembler.h asStructsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c utilsAssembler.c

assembler.o: assembler.c utilsGeneral.h asStructsAndMacros.h
	gcc -ansi -g -Wall -pedantic  -c assembler.c

as_funcs.o: as_funcs.c as_funcs.h asStructsAndMacros.h #linkedList.h
	gcc -ansi -g -Wall -pedantic -c as_funcs.c

asStructsAndMacros.o: asStructsAndMacros.c #linkedList.h
	gcc -ansi -g -Wall -pedantic -c asStructsAndMacros.c

#linkedList.o: linkedList.c linkedList.h
#	gcc -ansi -g -Wall -pedantic -c linkedList.c

utilsGeneral.o: utilsGeneral.c
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #prog

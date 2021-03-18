all: assembler

assembler: assembler.o asScanners.o symbolFuncs.o instructionFuncs.o commandFuncs.o utilsAssembler.o  asStructsAndMacros.o utilsGeneral.o  linkedList.o buildOutput.o
	gcc -ansi -g -Wall -pedantic -o assembler assembler.o asScanners.o symbolFuncs.o instructionFuncs.o commandFuncs.o utilsAssembler.o  asStructsAndMacros.o utilsGeneral.o  linkedList.o buildOutput.o

assembler.o: assembler.c  asStructsAndMacros.h asScanners.h utilsAssembler.h utilsGeneral.h buildOutput.h
	gcc -ansi -g -Wall -pedantic  -c assembler.c

asScanners.o: asScanners.c asScanners.h utilsAssembler.h asStructsAndMacros.h linkedList.h utilsGeneral.h symbolFuncs.h instructionFuncs.h commandFuncs.h buildOutput.h
	gcc -ansi -g -Wall -pedantic -c asScanners.c

buildOutput.o: buildOutput.c buildOutput.h asStructsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c buildOutput.c

utilsAssembler.o: utilsAssembler.c utilsGeneral.h utilsAssembler.h asStructsAndMacros.h
	gcc -ansi -g -Wall -pedantic -c utilsAssembler.c

commandFuncs.o: commandFuncs.c utilsGeneral.h utilsAssembler.h asStructsAndMacros.h symbolFuncs.h
	gcc -ansi -g -Wall -pedantic -c commandFuncs.c

instructionFuncs.o: instructionFuncs.c utilsGeneral.h utilsAssembler.h asStructsAndMacros.h symbolFuncs.h
	gcc -ansi -g -Wall -pedantic -c instructionFuncs.c

symbolFuncs.o: symbolFuncs.c utilsGeneral.h utilsAssembler.h asStructsAndMacros.h symbolFuncs.h
	gcc -ansi -g -Wall -pedantic -c symbolFuncs.c

asStructsAndMacros.o: asStructsAndMacros.c asStructsAndMacros.h linkedList.h utilsGeneral.h utilsAssembler.h
	gcc -ansi -g -Wall -pedantic -c asStructsAndMacros.c

linkedList.o: linkedList.c linkedList.h
	gcc -ansi -g -Wall -pedantic -c linkedList.c

utilsGeneral.o: utilsGeneral.c utilsGeneral.h
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #assembler

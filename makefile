all: prog

prog: main.o utilsGeneral.o
	gcc -ansi -g -Wall -pedantic -o prog main.o utilsGeneral.o

#file1.o: file1.c headers.h
#	gcc -ansi -g -Wall -pedantic -c file1.c


# file2.o: file2.c headers.h
	# gcc -ansi -g -Wall -pedantic -c file2.c


main.o: main.c utilsGeneral.h
	gcc -ansi -g -Wall -pedantic -c main.c


utilsGeneral.o: utilsGeneral.c utilsGeneral.h
	gcc -ansi -g -Wall -pedantic -c utilsGeneral.c

clean:
	rm *.o  #prog

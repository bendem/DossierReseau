# cphex\makefile ffffffff

LIBS=
all:	main	ListeSimple.o

ListeSimple.o:	ListeSimple.h	ListeSimple.c
	echo "Compilation de ListeSimple"
	cc -c ListeSimple.c	

main:	main.c	ListeSimple.o
	echo "Compilation de client"
	cc  -o main	main.c ListeSimple.o	$(LIBS)


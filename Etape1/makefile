# cphex\makefile

LIBS=
all:	admin	parking.o

parking.o:	../Parking/parking.c	../Parking/parking.h
	echo "Compilation de parking.o"
	cc -c ../Parking/parking.c

admin	:	admin.c	parking.o
	echo "Compilation de admin"
	cc -o admin  admin.c parking.o 	$(LIBS)


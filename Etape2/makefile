LIBS=../tcplib/tcplib.o ../Parking/parking.o
all:    admin cli ser

../Parking/parking.o:   ../Parking/parking.c    ../Parking/parking.h
	echo "Compilation de $@"
	cc -c ../Parking/parking.c -o $@

../tcplib/tcplib.o: ../tcplib/tcplib.c ../tcplib/tcplib.h
	echo "Compilation de $@"
	cc -c $< -o $@

admin   :   admin.c $(LIBS)
	echo "Compilation de admin"
	cc -o admin  $^

physlib.o:	../physlib/physlib.h	../physlib/physlib.c
	echo "Compilation de physlib.o"
	cc -c ../physlib/physlib.c

tcplib.o:	../tcplib/tcplib.h	../tcplib/tcplib.c
	echo "Compilation de tcplib.o"
	cc -c ../tcplib/tcplib.c

cli:	cli.c	physlib.o	tcplib.o ../Parking/parking.o
	echo "Compilation de client"
	cc  -o cli  $^

ser:	ser.c	physlib.o	tcplib.o ../Parking/parking.o
	echo "Compilation de serveur"
	cc -o ser  $^

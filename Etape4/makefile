LIBS = ../tcplib/tcplib.o ../Parking/parking.o

all:    admin cli ser

../Parking/parking.o: ../Parking/parking.c ../Parking/parking.h
	echo "Compilation de $@"
	cc -c ../Parking/parking.c -o $@

../tcplib/tcplib.o: ../tcplib/tcplib.c ../tcplib/tcplib.h
	echo "Compilation de $@"
	cc -c $< -o $@

../siglib/sigs.o: ../siglib/sigs.c ../siglib/sigs.h
	echo "Compilation de $@"
	cc -c $< -o $@

admin: admin.c $(LIBS)
	echo "Compilation de admin"
	cc -o $@  $^

physlib.o: ../physlib/physlib.c ../physlib/physlib.h
	echo "Compilation de physlib.o"
	cc -c $<

tcplib.o: ../tcplib/tcplib.c ../tcplib/tcplib.h
	echo "Compilation de tcplib.o"
	cc -c $<

cli: cli.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de client"
	cc  -o cli  $^

ser: ser.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de serveur"
	cc -o ser  $^

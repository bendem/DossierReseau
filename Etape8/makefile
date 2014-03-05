LIBS = ../tcplib/tcplib.o ../Parking/parking.o

all: admin BorneE ser BorneP BorneS

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
	cc -o $@ $^

physlib.o: ../physlib/physlib.c ../physlib/physlib.h
	echo "Compilation de physlib.o"
	cc -c $<

tcplib.o: ../tcplib/tcplib.c ../tcplib/tcplib.h
	echo "Compilation de tcplib.o"
	cc -c $<

BorneE: cli.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de BorneE"
	cc  -o BorneE $^

BorneP: pay.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de BorneP"
	cc  -o BorneP $^

BorneS: sortie.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de BorneS"
	cc  -o BorneS $^

ser: ser.c physlib.o tcplib.o ../Parking/parking.o ../siglib/sigs.o
	echo "Compilation de serveur"
	cc -o ser $^

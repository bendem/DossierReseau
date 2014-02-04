LIBS=../tcplib/tcplib.o ../Parking/parking.o
all:	admin

../Parking/parking.o:	../Parking/parking.c	../Parking/parking.h
	echo "Compilation de parking.o"
	cc -c ../Parking/parking.c -o $@

../tcplib/tcplib.o: ../tcplib/tcplib.c ../tcplib/tcplib.h
	echo "Compilation de $@"
	cc -c $< -o $@

admin	:	admin.c	$(LIBS)
	echo "Compilation de admin"
	cc -o admin  $^


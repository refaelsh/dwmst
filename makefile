all: dwmst 

main.o: main.c 
	cc -c main.c

dwmst: main.o 
	cc -lX11 -o dwmst main.o 

clean:
	rm dwmst main.o 

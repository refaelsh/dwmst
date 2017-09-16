all: dwmst 

main.o: main.c 
	cc -DDEBUG -O0 -g -Wall -c main.c

dwmst: main.o 
	cc -DDEBUG -lX11 -o dwmst main.o 

clean:
	rm dwmst main.o 

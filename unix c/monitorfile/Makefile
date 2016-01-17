

all:list.o monitorfile.o main.o
	gcc main.o list.o monitorfile.o -o app

list.o:list.c
	gcc -c list.c

monitorfile.o:monitorfile.c
	gcc -c monitorfile.c

main.o:main.c
	gcc -c main.c

clean:
	-rm -f *.o
	-rm -f app

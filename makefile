all: jonesro4.adventure jonesro4.buildrooms

jonesro4.adventure: jonesro4.adventure.o
	gcc jonesro4.adventure.o -o jonesro4.adventure -Wall -lpthread

jonesro4.adventure.o: jonesro4.adventure.c
	gcc -c jonesro4.adventure.c -g

jonesro4.buildrooms: jonesro4.buildrooms.o
	gcc -o jonesro4.buildrooms jonesro4.buildrooms.o

jonesro4.buildrooms.o: jonesro4.buildrooms.c
	gcc -c jonesro4.buildrooms.c

clean:	
	rm -r currentTime.txt
	rm jonesro4.adventure.o jonesro4.adventure
	rm jonesro4.buildrooms.o jonesro4.buildrooms
	rm -r jonesro4.rooms.*

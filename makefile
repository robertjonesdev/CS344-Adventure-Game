IFLAG=-I

# jonesro4.adventure: jonesro4.adventure.c
#	gcc –o jonesro4.adventure jonesro4.adventure.c -lpthread

jonesro4.buildrooms: jonesro4.buildrooms.o
	gcc -o jonesro4.buildrooms jonesro4.buildrooms.o

jonesro4.buildrooms.o: jonesro4.buildrooms.c
	gcc -c jonesro4.buildrooms.c

clean:
	rm jonesro4.buildrooms.o jonesro4.buildrooms
	rmdir jonesro4.rooms.*

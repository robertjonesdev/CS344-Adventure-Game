/*********************************************************
 * Name: Robert Jones
 * Class: CS344 Spring 2020
 * Assignment: Program 2 - Adventure
 * Due: May 3rd, 2020
 *
 * file: jonesro4.buildrooms.c
 *
 * ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* Constant definitions: maximum number of rooms and maximum number 
 * of connections for each room */
#define NUM_ROOMS 7
#define NUM_CONS 6

/* Struct definition that represent a Room */
struct Room {
	char name[9];
	char connection[NUM_CONS][9];
	char roomtype[11];
};


/* Function Definitions */
int 	IsGraphFull(struct Room Rooms[]);
void 	AddRandomConnection(struct Room Rooms[]);
void 	AddRoomTypes(struct Room Rooms[]);
int 	CanAddConnectionFrom(struct Room Rooms[], int x);
int 	ConnectionAlreadyExists(struct Room Rooms[], int x, int y);
void 	ConnectRoom( struct Room Rooms[], int x, int y );
int 	IsSameRoom( struct Room Rooms[], int x, int y );
void 	CreateDirectory(char* directoryName);
int 	GetRandomRoom();
void 	WriteRoomsToFiles(struct Room Rooms[]);
void 	InitRooms(struct Room Rooms[]);


int main()
{
	/* Seed random */
	srand(time(NULL));

	/* Declare and initialize a blank array of room structs
	 */
	struct Room Rooms[NUM_ROOMS];
	InitRooms(Rooms);

	
	/* Providing a hard-coded list of 10 room names
	 * that can be randomly selected from */
	const char *roomnames[10];
	roomnames[0] = "NewHaven";
	roomnames[1] = "Slums";
	roomnames[2] = "Pyramid";
	roomnames[3] = "Mines";
	roomnames[4] = "Dungeon";
	roomnames[5] = "Volcano";
	roomnames[6] = "Labrynth";
	roomnames[7] = "Forest";
	roomnames[8] = "Crypt";
	roomnames[9] = "Town";

	/* Randomly set the Room Names 
	 * for each of the rooms in the array (0 to NUM_ROOMS-1) */
	int i, usedRoomNames[10];
	for (i = 0; i < NUM_ROOMS; i++)
	{
		int rndRoomName = 0;
		/* random number from 0 to 9 */
		rndRoomName = rand() % 10;
	
		/* use an array of integers to keep track
		 * of which room names have been used
		 * only use a room name if not in the list.
		 * If a collision occurs, increase linearly 
		 * until no collision.
		 */
		while (usedRoomNames[rndRoomName] == 1) 
		{
			rndRoomName = (rndRoomName + 1) % 10;
		}
		
		/* Now its ensured the name hasn't been used, 
		 * store it as the Room name */
		usedRoomNames[rndRoomName] = 1;
		strcpy(Rooms[i].name,roomnames[rndRoomName]);	
		int j;
		
		for (j = 0; j < NUM_CONS; j++)
		{
			strcpy(Rooms[i].connection[j], "");
		}
		strcpy(Rooms[i].roomtype, "");
	}
	

	/* Create all connections in graphs */
	/* Continue looping through a random selection until the 
	 * Graph is determined to be full. To be full, all the rooms
	 * must have at least 3 connections.
	 */
	while (IsGraphFull(Rooms) == 0)
	{
		AddRandomConnection(Rooms);
	}
        
	/* call Add Room Types */
	AddRoomTypes(Rooms);

	/* And finally write each Room struct to a separate file */
	WriteRoomsToFiles(Rooms);	

	return 0;
}

/* Init Rooms
 * Receives: Array of room structs
 * This function clears the memeory of all elements in the array struct.
 */
void InitRooms(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		memset(Rooms[i].name,'\0', sizeof(Rooms[i].name) );
		memset(Rooms[i].roomtype, '\0', sizeof(Rooms[i].roomtype) );
		int j;
		for (j = 0; j < NUM_CONS; j++)
		{
			memset( Rooms[i].connection[j], '\0', sizeof(Rooms[i].connection[j] ) );
		}
	}
}


/* Write Rooms to Files
 * Receives: Array of room structs
 * This function takes a completed and full array of room structs 
 * and writes their contents to a file in a directory of "jonesro4.rooms.[PID]"
 */
void WriteRoomsToFiles(struct Room Rooms[]) {
	/* Create Directory for room files /jonesro4.rooms.PID/ */
	char dirName[80];
	int pid = getpid();
	char pidStr[10];
	sprintf(pidStr, "%d", pid);

	strcpy(dirName,"jonesro4.rooms.");
	strcat(dirName, pidStr);

	mkdir(dirName, 0777);


	/* Hard-coded array of file names */
	const char *filenames[7];
	filenames[0] = "first_room";
	filenames[1] = "second_room";
	filenames[2] = "third_room";
	filenames[3] = "fourth_room";
	filenames[4] = "fifth_room";
	filenames[5] = "sixth_room";
	filenames[6] = "seventh_room";

	/* For each room, print the struct contents to a file */
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
	
		char filePath[250];
		sprintf(filePath, "%s/%s", dirName, filenames[i]);
		
		FILE * fp;
		fp = fopen(filePath, "w");
		
		/* write the room name to file */
		fprintf(fp, "ROOM NAME: %s\n", Rooms[i].name);
		int j;
		/* write the connections to file */
		for (j = 0; j < NUM_CONS; j++)
		{
			/* test to make sure the connection is not empty
			 * if not empty, write to file */
			if (strcmp(Rooms[i].connection[j],"") != 0) 
			{
				fprintf(fp, "CONNECTION %d: %s\n", j+1, Rooms[i].connection[j]);
			}
		}
		/* Write the room type to file */
		fprintf(fp, "ROOM TYPE: %s\n", Rooms[i].roomtype);
		fclose(fp);

	}

}



/* Is Graph Full
 * Receives: Array of room structs
 * Returns true if all rooms have 3 to 6 outbound connections, false otherwise */
int IsGraphFull(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++) {
		/* compare the first 3 connections as a minimum
		 * anymore are okay but not necessary */
		if (strcmp(Rooms[i].connection[0], "") == 0 || 
			strcmp(Rooms[i].connection[1], "") == 0 || 
			strcmp(Rooms[i].connection[2], "") == 0) {
			return 0;
		}
	}
	return 1;	
}

/* Add Random Connection
 * Receives: Array of Room Structures
 * Adds a random, valid outbound connection from a Room to another Room */
void AddRandomConnection(struct Room Rooms[])
{
	int A;
	int B;

	/* Get a random room that has less than MAX_CONS connections */
	while(1)
	{
		A = GetRandomRoom(); 

		if (CanAddConnectionFrom(Rooms, A) == 1)
			break;
	}

	/* Get another random room that has less than MAX_CONS connections
	 * but that is also different than Room A */
	do
	{
		B = GetRandomRoom(); 
	}
	while ( CanAddConnectionFrom(Rooms, B) == 0 || 
		IsSameRoom(Rooms, A,B) == 1 || 
		ConnectionAlreadyExists(Rooms, A,B) == 1);

	/* Connect the two rooms together */
	ConnectRoom(Rooms, A, B);

}


/* Add Room Types
 * Receives: Array of room structures
 * This function selects a random room and assigns a room type of START_ROOM
 * Then it takes another random room that is not Room A and assigns it as
 * END_ROOM. Then all other rooms are assigned MID_ROOM 
 */

void AddRoomTypes(struct Room Rooms[])
{
	/* Get a random room A, assign as START_ROOM */
	int A = GetRandomRoom();
	int B;
	strcpy(Rooms[A].roomtype,"START_ROOM");
	
	/* Get a random room B, assign as END_ROOM */
	do
	{
		B = GetRandomRoom();
	}
	while (IsSameRoom(Rooms, A, B) == 1);
	strcpy(Rooms[B].roomtype,"END_ROOM");
	
	
	/* For all other rooms that are not A OR B, assign as MID_ROOM */
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		if (i == A || i == B) {
			continue;
		}
		else {
			strcpy(Rooms[i].roomtype,"MID_ROOM");
		}
	}

}


/* Get Random Room
 * Returns a room number from 0 to (NUM_ROOMS-1)
 */
int  GetRandomRoom()
{	
	int rndRoom = 0;
	rndRoom = rand() % NUM_ROOMS;
	return rndRoom;
	
}


/* Can Add Connection From
 * Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise */
int CanAddConnectionFrom(struct Room Rooms[], int x)
{
	/* test if the 6th/last connection is open*/
	if (strcmp(Rooms[x].connection[5], "") == 0) {
		return 1;
	}
	return 0;
}

/* Connection Already Exists
 * Receives: Array of room structures, room number integer, room number integer
 * Returns true if a connection from Room x to Room y already exists, false otherwise */
int ConnectionAlreadyExists(struct Room Rooms[], int x, int y)
{
	int i;
	/* scan the first room's conection and look for the 2nd room name
	 * if found, return 1. At the end of the for loop return 0 */
	for (i = 0; i < NUM_CONS; i++) 
	{
		if (strcmp(Rooms[x].connection[i], Rooms[y].name) == 0)
		{
			return 1;
		}
	}
	return 0;
}


/* Connect Room
 * Receives: array of room structs, room number integer, and room number integer
 * Requirements: this function requries that CanAddConnectionFrom is already 
 * been used to test if a connection can be added.
 * Connects Room x and y together, does not check if this connection is valid */
void ConnectRoom( struct Room Rooms[], int x, int y )
{
	int i = 0;
	int j = 0;
	/* Loop through the room connections to find the next available spot
	 * and save the spot number */
	while (strcmp(Rooms[x].connection[i], "") != 0)
	{
		i++;
	}
	/* loop again for the nexxt room */
	while (strcmp(Rooms[y].connection[j], "") != 0)
	{
		j++;
	}
	/* add the connection to both rooms */
	strcpy(Rooms[x].connection[i], Rooms[y].name);
	strcpy(Rooms[y].connection[j], Rooms[x].name);
}


/* Is Same Room
 * Receives: Array of room structs, room number integer, and room number integer
 * Returns true if Rooms x and y are the same ROom, false otherwise */
int IsSameRoom( struct Room Rooms[], int x, int y )
{
	/* Compare the room names since they are unique */
	if (strcmp(Rooms[x].name, Rooms[y].name) == 0 ) {
		return 1;
	}
	return 0;
}

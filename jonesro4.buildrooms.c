#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define NUM_ROOMS 7
#define NUM_CONS 6


struct Room {
	char name[9];
	char connection[NUM_CONS][9];
	char roomtype[11];
};


/* Function Definitions */
int IsGraphFull(struct Room Rooms[]);
void AddRandomConnection(struct Room Rooms[]);
void AddRoomTypes(struct Room Rooms[]);
int CanAddConnectionFrom(struct Room Rooms[], int x);
int ConnectionAlreadyExists(struct Room Rooms[], int x, int y);
void ConnectRoom( struct Room Rooms[], int x, int y );
int IsSameRoom( struct Room Rooms[], int x, int y );
void CreateDirectory(char* directoryName);
int GetRandomRoom();
void WriteRoomsToFiles(struct Room Rooms[]);
void InitRooms(struct Room Rooms[]);


int main()
{
	srand(time(NULL));

	struct Room Rooms[NUM_ROOMS];
	InitRooms(Rooms);

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

	/* Randomly set the Room Names */
	int i, usedRoomNames[10];
	for (i = 0; i < NUM_ROOMS; i++)
	{
		int rndRoomName = 0;
		rndRoomName = rand() % 10;
		while (usedRoomNames[rndRoomName] == 1) 
		{
			rndRoomName = (rndRoomName + 1) % 10;
		}
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
	while (IsGraphFull(Rooms) == 0)
	{
		AddRandomConnection(Rooms);
	}
        
	AddRoomTypes(Rooms);

	WriteRoomsToFiles(Rooms);	

	return 0;
}


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
		fprintf(fp, "ROOM NAME: %s\n", Rooms[i].name);
		int j;
		for (j = 0; j < NUM_CONS; j++)
		{
			if (strcmp(Rooms[i].connection[j],"") != 0) 
			{
				fprintf(fp, "CONNECTION %d: %s\n", j+1, Rooms[i].connection[j]);
			}
		}
		fprintf(fp, "ROOM TYPE: %s\n", Rooms[i].roomtype);
		fclose(fp);

	}

}



/* Returns true if all rooms have 3 to 6 outbound connections, false otherwise */
int IsGraphFull(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++) {
		if (strcmp(Rooms[i].connection[0], "") == 0 || 
			strcmp(Rooms[i].connection[1], "") == 0 || 
			strcmp(Rooms[i].connection[2], "") == 0) {
			return 0;
		}
	}
	return 1;	
}

/* Adds a random, valid outbound connection from a Room to another Room */
void AddRandomConnection(struct Room Rooms[])
{
	int A;
	int B;

	while(1)
	{
		A = GetRandomRoom(); 

		if (CanAddConnectionFrom(Rooms, A) == 1)
			break;
	}

	do
	{
		B = GetRandomRoom(); 
	}
	while ( CanAddConnectionFrom(Rooms, B) == 0 || 
		IsSameRoom(Rooms, A,B) == 1 || 
		ConnectionAlreadyExists(Rooms, A,B) == 1);

	ConnectRoom(Rooms, A, B);

}

void AddRoomTypes(struct Room Rooms[])
{
	int A = GetRandomRoom();
	int B;
	strcpy(Rooms[A].roomtype,"START_ROOM");
	do
	{
		B = GetRandomRoom();
	}
	while (IsSameRoom(Rooms, A, B) == 1);

	strcpy(Rooms[B].roomtype,"END_ROOM");
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


int  GetRandomRoom()
{	
	int rndRoom = 0;
	rndRoom = rand() % NUM_ROOMS;
	return rndRoom;
	
}

/* Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise */
int CanAddConnectionFrom(struct Room Rooms[], int x)
{
	if (strcmp(Rooms[x].connection[5], "") == 0) {
		return 1;
	}
	return 0;
}

/* Returns true if a connection from Room x to Room y already exists, false otherwise */
int ConnectionAlreadyExists(struct Room Rooms[], int x, int y)
{
	int i;
	for (i = 0; i < NUM_CONS; i++) 
	{
		if (strcmp(Rooms[x].connection[i], Rooms[y].name) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/* Connects Room x and y together, does not check if this connection is valid */
void ConnectRoom( struct Room Rooms[], int x, int y )
{
	int i = 0;
	int j = 0;
	while (strcmp(Rooms[x].connection[i], "") != 0)
	{
		i++;
	}
	while (strcmp(Rooms[y].connection[j], "") != 0)
	{
		j++;
	}

	strcpy(Rooms[x].connection[i], Rooms[y].name);
	strcpy(Rooms[y].connection[j], Rooms[x].name);
}

/* Returns true if Rooms x and y are the same ROom, false otherwise */
int IsSameRoom( struct Room Rooms[], int x, int y )
{
	if (strcmp(Rooms[x].name, Rooms[y].name) == 0 ) {
		return 1;
	}
	return 0;
}

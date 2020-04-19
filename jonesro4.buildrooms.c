#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define NUM_ROOMS 7

struct Room {
	char name[9];
	char con1[9];
	char con2[9];
	char con3[9];
	char con4[9];
	char con5[9];
	char con6[9];
	char roomtype[11];
};

int IsGraphFull(struct Room[]);
void AddRandomConnection();
int CanAddConnectionFrom(struct Room x);
int ConnectionAlreadyExists(struct Room x, struct Room y);
void ConnectRoom( struct Room x, struct Room y );
int IsSameRoom( struct Room x, struct Room y );
void CreateDirectory(char* directoryName);
void PrintRooms(struct Room[]);


int main()
{
	srand(time(NULL));

	struct Room Rooms[NUM_ROOMS];

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
		strcpy(Rooms[i].con1, "");
		strcpy(Rooms[i].con2, "");
		strcpy(Rooms[i].con3, "");
		strcpy(Rooms[i].con4, "");
		strcpy(Rooms[i].con5, "");
		strcpy(Rooms[i].con6, "");	
		strcpy(Rooms[i].roomtype, "");
	}
	
	PrintRooms(Rooms);

	/* Create all connections in graphs */
	/*
	while (IsGraphFull(Rooms) == 0)
	{
		AddRandomConnection(Rooms);
	}
	*/
		
	int result = IsGraphFull(Rooms);
	printf("IsGraphFull: %d\n",result);
	
}


void PrintRooms(struct Room Rooms[]) {

	int i;
	for (i = 0; i < NUM_ROOMS; i++) {
		printf("Room #%d\n",i);
		printf("ROOM NAME: %s\n",Rooms[i].name);
		printf("CONNECTION 1: %s\n", Rooms[i].con1);
		printf("CONNECTION 2: %s\n", Rooms[i].con2);
		printf("CONNECTION 3: %s\n", Rooms[i].con3);
		printf("CONNECTION 4: %s\n", Rooms[i].con4);
		printf("CONNECTION 5: %s\n", Rooms[i].con5);
		printf("CONNECTION 6: %s\n", Rooms[i].con6);
		printf("\n");
	
	}
	

}

void WriteRoomsToFiles() {
	/* Create directory and store directory name in dirName */ 
	char dirName[80];
	CreateDirectory(dirName);
	printf("Directory Name: %s\n",dirName);

	const char *filenames[7];
	filenames[0] = "first_room";
	filenames[1] = "second_room";
	filenames[2] = "third_room";
	filenames[3] = "fourth_room";
	filenames[4] = "fifth_room";
	filenames[5] = "sixth_room";
	filenames[6] = "seventh_room";

	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
	
		char filePath[250];
		sprintf(filePath, "%s/%s", dirName, filenames[i]);
		printf("Creating file: %s",filePath);
		
		FILE * fp;
		fp = fopen(filePath, "w");
		fprintf(fp, "ROOM NAME: \n");
		fclose(fp);

	}

}



void CreateDirectory(char *directoryName) {
	
	/* Create Directory for room files /jonesro4.rooms.PID/ */
	int pid = getpid();
	char pidStr[10];
	sprintf(pidStr, "%d", pid);

	strcpy(directoryName,"jonesro4.rooms.");
	strcat(directoryName, pidStr);

	mkdir(directoryName, 0777);

}


/* Returns true if all rooms have 3 to 6 outbound connections, false otherwise */
int IsGraphFull(struct Room Rooms[])
{
	
	int i;
	for (i = 0; i < NUM_ROOMS; i++) {
		if (strcmp(Rooms[i].con1, "") == 0 || 
			strcmp(Rooms[i].con2, "") == 0 || 
			strcmp(Rooms[i].con2, "") == 0) {
			return 0;
		}
	}
	

	/* For each file, is there between 3 and 6 outbound connections? */
	/*
	DIR* dirToCheck;
	struct dirent *fileInDir;
	struct stat dirAttributes;

	dirToCheck = opendir(directoryName);

	fileInDir = NULL;
	if (dirToCheck > 0)
	{
		while((fileInDir = readdir(dirToCheck)) != NULL)
		{
			if(strcmp(fileInDir->d_name, ".") != 0 && strcmp(fileInDir->d_name,"..") != 0 )
			{
				char filePath[250];
				sprintf(filePath, "%s/%s",directoryName, fileInDir->d_name);
				
				FILE * fp;
				fp = fopen(filePath, "r");
				
				char line[80];
				int numConnections = -1;
				while ( fgets(line, 80, fp) != NULL) {
					numConnections++;
				}
				fclose(fp);
				if (numConnections < 3) {
					return 0;
				}
			}
		}
	}
	return 1;
	*/
}

/* Adds a random, valid outbound connection from a Room to another Room */
void AddRandomConnection()
{
	struct Room A;
	struct Room B;

	while(1)
	{
	/*	A = GetRandomRoom(); */

		if (CanAddConnectionFrom(A) == 1)
			break;
	}

	do
	{
	/*	B = GetRandomRoom(); */
	}
	while ( CanAddConnectionFrom(B) == 0 || IsSameRoom(A,B) == 1 || ConnectionAlreadyExists(A,B) == 1);

	ConnectRoom(A,B);
	ConnectRoom(B,A);

}


/* Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise */
int CanAddConnectionFrom(struct Room x)
{

}

/* Returns true if a connection from Room x to Room y already exists, false otherwise */
int ConnectionAlreadyExists(struct Room x, struct Room y)
{

}

/* Connects Room x and y together, does not check if this connection is valid */
void ConnectRoom( struct Room x, struct Room y )
{

}

/* Returns true if Rooms x and y are the same ROom, false otherwise */
int IsSameRoom( struct Room x, struct Room y )
{

}


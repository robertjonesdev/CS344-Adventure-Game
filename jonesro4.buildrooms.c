#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

struct Room {
};

int IsGraphFull();
void AddRandomConnection();
int CanAddConnectionFrom(struct Room x);
int ConnectionAlreadyExists(struct Room x, struct Room y);
void ConnectRoom( struct Room x, struct Room y );
int IsSameRoom( struct Room x, struct Room y );
void CreateDirectory(char* directoryName);

int main()
{
	srand(time(NULL));

	/* Create directory and store directory name in dirName */ 
	char dirName[80];
	CreateDirectory(dirName);
	printf("Directory Name: %s\n",dirName);


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

	const char *filenames[7];
	filenames[0] = "first_room";
	filenames[1] = "second_room";
	filenames[2] = "third_room";
	filenames[3] = "fourth_room";
	filenames[4] = "fifth_room";
	filenames[5] = "sixth_room";
	filenames[6] = "seventh_room";

	int usedRoomNames[10];

	int i;
	for (i = 0; i < 7; i++)
	{
		int rndRoomName = 0;
		rndRoomName = rand() % 10;
		while (usedRoomNames[rndRoomName] == 1) 
		{
			rndRoomName = (rndRoomName + 1) % 10;
		}
		usedRoomNames[rndRoomName] = 1;
		
		char filePath[250];
		sprintf(filePath, "%s/%s", dirName, filenames[i]);
		printf("Creating file: %s with room name: %s\n", filePath, roomnames[rndRoomName]);
		
		FILE * fp;
		fp = fopen(filePath, "w");
		fprintf(fp, "ROOM NAME: %s\n",roomnames[rndRoomName]);
		fclose(fp);
	}

	/* Create all connections in graphs */
	/*
	while (IsGraphFull(dirName) == 0)
	{
		AddRandomConnection();
	}
	*/
	
	int result = IsGraphFull(dirName);
	printf("IsGraphFull: %d\n",result);
	
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
int IsGraphFull(char *directoryName)
{
	/* For each file, is there between 3 and 6 outbound connections? */
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


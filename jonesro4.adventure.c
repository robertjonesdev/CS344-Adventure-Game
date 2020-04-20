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

void InitRooms(struct Room Rooms[]);
void ReadFiles(struct Room Rooms[]);
void PrintRooms(struct Room Rooms[]);
int GetStartRoom(struct Room Rooms[]);
int IsEndRoom(struct Room Rooms[], int X);
void PrintCurrentRoom(struct Room Rooms[], int currentRoom);
void StorePath(char **Path, int *strlength, char roomname[9]);

int main()
{
	struct Room Rooms[NUM_ROOMS];
	
	int PathStrLength = 120;
	char *PathToVictory;
	PathToVictory = (char *) malloc(PathStrLength);

	memset(PathToVictory, '\0', sizeof(PathToVictory));

	InitRooms(Rooms);
	ReadFiles(Rooms);

	int currentRoom = GetStartRoom(Rooms);
	char userInput[80];
	int stepCounter = 0;
	while(IsEndRoom(Rooms,currentRoom) != 1)
	{
		PrintCurrentRoom(Rooms, currentRoom);
		printf("WHERE TO? >");
		scanf("%s", userInput);

		if (strcmp(userInput,"time") == 0) 
		{

		}
		else 
		{
			
			int newRoom  = MoveRooms(Rooms, currentRoom, userInput);
			if (newRoom != currentRoom) 
			{
				stepCounter++;
				currentRoom = newRoom;
				StorePath(&PathToVictory, &PathStrLength, Rooms[newRoom].name);				
			}
		}
		printf("\n");
	}
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepCounter);
	printf("%s", PathToVictory);
	free(PathToVictory);

	return 0;
}


int GetStartRoom(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		if (strcmp(Rooms[i].roomtype,"START_ROOM") == 0) 
		{
			return i;
		}
	}
	return -1;
}


void StorePath(char **Path, int *strlength, char roomname[9])
{

	int len = strlen(*Path);
	if (len > (*strlength - 20))
	{
		*Path = (char *) realloc(*Path,sizeof(char)*(*strlength *2));
	
		*strlength = *strlength * 2;
	}

	strcat(*Path, roomname);
	strcat(*Path, "\n");
}

int MoveRooms(struct Room Rooms[], int currentRoom, char nextRoomName[80])
{
	int i;
	for (i = 0; i < NUM_CONS; i++)
	{
		if (strcmp(Rooms[currentRoom].connection[i],nextRoomName) == 0)
		{
			int j;
			for (j = 0; j < NUM_ROOMS; j++)
			{
				if (strcmp(Rooms[j].name,nextRoomName) == 0) 
				{
					return j;
				}
			}
		}
	}
	/* ERROR, Room connection not found! */
	printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
	return currentRoom;
}

void PrintCurrentRoom(struct Room Rooms[], int currentRoom)
{
		printf("CURRENT LOCATION: %s\n", Rooms[currentRoom].name);
		printf("POSSIBLE CONNECTIONS: ");
		int i;
		int numConnections = 0;
		for (i = 0; i < NUM_CONS; i++)
		{
			if (strcmp(Rooms[currentRoom].connection[i],"") != 0)
			{
				numConnections++;
				if (numConnections > 1)
					printf(", ");
				printf("%s",Rooms[currentRoom].connection[i]);
			}
		}
		printf(".\n");

}


int IsEndRoom(struct Room Rooms[], int X)
{
	if (strcmp(Rooms[X].roomtype,"END_ROOM") == 0)
	{
		return 1;
	}
	return 0;

}

void InitRooms(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		memset(Rooms[i].name,'\0',sizeof(Rooms[i].name) );
		memset(Rooms[i].roomtype,'\0', sizeof(Rooms[i].roomtype) );
		int j;
		for (j = 0; j < NUM_CONS; j++)
		{
			memset( Rooms[i].connection[j] ,'\0' , sizeof(Rooms[i].connection[j] ) );
		}
	}

}

void ReadFiles(struct Room Rooms[])
{
	int newestDirTime = -1; // Modified timestamp of newest subdir examined
	char targetDirPrefix[32] = "jonesro4.rooms."; // Prefix we're looking for
	char newestDirName[256]; // Holds the name of the newest dir that contains prefix
        memset(newestDirName, '\0', sizeof(newestDirName));

	DIR* dirToCheck;
	struct dirent *fileInDir;
	struct stat dirAttributes;

	dirToCheck = opendir(".");


	fileInDir = NULL;
	if (dirToCheck > 0)
	{
		while((fileInDir = readdir(dirToCheck)) != NULL)
		{
			if(strstr(fileInDir->d_name, targetDirPrefix) != NULL)
			{

	        		stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

		        	if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
			        {
					newestDirTime = (int)dirAttributes.st_mtime;
					memset(newestDirName, '\0', sizeof(newestDirName));
					strcpy(newestDirName, fileInDir->d_name);
				}
			}
		}
	}
	
	closedir(dirToCheck);
	/* Now go to most recently created room directory,
	 * Read each of the room files and save the room to a Room struct in the provided Rooms array
	 */
	dirToCheck = opendir(newestDirName);
	fileInDir = NULL;
	if (dirToCheck > 0)
	{
		int roomNum = 0;
		while ( ( fileInDir = readdir(dirToCheck) ) != NULL )
		{	
			if ( strcmp( fileInDir->d_name, "." ) != 0 && strcmp( fileInDir->d_name, ".." ) != 0 )
			{
				char filePath[250];
				sprintf(filePath, "%s/%s", newestDirName, fileInDir->d_name);

				FILE * fp;
				fp = fopen(filePath, "r");

				char line[80];
				int connectionNum = 0;
				while ( fgets(line, 80, fp) != NULL) 
				{
					
					if (strstr(line,"NAME") != NULL) 
					{
						char name[9];
						sscanf(line, "%*s %*s %s", &name);
						strcpy(Rooms[roomNum].name,name);			
					} 
					else if (strstr(line,"CONNECTION" ) != NULL) 
					{
						char name[9];
						sscanf(line, "%*s %*s %s", &name);
						strcpy(Rooms[roomNum].connection[connectionNum], name);
						connectionNum++;
					} 
					else if (strstr(line,"TYPE" ) != NULL) {
						char roomtype[11];
						sscanf(line, "%*s %*s %s", &roomtype);
						strcpy(Rooms[roomNum].roomtype,roomtype);
					}
					
				}
				fclose(fp);
				roomNum++;
			}
		}
	}
	closedir(dirToCheck);
}

/* For Debug only */
void PrintRooms(struct Room Rooms[])
{
	int i;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		printf("ROOM NAME: %s\n", Rooms[i].name);
		int j;
		for (j = 0; j < NUM_CONS; j++)
		{
			if (strcmp(Rooms[i].connection[j],"") != 0)
			{
				printf("CONNECTION %d: %s\n", j+1, Rooms[i].connection[j]);
			}
		}
		printf("ROOM TYPE: %s\n\n", Rooms[i].roomtype);
	}
}


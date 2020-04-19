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

int main()
{
	struct Room Rooms[NUM_ROOMS];
	InitRooms(Rooms);

	ReadFiles(Rooms);
	PrintRooms(Rooms);
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

        			printf("Found the prefex: %s\n", fileInDir->d_name);
	        		stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

		        	if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
			        {
					newestDirTime = (int)dirAttributes.st_mtime;
					memset(newestDirName, '\0', sizeof(newestDirName));
					strcpy(newestDirName, fileInDir->d_name);
					printf("Newer subdir: %s, new time: %d\n",newestDirName, newestDirTime);
				}
			}
		}
	}


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
}


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


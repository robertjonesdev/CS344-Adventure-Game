/*****************************************************************
 *  Name: Robert Jones
 *  Class: CS344 Spring 2020
 *  Assignment: Program 2
 *  Due: May 3rd, 2020
 *
 *  file: jonesro4.adventure.c
 *
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>

/* Constant Definitions: maximum number of rooms and maximum number of connections for each room */
#define NUM_ROOMS 7
#define NUM_CONS 6

/* Struct definition that represents a Room */
struct Room {
	char name[9];
	char connection[NUM_CONS][9];
	char roomtype[11];
};


/* Global variables for mutex time function */
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int keep_running = 1;


/* Function declarations */
void InitRooms(struct Room Rooms[]);
void ReadFiles(struct Room Rooms[]);
void PrintRooms(struct Room Rooms[]);
int GetStartRoom(struct Room Rooms[]);
int IsEndRoom(struct Room Rooms[], int X);
void PrintCurrentRoom(struct Room Rooms[], int currentRoom);
void StorePath(char **Path, int *strlength, char roomname[9]);
void * PrintTime();


int main()
{

	/* For the time function, create a new mutex, lock, and create a new thread for time function */
	pthread_mutex_lock(&myMutex);
	pthread_t time_thread;
	int result_code = pthread_create(&time_thread, NULL, PrintTime, NULL); 

	/* Dynamically allocate a string to accumulate the path taken to victory
	 * The string will be reallocated in the function if more space is needed.
	 * The allocated memory will be freed at the end of main */
	int PathStrLength = 120;
	char *PathToVictory;
	PathToVictory = (char *) malloc(PathStrLength);
	memset(PathToVictory, '\0', sizeof(PathToVictory));

	/*
	 * Declare and initialize the array of Rooms structs
	 * Initialize to clear out the data
	 * Read the room files and store in to the Rooms structs
	 */
	struct Room Rooms[NUM_ROOMS];
	InitRooms(Rooms);
	ReadFiles(Rooms);

	int currentRoom = GetStartRoom(Rooms);

	/*
	 * Game Loop
	 */
	char userInput[80];
	int stepCounter = 0;
	while(IsEndRoom(Rooms,currentRoom) != 1)
	{
		/* Begin with printing the description of the current room */
		PrintCurrentRoom(Rooms, currentRoom);

		/* Provide a prompt and gather user input */
		printf("WHERE TO? >");
		scanf("%s", userInput);

		if (strcmp(userInput,"time") == 0) 
		{
			/* command entered is 'time */

			/* Unlock Mutex so that time can write to file */
			pthread_mutex_unlock(&myMutex);
			result_code = pthread_join(time_thread, NULL);
			pthread_mutex_lock(&myMutex);	
			
			/* After PrintTime finishes, lock the mutex, 
			 * read the time file and print to console */
			
			FILE * fptr;
			fptr = fopen("currentTime.txt","r");
			char line[80];
			while ( fgets(line, 80, fptr) != NULL)
			{
				printf("\n%s",line);
			}

			fclose(fptr);

			/* Create a new time thread */
			result_code = pthread_create(&time_thread, NULL, PrintTime, NULL);
	
		}
		else 
		{
			/* command entered is not 'time' 
			 * Use MoveRooms() to receive the new current room.
			 */
			int newRoom  = MoveRooms(Rooms, currentRoom, userInput);
			/* if the new room is different than the current room
			 * then the move was valid. Increase the step counter,
			 * set the current room 
			 * and call StorePath to store the new room to the
			 * Path To Victory string
			 */
			if (newRoom != currentRoom) 
			{
				stepCounter++;
				currentRoom = newRoom;
				StorePath(&PathToVictory, &PathStrLength, Rooms[newRoom].name);				
			}
		}
		printf("\n");
	}
	/* Display the winning message */
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	
	/* Display the total steps from the stepCounter */
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepCounter);
	
	/* Print the path of victory */
	printf("%s", PathToVictory);
	free(PathToVictory);
	
	/* Kill the time thread, without printing time to file 
	 * Rejoin it back to main thread and exit program */
	keep_running = 0;
	pthread_mutex_unlock(&myMutex);
	result_code = pthread_join(time_thread, NULL);
	pthread_mutex_destroy(&myMutex);

	return 0;
}


/* Print Time
 * This is a threaded function that is began at the start of main.
 * The thread will lock the myMutex (global variable) and then
 * print the current time to a file named currentTime.txt
 * Once the file is written, the mutex will be unlcoked.
 */
void * PrintTime() 
{
	time_t t;
	struct tm *tmp;
	char buffer[80];

	pthread_mutex_lock(&myMutex);

	if (keep_running)
	{
		time ( &t );
		tmp = localtime( &t );
		strftime(buffer,80,"%l:%M%P, %A, %B %d, %Y", tmp);
		FILE * fp;
		fp = fopen("currentTime.txt","w");
		fprintf(fp, "%s\n",buffer);
		fclose(fp);
	} 
	
	pthread_mutex_unlock(&myMutex);
	
	return NULL;
}


/* Get Start Room
 * Receives: Array of room structs
 * Returns: room number integer
 * This function searches through each of the rooms looking for the Room Type 
 * of Start Rooms. There should only be one, so it will return the first one 
 * it enounters. The room number represents the array element.
 */
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


/* Store Path
 * Receives: String Pointer, Integer Points of str length, Room name (string)
 * This function keep track of the path taken to victory. It stores each room name
 * in a character string separated by new line characters. If the length of the string
 * gets within 20 characters of the maximum size, the it will dynamically reallocate 
 * memory to the string. 
 * Prerequisites: the string must be dynamically allocated.
 */
void StorePath(char **Path, int *strlength, char roomname[9])
{

	int len = strlen(*Path);
	/* if the string length is within 20 characters of the maximum, resize
	 * the array by 2xlength
	 */
	if (len > (*strlength - 20))
	{
		*Path = (char *) realloc(*Path,sizeof(char)*(*strlength *2));
		
		/* keep track of the stirng length for future use*/
		*strlength = *strlength * 2;
	}
	/* store the room name and a new line character to separate */
	strcat(*Path, roomname);
	strcat(*Path, "\n");
}


/* Move Rooms 
 *
 */
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


/* Print Current Room
 * Receives: Array of Rooms struct, integer of which room to print
 * This function prints to stdout a description of the current room 
 * and all possible connections going out of the room.
 */ 
void PrintCurrentRoom(struct Room Rooms[], int currentRoom)
{
		printf("CURRENT LOCATION: %s\n", Rooms[currentRoom].name);
		printf("POSSIBLE CONNECTIONS: ");
		int i;
		int numConnections = 0;
		for (i = 0; i < NUM_CONS; i++)
		{
			/* only print a connection if it has a value */
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


/* Is End Room
 * Receives: Array of Rooms structs, integer of which room to evaluate
 * This function returns true (1) or false (0) if the room argument 
 * is the end room.
 */
int IsEndRoom(struct Room Rooms[], int X)
{
	if (strcmp(Rooms[X].roomtype,"END_ROOM") == 0)
	{
		return 1;
	}
	return 0;

}


/* Init Rooms
 * Receives: Array of Rooms structs
 * This function initializes a provided room struct with empty strings for the name, 
 * connections, and room type. Because the array is passed by reference
 * the values are changed without returning.
 */
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


/* Read Files
 * Receives: Array of Room structs
 * This function opens the most recently created directory with a prefix of
 * jonesro4.rooms and opens all files (except . and ..) and loads the
 * room contents into the Room struct array.
 */
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


	/* Open the current working directory and look for a directory of the given
	 * targetDirPrefix that was most recently created.
	 * Save the directory name to dirToChecK
	 */
	fileInDir = NULL;
	if (dirToCheck > 0)
	{
		while((fileInDir = readdir(dirToCheck)) != NULL)
		{
			if(strstr(fileInDir->d_name, targetDirPrefix) != NULL)
			{

	        		stat(fileInDir->d_name, &dirAttributes); 
				// Get attributes of the entry

		        	if ((int)dirAttributes.st_mtime > newestDirTime) 
				// If this time is bigger
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
		/* loop through each file in the directory */
		while ( ( fileInDir = readdir(dirToCheck) ) != NULL )
		{	
			/* Ignore files "." and ".." */
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
					
					/* Room name, generally the first line */
					if (strstr(line,"NAME") != NULL) 
					{
						/* save line to room name */
						char name[9];
						sscanf(line, "%*s %*s %s", &name);
						strcpy(Rooms[roomNum].name,name);			
					} 
					/* Room connection lines 2 through 4, 5, 6, or 7 */
					else if (strstr(line,"CONNECTION" ) != NULL) 
					{
						/* save line to room connection */
						char name[9];
						sscanf(line, "%*s %*s %s", &name);
						strcpy(Rooms[roomNum].connection[connectionNum], name);
						connectionNum++;
					} 
					/* Room type, generally the last line */
					else if (strstr(line,"TYPE" ) != NULL) 
					{
						/* save line to room type */
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

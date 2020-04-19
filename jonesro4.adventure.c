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
	char roomtype[1];
};

void ReadFiles();

int main()
{
	ReadFiles();
	return 0;
}


void ReadFiles()
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
					printf("Newer subdir: %s, new time: %d\n",
					fileInDir->d_name, newestDirTime);
				}
			}
		}
	}
}

/*
			if(strcmp(fileInDir->d_name, ".") != 0 && strcmp(fileInDir->d_name) != 0)
			{
				char filePath[250];
				sprintf(filePath, "%s/%s", directoryName, fileInDir->d_name);

				FILE * fp;
				fp = fopen(filePath, "r");

				char line[80];
				while ( fgets(line, 80, fp) != NULL) 
				{
					
				}
				fclose(fp);
			}
*/

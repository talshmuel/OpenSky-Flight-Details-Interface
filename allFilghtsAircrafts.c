#define _CRT_SECURE_NO_WARNINGS
#include <unistd.h>
#include "allFilghtsAircrafts.h"


char* allFlightsAircrafts(int argc, char** argv, int* bufferSize)
{
   
    Flight1* arr = (Flight1*)malloc(sizeof(Flight1));
    int numOfAircraft = argc;
    int phySize = 1, logSize = 0;
    int i;

    char** aircraftsNames = (char**)malloc(numOfAircraft * sizeof(char*));
    for (i = 0; i < numOfAircraft; i++)
    {
        aircraftsNames[i]=(char*)malloc(7*sizeof(char));
        strcpy(aircraftsNames[i],argv[i]);
    }
    
    
    DIR* dir;
    struct dirent* entry;
    // open the directory
    dir = opendir("./flightDB");
   
    if (dir == NULL) {
        printf("Failed to open directory\n");

    }

   
    while ((entry = readdir(dir)) != NULL) {
        
 
         if (entry->d_type != DT_DIR) {
            continue;
        }
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
    
        char fileNameArv[100];
        char fileNameDep[100];
   
    strcpy(fileNameArv, "./flightDB/");
	strcat(fileNameArv, entry->d_name);
	strcat(fileNameArv, "/");
	strcat(fileNameArv, entry->d_name);
	strcat(fileNameArv, ".arv");

	strcpy(fileNameDep, "./flightDB/");
	strcat(fileNameDep, entry->d_name);
	strcat(fileNameDep, "/");
	strcat(fileNameDep, entry->d_name);
	strcat(fileNameDep, ".dep");
	 
	FILE* fpArv = fopen(fileNameArv, "r");
        arr = getDataFromFileToArray(fpArv,arr, &phySize, &logSize, aircraftsNames, numOfAircraft);
        fclose(fpArv);
        
        FILE* fpDep = fopen(fileNameDep, "r");
        arr = getDataFromFileToArray(fpDep,arr, &phySize, &logSize, aircraftsNames, numOfAircraft);
        fclose(fpDep);
        
    }
   
  
    char* buffer2 = printAllFlightOfTheAirCrafts(arr, logSize , bufferSize);
	
    
    
    free(arr);

	
   
    for(int i=0 ; i < numOfAircraft;i++)
    { 
         
         free(aircraftsNames[i]);
    }
     
    free(aircraftsNames);

	// close the directory
    closedir(dir);

	
	return buffer2;
	
	
	

}

Flight1* getDataFromFileToArray(FILE* fp, Flight1* arr, int* phSize, int* lgSize, char** airCraftsNames, int sizeNames)
{

	char aircraftname[7];
	char c[2];
	char str[100];
	
        
	int rowCounter = countRowInFile(fp);
	
	fseek(fp, 73, SEEK_SET);
	
	for (int i = (*lgSize); i < rowCounter - 2; i++)
	{
		
		fscanf(fp, "%6s", aircraftname);
		aircraftname[6] = '\0';
		
		for (int j = 0; j < sizeNames; j++)
		{
			if (strcmp(airCraftsNames[j], aircraftname) == 0)
			{       
			
	                    	if (*phSize <= *lgSize)
	                        {              
		                        (*phSize) = (*phSize) * 2;
		                        arr = (Flight1*)realloc(arr, (*phSize) * sizeof(Flight1));
	                        }	        
	 			
				j = sizeNames;
				strcpy(arr[*lgSize].icao24, aircraftname);

				fscanf(fp, "%1s", c);
				c[1] = '\0';
				fscanf(fp, "%10s", arr[*lgSize].time_dpt);
				arr[*lgSize].time_dpt[10] = '\0';


				fscanf(fp, "%1s", c);
				c[1] = '\0';
				fscanf(fp, "%4s", arr[*lgSize].ap_dpt);
				arr[*lgSize].ap_dpt[4] = '\0';

				fscanf(fp, "%1s", c);
				c[1] = '\0';
				fscanf(fp, "%10s", arr[*lgSize].time_arv);
				
				fscanf(fp, "%1s", c);
				c[1] = '\0';
				fscanf(fp, "%4s", arr[*lgSize].ap_arv);
				fseek(fp, 9, SEEK_CUR);
				
				(*lgSize)++;
				j=sizeNames;
			}

			else if(j == sizeNames-1) {
			
				fscanf(fp, "%42s", str);
			}
		}
	}
      
        
	
	if(*lgSize!=0){
	       arr = (Flight1*)realloc(arr, (*lgSize) * sizeof(Flight1));
	       (*phSize) = (*lgSize);
	}      
	else       
	       (*phSize)=1;
	
	return arr;
}

char* printAllFlightOfTheAirCrafts(Flight1* arr, int lgSize , int* bufferSize)
{
	
	int size = (lgSize)*(90);
   
    char* buffer1;
    buffer1 = (char*)malloc(size*sizeof(char));
   

    buffer1[0] = '\0';
	
	for (int i = 0; i < lgSize; i++)
	{
		strcat(buffer1,arr[i].icao24);
		strcat(buffer1," departed from ");
		strcat(buffer1,arr[i].ap_dpt);
		strcat(buffer1," at ");
		strcat(buffer1,arr[i].time_dpt);

		strcat(buffer1," arrived in ");
		strcat(buffer1,arr[i].ap_arv);
		strcat(buffer1," at ");
		strcat(buffer1,arr[i].time_arv);

		strcat(buffer1,"\n");
	}

	size = strlen(buffer1);
    buffer1 = (char*)realloc(buffer1,(size+1)*sizeof(char));
    
    *bufferSize = size;
	

    return buffer1;

	
}

int countRowInFile(FILE* fp)
{
	char c;
	int rowCounter = 0;


	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			rowCounter++;
	}
	fseek(fp, 0, SEEK_SET);
	return rowCounter;
}


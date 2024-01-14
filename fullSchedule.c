#define _CRT_SECURE_NO_WARNINGS
#include "fullSchedule.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* fullAirportSchedule(int argc, char** argv, int* bufferSize)
{
  
    if (argc < 1)
        exit(1);

    char fileNameArv[100];
    char fileNameDep[100];

    strcpy(fileNameArv, "./flightDB/");
    strcat(fileNameArv, argv[0]);
    strcat(fileNameArv, "/");
    strcat(fileNameArv, argv[0]);
    strcat(fileNameArv, ".arv");

    strcpy(fileNameDep, "./flightDB/");
    strcat(fileNameDep, argv[0]);
    strcat(fileNameDep, "/");
    strcat(fileNameDep, argv[0]);
    strcat(fileNameDep, ".dep");

    FILE* fpArv = fopen(fileNameArv, "r");
    FILE* fpDep = fopen(fileNameDep, "r");

    if (fpArv == NULL || fpDep == NULL)
    {
        printf("The airport not found in the data base\n");
        char* buffer = (char*)malloc(2*sizeof(char));
        bufferSize=1;
        return buffer;
        // printf("The airport not found in the data base\n");
        // exit(1);
    }

    char* buffer4 = airportSchedule(fpArv, fpDep , bufferSize);
   
    
    fclose(fpArv);
    fclose(fpDep);
    

    return buffer4;

   
   
}


char* airportSchedule(FILE* fpArv, FILE* fpDep,int* bufferSize)
{
    int j, i, rowCountArv = 0, rowCountDep = 0;
    char c;
    char temp[20];



    while ((c = fgetc(fpArv)) != EOF) {
        if (c == '\n') {
            rowCountArv++;
        }
    }



    fseek(fpArv, 0, SEEK_SET);
    fseek(fpArv, 73, SEEK_CUR);

    while ((c = fgetc(fpDep)) != EOF) {
        if (c == '\n') {
            rowCountDep++;
        }
    }



    fseek(fpDep, 0, SEEK_SET);
    fseek(fpDep, 74, SEEK_SET);

    int rowsArray = rowCountArv + rowCountDep - 2;

    Flight2** allDetails = (Flight2**)malloc(rowsArray * sizeof(Flight2*));
    for (i = 0; i < rowCountArv - 2; i++)
    {
        allDetails[i] = (Flight2*)malloc(sizeof(Flight2));
        char time[11];
        allDetails[i]->type = 'A';

        fscanf(fpArv, "%18s", temp);
        fscanf(fpArv, "%4s", allDetails[i]->secondAirport);
        allDetails[i]->secondAirport[4] = '\0';
        fscanf(fpArv, "%1s", temp);
        fscanf(fpArv, "%10s", time);
        time[10] = '\0';
        allDetails[i]->time = atoi(time);
        fscanf(fpArv, "%6s", temp);

        int lenghtOfCallsign = 0;
        char callSign = '#';
        int index = 0;
        while (callSign != '\n')
        {
            allDetails[i]->callSign[index] = callSign;
            callSign = fgetc(fpArv);
            index++;
        }

        allDetails[i]->callSign[9] = '\0';

    }


    for (j = i; j < rowsArray - 2; j++)
    {

        allDetails[j] = (Flight2*)malloc(sizeof(Flight2));
        char time[11];
        allDetails[j]->type = 'D';
        fscanf(fpDep, "%6s", temp);
        fscanf(fpDep, "%10s", time);
        time[10] = '\0';
        allDetails[j]->time = atoi(time);
        fscanf(fpDep, "%17s", temp);
        fscanf(fpDep, "%4s", allDetails[j]->secondAirport);
        allDetails[j]->secondAirport[4] = '\0';
        fscanf(fpDep, "%1s", temp);


        char callSign = '#';
        int index = 0;
        while (callSign != '\n')
        {
            allDetails[j]->callSign[index] = callSign;
            callSign = fgetc(fpDep);
            index++;
        }

        allDetails[j]->callSign[9] = '\0';
        fscanf(fpDep, "%1s", temp);


    }


    qsort(allDetails, rowsArray - 3, sizeof(Flight2*), swap);
    char* buffer3= printTable(rowsArray, allDetails,bufferSize);
    
  
  
      

    for (int k = 0; k < rowsArray - 3; k++)
    {
        free(allDetails[k]);
    }
    free(allDetails);
    return buffer3;
    
   
  
  
}

int swap(const void* a, const void* b) {
    const Flight2** ia = (const Flight2**)a;
    const Flight2** ib = (const Flight2**)b;
    return (*ia)->time - (*ib)->time;
}

char* printTable(int rowsArray, Flight2** allDetails,int* bufferSize)
{
    char str[11];
    int size = (rowsArray - 3)*(80);
   
    char* buffer1 = NULL;
    buffer1 = (char*)malloc(size*sizeof(char));
   

    buffer1[0] = '\0';

    for (int i = 0; i < rowsArray - 3; i++)
    {
        strcat(buffer1,"Flight ");
        strcat(buffer1, allDetails[i]->callSign);
        


        
        if (allDetails[i]->type == 'A'){
            strcat(buffer1, "arriving from ");
            strcat(buffer1, allDetails[i]->secondAirport);
            
        }
            
        else{
            strcat(buffer1," departing to ");
            strcat(buffer1,allDetails[i]->secondAirport);

        }
           
        strcat(buffer1, " at ");
        sprintf(str,"%d",allDetails[i]->time);
        strcat(buffer1, str);
        strcat(buffer1, "\n");
    }

   
    size = strlen(buffer1);
    buffer1 = (char*)realloc(buffer1,sizeof(char)*(size+1));

    *bufferSize = size;

    return buffer1;
}


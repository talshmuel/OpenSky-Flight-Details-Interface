#define _CRT_SECURE_NO_WARNINGS
#include "airportsIcoa.h"

char* allArrivalFlightsDetails(int argc ,char** argv , int* bufferSize)
{
	if (argc == 0)
		printf("Error: No airport code entered\n");
	else
	{
		char fileName[26]; 
		int numOfRow;
		FILE* airportArvFile = NULL;
		Flight3** airportFlight = (Flight3**)malloc((argc) * sizeof(Flight3*)); //Each cell in the array represents the flights of a certain airport
		int* arrSizes = (int*)malloc((argc) * sizeof(int));

		for (int i = 0; i < argc; i++)
		{
			createNameFile(fileName, argv[i]);
			airportArvFile = fopen(fileName, "r");
			if (!airportArvFile) {
				perror("Invalid airport code");
                char* buffer = (char*)malloc(2*sizeof(char));
                bufferSize=1;
                return buffer;
			}
			numOfRow = countNumOfRowInFile(airportArvFile);
			airportFlight[i] = (Flight3*) malloc((numOfRow - 2) * sizeof(Flight3));
			if (!airportFlight[i]) {
				perror("malloc failed");
				exit(EXIT_FAILURE);
			}
			arrSizes[i] = numOfRow - 2;
			insertData(airportArvFile, airportFlight[i], numOfRow - 2);
		}
		char* buffer1 = printData(airportFlight, argc, arrSizes, argv,bufferSize);
		freeData(airportFlight, argc);
		free(arrSizes);
		fclose(airportArvFile);

        return buffer1;

	}
}

void createNameFile(char* res, char* airport)
{
    
    strcpy(res, "./flightDB/");
    strcat(res, airport);
    strcat(res, "/");
    strcat(res, airport);
    strcat(res, ".arv");
}
int countNumOfRowInFile(FILE* f)
{
    int c, res = 0;
    while ((c = fgetc(f)) != EOF)
        if (c == '\n')
            res++;
    fseek(f, 72, SEEK_SET);
    return res;
}
void insertData(FILE* f, Flight3* flightArr, int numOfFlights)
{
    int j;
    char tmp[7];
    tmp[0] = ' ';
    for (int i = 0; i < numOfFlights; i++)
    {
        j = 0;
        while (tmp[0] == '\n' || tmp[0] == ' ')
        fread(tmp, sizeof(char), 1, f);
        fread(tmp, sizeof(char), 6, f);
        fread(flightArr[i].dptTime, sizeof(char), 10, f);
        flightArr[i].dptTime[10] = '\0';
        fread(tmp, sizeof(char), 1, f);
        fread(flightArr[i].srcAirport, sizeof(char), 4, f);
        flightArr[i].srcAirport[4] = '\0';
        fread(tmp, sizeof(char), 1, f);
        fread(flightArr[i].arvTime, sizeof(char), 10, f);
        flightArr[i].arvTime[10] = '\0';
        fread(tmp, sizeof(char), 6, f);
        fread(flightArr[i].flightNum + j, sizeof(char), 1, f);
        while (flightArr[i].flightNum[j] != '\n' && flightArr[i].flightNum[j] != ' ')
        {
            j++;
            fread(flightArr[i].flightNum + j, sizeof(char), 1, f);
        }
        flightArr[i].flightNum[j] = '\0';
        tmp[0] = ' ';
    }
}
char* printData(Flight3** FlightArr, int numOfAirports, int* numOfFlights, char** airportsNames,int* bufferSize)
{
    char* buffer;
    int size = numOfAirports;
    for(int i=0; i<numOfAirports;i++)
    {
        (size) += numOfFlights[i];
    }

    size = size * 100;
    
    buffer = (char*)malloc((size)*sizeof(char));
    buffer[0] = '\0';

    for (int i = 0; i < numOfAirports; i++)
    {
        strcat(buffer,"Flights to the airport ");
        strcat(buffer,airportsNames[i]);
        strcat(buffer,"\n");

        for (int j = 0; j < numOfFlights[i]; j++)
        {
        strcat(buffer,"Flights #");
        strcat(buffer,FlightArr[i][j].flightNum);
        strcat(buffer," arriving from ");
        strcat(buffer,FlightArr[i][j].srcAirport);
        strcat(buffer,", tookoff at ");
        strcat(buffer,FlightArr[i][j].dptTime);
        strcat(buffer," landed at ");
        strcat(buffer,FlightArr[i][j].arvTime);
        strcat(buffer,"\n");
        }
    }
    size = strlen(buffer);
    buffer = (char*)realloc(buffer,(size+1)*sizeof(char));
    
    *bufferSize = size;
   

    return buffer;
}

void freeData(Flight3** arr, int size)
{
    for (int i = 0; i < size; i++)
        free(arr[i]);
    free(arr);
}

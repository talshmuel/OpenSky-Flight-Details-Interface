#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct 
{
	char icao24[7];
	char ap_dpt[5];
	char time_dpt[11];
	char ap_arv[5];
	char time_arv[11];
}Flight1;

char* allFlightsAircrafts(int argc, char** argv, int* bufferSize);
Flight1* getDataFromFileToArray(FILE* fp, Flight1* arr, int* phSize, int* lgSize, char** airCraftsNames, int sizeNames);
char* printAllFlightOfTheAirCrafts(Flight1* arr, int lgSize, int* bufferSize);
int countRowInFile(FILE* fp);


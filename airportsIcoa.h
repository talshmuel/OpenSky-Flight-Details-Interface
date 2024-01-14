#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct 
{
	char flightNum[8];
	char dptTime[11];
	char arvTime[11];
	char srcAirport[5];
}Flight3;

char* allArrivalFlightsDetails(int argc ,char** argv , int* bufferSize);
void createNameFile(char* res, char* aiport);
int countNumOfRowInFile(FILE* f);
void insertData(FILE* f, Flight3* flightArr, int numOfFlights);
char* printData(Flight3** FlightArr, int numOfAirports, int* numOfFlights, char** airportsNames,int* bufferSize);
void freeData(Flight3** arr, int size);


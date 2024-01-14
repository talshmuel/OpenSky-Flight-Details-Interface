#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct  {
	char type;
	char callSign[10];
	char secondAirport[5];
	int time;

}Flight2;

char* fullAirportSchedule(int, char**,int* );
char* airportSchedule(FILE*, FILE*,int*);
int swap(const void*, const void*);
char* printTable(int, Flight2**,int*);



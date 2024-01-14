#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "updateDB.h"

#define FLIGHT_DB_DIR "./flightDB"

int updateDB() {
    DIR* dir;
    struct dirent* entry;

    dir = opendir(FLIGHT_DB_DIR);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    char** airport_names = (char**)malloc(sizeof(char*));
    int number_of_airports = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) {
            continue;
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char* airport_name = entry->d_name;

        number_of_airports++;
        airport_names = (char**)realloc(airport_names, sizeof(char*) * number_of_airports);
        airport_names[number_of_airports - 1] = (char*)malloc(sizeof(char) * strlen(airport_name));
        strcpy(airport_names[number_of_airports - 1], airport_name);





        char remove_command[100];
        snprintf(remove_command, 100, "rm -rf %s/%s", FLIGHT_DB_DIR, airport_name);

        system(remove_command);
    }

    //chdir("../");
    for (int i = 0; i < number_of_airports; i++) {
        char flight_scanner_command[100];
        strcpy(flight_scanner_command, "bash ./flightScanner.sh ");
        strcat(flight_scanner_command, airport_names[i]);
        system(flight_scanner_command);

        free(airport_names[i]);
    }
    //chdir("./program4");

    free(airport_names);

    closedir(dir);
}

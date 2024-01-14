#define _CRT_SECURE_NO_WARNINGS
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <zip.h>
#include <errno.h>

int file_exists(const char*);
void delete_directory(const char* directory_path);
void zip_directory(const char *directory_path, const char *output_path, struct zip *archive, const char *base_directory);
void zip_directories(const char* base_directory, const char* output_path);
void zipAllFiles();
void unZip(const char* zipFilePath, const char* destinationDirectory);
void unZipAllFiles();
int directoryExists();
#endif // !HEADER_H
#include <limits.h>
#include "zipFiles.h"

int file_exists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}


void delete_directory(const char *directory_path)
{
    
    DIR *dir = opendir(directory_path);
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            char entry_path[512];
            snprintf(entry_path, sizeof(entry_path), "%s/%s", directory_path, entry->d_name);
            if (entry->d_type == DT_DIR)
            {
                delete_directory(entry_path);
            }
            else if (entry->d_type == DT_REG)
            {
                if (remove(entry_path) != 0)
                {
                    fprintf(stderr, "Error deleting file '%s'\n", entry_path);
                }
            }
        }
        closedir(dir);
    }
    if (remove(directory_path) != 0)
    {
        fprintf(stderr, "Error deleting directory '%s'\n", directory_path);
    }
}

void zip_directory(const char *directory_path, const char *output_path, struct zip *archive, const char *base_directory)
{
    struct zip_source *source;

    DIR *dir = opendir(directory_path);
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                char subdir_path[512];
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", directory_path, entry->d_name);
                char relative_path[512];
                snprintf(relative_path, sizeof(relative_path), "%s/%s", base_directory, entry->d_name);
                zip_directory(subdir_path, output_path, archive, relative_path);
            }
            else if (entry->d_type == DT_REG)
            {
                char file_path[512];
                snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);
                if (file_exists(file_path))
                {
                    source = zip_source_file_create(file_path, 0, -1, NULL);
                    if (!source)
                    {
                        fprintf(stderr, "Error creating zip source for file '%s'\n", file_path);
                        continue;
                    }

                    char relative_path[512];
                    snprintf(relative_path, sizeof(relative_path), "%s/%s", base_directory, entry->d_name);
                    if (zip_file_add(archive, relative_path, source, ZIP_FL_ENC_UTF_8) == -1)
                    {
                        fprintf(stderr, "Error adding file '%s' to zip archive\n", file_path);
                        zip_source_free(source);
                        continue;
                    }
                }
            }
        }
        closedir(dir);
    }
    else
    {
        fprintf(stderr, "Error opening directory '%s'\n", directory_path);
        return;
    }
}

void zip_directories(const char *base_directory, const char *output_path)
{
    struct zip *archive;

    archive = zip_open(output_path, ZIP_CREATE | ZIP_TRUNCATE, NULL);
    if (!archive)
    {
        fprintf(stderr, "Error creating zip archive\n");
        return;
    }

    zip_directory(base_directory, output_path, archive, base_directory);

    if (zip_close(archive) == -1)
    {
        fprintf(stderr, "Error closing zip archive\n");
        return;
    }

    printf("Directories in '%s' successfully zipped to '%s'\n", base_directory, output_path);

}

void zipAllFiles()
{
    const char *base_directory = "./flightDB";
    const char *output_path = "flightDB.zip";

    zip_directories(base_directory, output_path);
}

void unZip(const char *zipFilePath, const char *destinationDirectory)
{
    char command[1000];

    snprintf(command, sizeof(command), "unzip %s -d %s", zipFilePath, destinationDirectory);

    system(command);
}

void unZipAllFiles()
{
    const char *zipFilePath = "./flightDB.zip";
    const char *destinationDirectory = "../runMe";

    unZip(zipFilePath, destinationDirectory);
}

int directoryExists()
{
    const char *path = "./flightDB";
    DIR *dir = opendir(path);
    if (dir)
    {
        // Directory exists
        closedir(dir);
        return 1;
    }
    else
    {
        // Directory does not exist or cannot be opened
        return 0;
    }
}
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <unistd.h>
#include "utilities.h"
#include "arraylist.h"

int getNumOfDirectories(char *input) {

    int fileCount = 0;
    int dirCount = 0;

    WIN32_FIND_DATA file;
    HANDLE fHandle;


    fHandle = FindFirstFile(input, &file);
    if (fHandle == INVALID_HANDLE_VALUE) {
        printf("Invalid File Handle.\n");
        return 0;
    }

    while (FindNextFile(fHandle, &file) != 0) {
        if (strcmp(file.cFileName, "..") == 0) continue;
        switch (file.dwFileAttributes) {
            case 16:
                dirCount++;
                break;
            case 32:
                fileCount++;
                break;
            default:
                break;
        }
    }
    printf("Directories: %d\nFiles: %d\n", dirCount, fileCount);
    return dirCount;
}

void getLargestDirectory(char input[], char dir_path[], struct iFile *maxFile, struct iFile *minFile) {
    int fileCount = 0;
    int dirCount = 0;
    unsigned long max = 0;
    unsigned long long min = ULONG_LONG_MAX;
    WIN32_FIND_DATA file;
    HANDLE fHandle;
    minFile->valid = 0;
    maxFile->valid = 0;


    fHandle = FindFirstFile(input, &file);
    if (fHandle == INVALID_HANDLE_VALUE) {
        printf("Invalid File Handle.\n");
        return;
    }

    while (FindNextFile(fHandle, &file) != 0) {
        if (file.nFileSizeLow > max) {
            maxFile->valid = 1;
            max = file.nFileSizeLow;
            strcpy(maxFile->name, file.cFileName);
            strcpy(maxFile->extension, get_extension(file.cFileName));
            strcpy(maxFile->path, dir_path);
            strcat(maxFile->path, file.cFileName);
        }

        if (file.dwFileAttributes == 32) {
            if (file.nFileSizeLow < min) {
                minFile->valid = 1;
                min = file.nFileSizeLow;
                strcpy(minFile->name, file.cFileName);
                strcpy(minFile->extension, get_extension(file.cFileName));
                strcpy(minFile->path, dir_path);
                strcat(minFile->path, file.cFileName);
            }
        }
        if (strcmp(file.cFileName, "..") == 0) continue;
        switch (file.dwFileAttributes) {
            case 16:
                dirCount++;
                break;
            case 32:
                fileCount++;
                break;
            default:
                break;
        }
    }
    maxFile->valid = 1;
    maxFile->size = max;


}

void getExtensionCount(char input[], struct arraylist *array) {

    WIN32_FIND_DATA file;
    HANDLE fHandle;


    fHandle = FindFirstFile(input, &file);
    if (fHandle == INVALID_HANDLE_VALUE) {
        printf("Invalid File Handle.\n");
        return;
    }

    while (FindNextFile(fHandle, &file) != 0) {
        if (file.dwFileAttributes == 32) {
            char *extension = get_extension(file.cFileName);
            extensionTypes(extension, array);
        }
    }
}


int main(int argc, char *argv[]) {
    struct arraylist arraylist;
    if (argc == 2) {
        char star = '*';

        char *parsed_input = parser(argv[1], "\\", "\\\\");

        char *result = strncat(parsed_input, &star, 1);
//        getNumOfDirectories(result);


        struct iFile maxFile;
        struct iFile minFile;
//        getLargestDirectory(result, argv[1], &maxFile, &minFile);
        getExtensionCount(result, &arraylist);
        for (int i = 0; i < arraylist.size; ++i) {
            printf("extension %s count %d\n", arraylist.extensions[i].extension, arraylist.extensions[i].count);
        }

    } else if (argc > 2) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("One argument expected.\n");
    }
}


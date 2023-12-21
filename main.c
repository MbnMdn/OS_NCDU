#include <stdio.h>
#include <windows.h>

int getNumOfDirectories(char *argv[]) {

    int fileCount = 0;
    int dirCount = 0;

    WIN32_FIND_DATA file;
    HANDLE fHandle;


    fHandle = FindFirstFile(argv[1], &file);
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

}


int main(int argc, char *argv[]) {
    if (argc == 2) {

        getNumOfDirectories(argv);
    } else if (argc > 2) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("One argument expected.\n");
    }
}
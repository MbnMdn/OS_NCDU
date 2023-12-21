#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

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

char* parser(const char* s, const char* oldW,
                  const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    char star = '*';
    strncat(result,&star,i++);
    result[i] = '\0';
    return result;
}


int main(int argc, char *argv[]) {
    if (argc == 2) {

        getNumOfDirectories(argv);
    } else if (argc > 2) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("One argument expected.\n");
    }

    printf("%s", parser("D:\\h\\j",  "\\",  "\\\\"));


}


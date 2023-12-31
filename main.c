#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "utilities.h"
#include "arraylist.h"

void initDirectoryTask(struct task *task, char input[]) {
    DIR *dirp;
    struct dirent *direntp;

    unsigned long max = 0;
    unsigned long long min = ULONG_LONG_MAX;

    unsigned long long folderSize = 0;

    dirp = opendir(input);
    if (dirp == NULL) {
        perror("Invalid File Handle.\\n");
    } else {
        for (;;) {
            direntp = readdir(dirp);
            if (direntp == NULL) break;

            if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0 ||
                strcmp(direntp->d_name, ".DS_Store") == 0)
                continue;

            // Directory
            if (direntp->d_type == 4) {
                char filepath[1000];
                sprintf(filepath, "%s%s/", input, direntp->d_name);

                struct iDirectory directory;
                directory.valid = 1;
                strcpy(directory.name, direntp->d_name);
                strcpy(directory.path, filepath);

                appendToDirectoryArray(task, directory);
            }
                // File
            else if (direntp->d_type == 8) {
                char filepath[1000];
                sprintf(filepath, "%s%s", input, direntp->d_name);

                struct iFile file;
                file.valid = 1;
                strcpy(file.name, direntp->d_name);
                strcpy(file.path, filepath);
                strcpy(file.extension, get_extension(direntp->d_name));

                long int fileSize = findSize(filepath);

                file.size = fileSize;
                folderSize = folderSize + fileSize;

                appendToFileArray(task, file);

                if (fileSize > max) {
                    task->maxSize = file;
                    max = fileSize;
                }

                if (fileSize < min) {
                    task->minSize = file;
                    min = fileSize;
                }

                task->dirSize = folderSize;

                char *extension = get_extension(direntp->d_name);
                extensionTypes(extension, task);


            }
        }
        closedir(dirp);
    }
}


int main(int argc, char *argv[]) {
    struct task *task = (struct task *) mmap(NULL, sizeof(struct task), PROT_READ | PROT_WRITE,
                                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (argc == 2) {

        initDirectoryTask(task, argv[1]);

        for (int i = 0; i < task->directoryCount; ++i) {
            int pipefd[2];
            int returnstatus = pipe(pipefd);

            if (returnstatus == -1) {
                printf("Pipe cannot be created\n");
                return 1;
            }

            pid_t pid;
            pid = fork();
            if (pid == 0) {
//                printf("Child %d\n", i);
                struct task childTask;
                initDirectoryTask(&childTask, task->directory[i].path);

                if (childTask.maxSize.size > task->maxSize.size){
                    task->maxSize = childTask.maxSize;
                }

                if (childTask.minSize.size < task->minSize.size){
                    task->minSize = childTask.minSize;
                }


                exit(0);
            } else if (pid > 0) {
//                printf("Parent\n");
                wait(&pid);
            } else {
                printf("Error\n");
            }
        }

        printf("max : %s\n" , task->maxSize.name);
        printf("min : %s" , task->minSize.name);

    } else if (argc > 2) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("One argument expected.\n");
    }


}


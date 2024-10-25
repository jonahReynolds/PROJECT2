#include <string.h>
#include <stdio.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){


    if (strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided");
        return;
    } 


    char base [64];
    char dir [64];
    struct NODE* parent = splitPath(pathName, base, dir);

    if (parent == NULL) {
        return;
    }


    struct NODE* curr = cwd->childPtr;


    while (curr != NULL) {
        if (strcmp(curr->name, base) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", base);
            return;
        }
        curr = curr->siblingPtr;
        
    }

    struct NODE* directory = (struct NODE*)malloc(sizeof(struct NODE));
    directory->parentPtr = parent;
    directory->childPtr = NULL;
    directory->siblingPtr = NULL;
    directory->fileType = 'D';
    strcpy(directory->name, base);

    if (parent->childPtr == NULL) {
        parent->childPtr = directory;
    } else {
        struct NODE* lastChild = parent->childPtr;
        while (lastChild->siblingPtr != NULL) {
            lastChild = lastChild->siblingPtr;
        }
        lastChild->siblingPtr = directory;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    char copy[128];
    strcpy(copy, pathName);
    

    char* slash = strrchr(copy, '/');
    
    if (slash != NULL) {
        strncpy(dirName, copy, slash - copy);
        dirName[slash - copy] = '\0';
        strcpy(baseName, slash + 1);
    } else {

        dirName[0] = '\0';
        strcpy(baseName, copy);
    }

    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
    }

    struct NODE* currentDir = cwd;
    
    if (dirName[0] == '\0') {
        return currentDir;
    } else {
        currentDir = root;

        char* token = strtok(dirName, "/");

        while (token != NULL) {

            struct NODE* curr = root;
            struct NODE* nextDir = NULL;

            curr = currentDir->childPtr;

            while (curr != NULL) {
                if (strcmp(curr->name, token) == 0) {
                    nextDir = curr;
                    break;
                }
                curr = curr->siblingPtr;
            }

            if (nextDir == NULL) {

                printf("ERROR: directory %s does not exist\n", token);
                return NULL;
            }
            currentDir = nextDir;
            token = strtok(NULL, "/");
        }

        return currentDir;
    }
}

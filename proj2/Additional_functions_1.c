#include "Header_file.h"
#include <stdlib.h>
#include <string.h>

/*Creates a new path item given a local name, a full name and a value*/
Path_item NEWpath(char *local_name,char *full_name, char *valor){
    Path_item new = (Path_item)malloc(sizeof(struct path_item));
    if(valor == NULL)
        new->path_value = NULL;
    else {
        new->path_value = (char *) malloc(strlen(valor) + 1);
        strcpy(new->path_value, valor);/*Adding value in case it isn't NULL*/
    }
    if(local_name == NULL || full_name == NULL){/*Setting blank node, used to
    * set up root node before initialization*/
        new -> path_local_name = NULL;
        new -> path_full_name = NULL;
    }
    else{
        new -> path_local_name = (char*)malloc(strlen(local_name) + 1);
        new -> path_full_name = (char*)malloc(strlen(full_name) + 1);
        strcpy(new -> path_local_name, local_name);
        strcpy(new -> path_full_name, full_name);
    }
    return new;
}

/* Checking if path corresponds to a certain name */
int EQUALpath(path_link path, char*name){
    return !strcmp(path->path->path_full_name, name);
}

/* Adds new value to a node or changes existing value to a new one*/
void ADDvalue(Path_item path, char *new_value){
    if(path->path_value == NULL)
        path->path_value = (char *) malloc(strlen(new_value) + 1);
    else
        path->path_value = (char *) realloc(path->path_value, strlen(new_value) + 1);
    strcpy(path -> path_value, new_value);
}

/*Frees all attributes in of an item*/
void FREEitem(Path_item item){
    if(item->path_value != NULL)
        free(item->path_value);
    free(item->path_full_name);
    free(item->path_local_name);
    free(item);
}

/*Frees a node*/
void FREEpath(path_link Path){
    FREEitem(Path->path);
    free(Path);
}


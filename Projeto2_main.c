/*
 * Files: Projeto2_main.c, Additional_functions_1.c, Additional_functions_2.c
 * Author: Vasco Silva ist199132
 * Description: File system implemented in C
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header_file.h"

/*Input function for double parameter commands*/
void input_double_value(char **value_1,char **value_2){
    int i= 0;
    char *buffer = (char*)malloc_check(MAX_INPUT);/*Allocating buffer to store both
    * parameters*/
    scanf("%[^\n]", buffer);
    while(buffer[i] != ' '){i++;}/*Finding the whitespace that divides
    * both parameters*/
    buffer[i] = '\0';/*Place a null character in place of the whitespace to
    * allow separating parameters for strcpy and strlen*/
    *value_1 = (char*)realloc(*value_1, strlen(buffer)+1);
    strcpy(*value_1, buffer);/*Storing first parameter*/
    *value_2 = (char*)realloc(*value_2, strlen(buffer + i + 1)+1);
    strcpy(*value_2, buffer + i + 1);/*Storing second parameter*/
    free(buffer);
}

/*Input function for single parameter commands*/
void input_single_value(char** value){
    char *buffer = (char*)malloc_check(MAX_INPUT);
    scanf("%[^\n]", buffer);
    *value = (char*)realloc(*value, strlen(buffer)+1);
    strcpy(*value, buffer);
    free(buffer);
}

/*Processes path returning an array of pointers to strings containing
 * each local component of the path*/
char **process_local_path(char *path, int *n_components){
    int i, size = 20;/*Initial size for components array*/
    char *path_temp = (char*)malloc_check(strlen(path)+1);
    char **directories = (char**)malloc_check(sizeof(char*)*size), *directory;
    strcpy(path_temp, path);/*Copying original path to temporary string to
    * avoid modification of the original path*/
    directory = strtok(path_temp, "/");/*Getting the first component*/
    directories[0] = malloc_check(strlen(directory) + 1);
    strcpy(directories[0], directory);
    for(i = 1;(directory = strtok (NULL, "/")) && directory != NULL; i++){
        /*storing each component provided by strtok in the directories array*/
        directories[i] = (char*)malloc_check(strlen(directory) + 1);
        strcpy(directories[i], directory);
        if(i == size-1) {/*Allocating extra memory in case size isn't enough*/
            size *= 2;
            directories = realloc(directories, sizeof(char *) * size);
        }
    }
    directories = realloc(directories, sizeof(char *) * i);
    *n_components = i;/*Storing the number of components obtained*/
    free(path_temp);
    return directories;
}

/* Builds full paths given array of components.
 * Ex: ["usr", "bin", "file"] -> ["/usr", "/usr/bin", "/usr/bin/file"]*/
char **process_full_path(char **local_path, int n_components){
    int i;
    char **directories = (char**)malloc_check(sizeof(char*)*n_components);
    directories[0] = (char*)malloc_check(strlen(local_path[0]) + 2);
    strcpy(directories[0], "/");/*Adding first "/" to the begging of the path*/
    strcat(directories[0], local_path[0]);
    for(i=1; i < n_components; i++){
        /*Concatenating previous full path to new one to allow building full paths*/
        directories[i] = (char*)malloc_check(strlen(directories[i-1]) + strlen(local_path[i]) + 2);
        strcpy(directories[i], directories[i-1]);
        strcat(directories[i], "/");
        strcat(directories[i], local_path[i]);
    }
    return directories;
}

/*Hash function for command input
 * Adds all ascii values in a string and returns the modulus by 100*/
int command_hash(char* word){
    int i, sum = 0;
    for(i = 0; word[i]; i++){
        sum += word[i];
    }
    return sum % 100;
}

/*Help command, prints help message*/
void help_command(){
    printf(HELP_MESSAGE);
}

/*Set command, creates new directories and files adding them to the n-ary tree*/
void set_Command(path_link root_nodes, int *uninit){
    char *name = (char*)malloc_check(1), *value = (char*)malloc_check(1), **path_local_comp, **path_full_comp;
    int i,c,no_args_flag = 0, n_comp;/* Stores the number of components from the input path*/
    Path_item valued_new_path_item;
    path_link current = root_nodes, current_head,current_parent = NULL, valued_new_path;
    input_single_value(&name);
    if((c = getchar()) == '\n')
        no_args_flag = 1;
    else
        input_single_value(&value);/*Getting input parameters*/
    /*Processing input into local and full paths*/
    path_local_comp = process_local_path(name, &n_comp);
    path_full_comp = process_full_path(path_local_comp, n_comp);
    /*Creating new item and new node with value*/
    if(no_args_flag) {
        valued_new_path_item = NEWpath(path_local_comp[n_comp - 1], path_full_comp[n_comp - 1], value);
        valued_new_path = New_path_link(valued_new_path_item, NULL);
    }
    else {
        valued_new_path_item = NEWpath(path_local_comp[n_comp - 1], path_full_comp[n_comp - 1], NULL);
        valued_new_path = New_path_link(valued_new_path_item, NULL);
    }
    /*Initializing root with the first local and global components if uninitialized*/
    if(*uninit) {
        free(root_nodes->path);/*freeing the blank node item of the uninitialized root*/
        initRoot(root_nodes, NEWpath(path_local_comp[0], path_full_comp[0], NULL));
        current = root_nodes;
        *uninit = 0;
    }
    /*Searching for all but the last input components of the input, in case it doesn't exist
     * a new path node is created with NULL value */
    for(i = 0; i < n_comp - 1; i++){
        Path_item new_item = NEWpath(path_local_comp[i], path_full_comp[i], NULL);
        current_head = current;/*Storing the head of the current linked list*/
        current = lookup(current, path_local_comp[i]);
        if(current == NULL){
            path_link new_path = New_path_link(new_item, current_parent);
            if(current_head == NULL) /*If the current head is NULL then the child path
            * of the current parent is defined as the new path*/
                current_parent->child_paths = new_path;
            else /*Else the newly created path is simply inserted in the end*/
                insertEnd(current_head, new_path);
            current = new_path;
        }
        else FREEitem(new_item); /*Freeing new item in case path already exists*/
        current_parent = current;
        current = current->child_paths;
    }
    /* Here we search the actual level where the input path might be located in the tree
     * and either add the new path with value or change an existing one*/
    valued_new_path->parent_path = current_parent;
    if(n_comp > 1 && (current = lookup(current, valued_new_path_item->path_local_name)) == NULL)
        /*Updates the parents child in case the input was not located in the root nodes*/
        current_parent->child_paths = insertEnd(current_parent->child_paths, valued_new_path);
    else if(n_comp == 1 && (current = lookup(current, valued_new_path_item->path_local_name)) == NULL)
        insertEnd(root_nodes, valued_new_path);
    else if(!no_args_flag){ /*In case the path already existed its value is updated*/
        ADDvalue(current->path, value);
        FREEpath(valued_new_path);
    }
    else{
        current->path->path_value = NULL;
    }
    free(name);free(value);
    for(n_comp -= 1;n_comp >= 0;n_comp--){
        free(path_local_comp[n_comp]);free(path_full_comp[n_comp]);
    }
    free(path_local_comp);free(path_full_comp);
}

/*Searches the tree structure for a path by name.
 *Returns the node if it exists or else a NULL pointer*/
path_link path_name_search(char* path_name, path_link root_nodes){
    char **path_components;
    path_link current = root_nodes;
    int i, n_components;/*Number of components in path_name*/
    path_components = process_local_path(path_name, &n_components);/* Separating path_name in components*/
    for(i = 0;i < n_components; i++){
        /*Searching each level of the tree by local component*/
        current = lookup(current, path_components[i]);
        if(current == NULL) {
            while (n_components > 0) { free(path_components[--n_components]); }
            free(path_components);
            return NULL;
        }
        if(i == n_components - 1) {
            while (n_components > 0) {free(path_components[--n_components]);}
            free(path_components);
            return current;
        }
        current = current->child_paths;/*Updating current to search in the child nodes*/
    }
    return NULL;
}

/* Executes the print command, printing each path in depth and in order of creation
 * Goes through the tree structure recursively allowing order of depth*/
void print_command(path_link node){
    while(node != NULL){
        if(node->path->path_value != NULL)/*Checking if the node has a value*/
            printf("%s %s\n", node->path->path_full_name, node->path->path_value);
        if(node->child_paths != NULL)
            print_command(node->child_paths);/*Recursive call to explore child nodes*/
        node = node->next_sibling; /*Exploring the next sibling*/
    }
}

/*Recursively searches the tree structure in order of component creation*/
path_link search_aux(path_link node, char* value){
    path_link tmp;
    for(;node != NULL; node = node->next_sibling){
        if(node->path->path_value != NULL && !strcmp(node->path->path_value, value))
            return node;/*Found node*/
        tmp = search_aux(node->child_paths, value); /*tmp will store the return
        * value of the recursive call*/
        if(tmp != NULL)
            return tmp;
    }
    return NULL;
}

/*Executes search command*/
void search_command(path_link node){
    char *value = malloc_check(1);
    path_link res;
    input_single_value(&value);/*Getting the value parameter input*/
    res = search_aux(node, value);/*Searching the tree with search_aux*/
    if(res == NULL){
        printf(NOT_FOUND);
        return;
    }
    free(value);
    printf("%s\n", res->path->path_full_name);
}

/*Executes find command*/
void find_command(path_link root_nodes){
    char *name = (char*)malloc_check(1);
    path_link result;
    input_single_value(&name);/*Path name input*/
    result = path_name_search(name, root_nodes);/*Searching tree for node by name*/
    if(result == NULL)
        printf(NOT_FOUND);
    else if (result->path->path_value == NULL)
        printf(NO_DATA);/*Node has no value*/
    else
        printf("%s\n", result->path->path_value);
    free(name);
}

/*Copies all full path names from all paths in a linked list,
 * sorts them alphabetically and prints them*/
void name_copy_sort(path_link current){
    int i, j, size = 40;/*Base size for array of path names*/
    char **aux_vector = (char**) malloc_check(sizeof(char *)*size);
    for(i = 0; current != NULL; i++){
        /*copying all full names to an array*/
        aux_vector[i] = (char*)malloc_check(strlen(current->path->path_local_name) + 1);
        strcpy(aux_vector[i], current->path->path_local_name);
        current = current->next_sibling;
        if(i == size-1) {/*Resizing the array if necessary*/
            size *= 2;
            aux_vector = realloc(aux_vector, sizeof(char *) * size);
        }
    }
    aux_vector = realloc(aux_vector, sizeof(char*)*i);/*resizing the array*/
    quicksort(aux_vector, 0, i-1);/*sorting the array alphabetically*/
    for(j = 0; j < i; j++)
        printf("%s\n", aux_vector[j]);
    while(i > 0){free(aux_vector[--i]);}
    free(aux_vector);
}

/*Executes list command*/
void list_command(int no_args, path_link root_nodes){
    path_link current, directory;
    char *name = (char*)malloc_check(1);
    if(no_args)/*Sets directory for listing to first root path in case of no args*/
        directory = root_nodes;
    else{
        input_single_value(&name);/*Path name input*/
        directory = path_name_search(name, root_nodes);/*Searching for path*/
    }
    free(name);
    if(directory == NULL){
        printf(NOT_FOUND);/*Directory was not found*/
        return;
    }
    if(no_args)
        current = directory;
    else
        current = directory->child_paths;/*setting current to the first sub-directory*/
    name_copy_sort(current);/*Sorts and prints all sub-directory names*/
}

/*Finds head in of a sub-path list given all full components list of sub-paths*/
path_link get_head(path_link root_nodes, char** path_names, int n_comp){
    path_link parent;
    if(n_comp == 1)
        return root_nodes;
    /*Searching for parent in order to get first child node which is the lists head*/
    parent = path_name_search(path_names[n_comp-2], root_nodes);
    if(parent == NULL)
        return NULL;
    return parent->child_paths;
}

/*Recursively frees all subjacent nodes to a certain node */
void free_subjacent_nodes(path_link node){
    path_link prev;
    while(node != NULL){
        if(node->child_paths != NULL)/*Recursive call to free all child nodes*/
            free_subjacent_nodes(node->child_paths);
        prev = node;/*storing current node in prev to allow node to be freed after moving to next node*/
        node = node->next_sibling;
        FREEpath(prev);
    }
}

/*Deletes node from linked list*/
path_link delete_operation(char* input, path_link head){
    path_link curr = head, prev = NULL, parent;
    if(head == NULL) {
        printf(NOT_FOUND);
        return NULL;
    }
    parent = head->parent_path;
    while(curr != NULL && !EQUALpath(curr, input)){/*searching for path in list*/
        prev = curr;
        curr = curr->next_sibling;
    }
    if(curr != NULL){/*Removing path from list and freeing the node*/
        if(prev != NULL)
            prev->next_sibling = curr->next_sibling;
        else head = curr->next_sibling;
        free_subjacent_nodes(curr->child_paths);
        FREEpath(curr);
    }
    else{/*Path was not found*/
        printf(NOT_FOUND);
        return NULL;
    }
    if(parent != NULL)
        parent->child_paths = head;/*Assigning new head to parent node child in case
        * the previous head node was deleted*/
    return head;
}

/*Deletes all root nodes therefore all created paths*/
void delete_root(path_link root_node, int *uninit){
    path_link curr = root_node, prev = NULL;
    while(curr != NULL){
        prev = curr;
        curr = curr->next_sibling;
        free_subjacent_nodes(prev->child_paths);
        FREEpath(prev);
    }
    *uninit = 1;/*Setting uninitialized flag to 1 indicating the root node hasn't been initialized*/
}

/*Executes delete command*/
path_link delete_command(path_link root_nodes, int *uninit, int no_args){
    path_link head;
    char* input_Path = (char*)malloc_check(1), **full_path_names, **local_path_names;
    int n_comp, i;
    if(no_args){/*If delete is called without any parameters the whole file system is deleted*/
        free(input_Path);
        delete_root(root_nodes, uninit);
        /*Returning uninitialized root node path*/
        return New_path_link(NEWpath(NULL, NULL, NULL), NULL);
    }
    input_single_value(&input_Path);/*Input path*/
    /*Extracting local and full path names from input*/
    local_path_names = process_local_path(input_Path, &n_comp);
    full_path_names = process_full_path(local_path_names, n_comp);
    free(input_Path);
    input_Path = full_path_names[n_comp-1];
    head = get_head(root_nodes, full_path_names, n_comp);/*Getting head node
    * from the list where the deletion will happen*/
    head = delete_operation(input_Path, head);/*deleting node*/
    for(i = 0;i < n_comp;i++){/*freeing processed input arrays*/
        free(local_path_names[i]);free(full_path_names[i]);
    }
    free(local_path_names);free(full_path_names);
    if(n_comp == 1 && head != NULL)/*Case where the first root node is deleted where
    * a new root node will have to be assigned*/
        return head;
    return root_nodes;
}

/*Executes quit command to terminate the program*/
void quit_command(int *running, path_link root_nodes, int *uninit){
    *running = 0;
    if(!(*uninit))
        delete_root(root_nodes, uninit);/*Freeing the whole file system
        * by deleting all root nodes*/
    else {/*Root node is uninitialized*/
        free(root_nodes->path);/*freeing NULL values path item*/
        free(root_nodes);
    }
}

/*Prints out errors or nothing depending on what command was called if the
 * root node isn't initialized yet*/
void uninit_errors(int hash, int no_args){
    char *buffer;
    if(hash != HASH_PRINT){
        if(!no_args) {
            buffer = (char *) malloc_check(MAX_INPUT);
            /*reading and discarding commands parameters*/
            scanf("%[^\n]", buffer);
            free(buffer);
            printf(NOT_FOUND);
        }
    }
}

int main(){
    char *command = (char*)malloc_check(7);
    /* no_args: indicates whether command was called with or without arguments
     * running: indicates whether program is still running(only changed by quit command)
     * uninitialized: indicates whether the root node has been initialized or not*/
    int hashed_command, args, running = 1, no_args = 0, uninitialized = 1;
    /*Setting root node to a NULL valued path*/
    path_link root_nodes = New_path_link(NEWpath(NULL, NULL, NULL), NULL);
    while(running){
        scanf("%s", command);/*Reading command*/
        args = getchar();/*Reading character after command to check for parameters*/
        if(args == '\n')/*Args being \n indicates no parameters were inputted*/
            no_args = 1;
        hashed_command = command_hash(command);/*Hashing input fore the switch case comparisons*/
        if(uninitialized && hashed_command != HASH_QUIT && hashed_command != HASH_SET && hashed_command != HASH_HELP){
            /*Calling uninit_errors in case root_node isn't initialized*/
            uninit_errors(hashed_command, no_args);
            no_args = 0;
            continue;
        }
        switch(hashed_command){/*Checks for what command was passed as input*/
            case HASH_QUIT:
                quit_command(&running, root_nodes, &uninitialized);
                break;
            case HASH_HELP:
                help_command();
                break;
            case HASH_SET:
                set_Command(root_nodes, &uninitialized);
                break;
            case HASH_FIND:
                find_command(root_nodes);
                break;
            case HASH_LIST:
                list_command(no_args, root_nodes);
                break;
            case HASH_PRINT:
                print_command(root_nodes);
                break;
            case HASH_DELETE:
                root_nodes = delete_command(root_nodes, &uninitialized, no_args);
                break;
            case HASH_SEARCH:
                search_command(root_nodes);
                break;
            default:
                printf("Unknown command\n");
        }
        no_args = 0;/*Resetting no_args*/
    }
    free(command);
    return 0;
}



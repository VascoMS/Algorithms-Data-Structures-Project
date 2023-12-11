#include "Header_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define less(A, B) (strcmp(A, B) < 0)
#define exch(A, B) {char *t = A; A = B; B = t;}

/*malloc wrapper function, enables checking if no more memory is available*/
void* malloc_check(unsigned size){
    void *new = malloc(size);
    if(new == NULL) {
        printf("No memory\n.");
        exit(0);
    }
    return new;
}


/*Creates a new node and returns a pointer to that node*/
path_link New_path_link(Path_item new_path, path_link parent){
    path_link new = (path_link) malloc(sizeof(struct path_node));
    new->path = new_path;
    new->parent_path = parent;
    new->next_sibling = NULL;
    new->child_paths = NULL;
    return new;
}

/* Initializes root with a path item*/
void initRoot(path_link root, Path_item new_path){
    root->path = new_path;
    root->next_sibling = NULL;
    root->child_paths = NULL;
}

/*Inserts an element to the end of a linked list, used to append a new file to
 * a directory*/
path_link insertEnd(path_link head, path_link new_path){
    path_link current;
    if(head == NULL)
        return new_path;
    for(current = head; current -> next_sibling != NULL; current = current->next_sibling)
        ;
    current->next_sibling = new_path;
    return head;
}

/*Searches for an element in a linked list, returns NULL if not found*/
path_link lookup(path_link head, char *name){
    path_link current;
    for(current = head; current != NULL; current = current -> next_sibling){
        if(!strcmp(name, current->path->path_local_name))
            return current;
    }
    return NULL;
}

/*Partition function for quicksort algorithm*/
int partition(char *a[], int l, int r) {
    int i = l - 1;
    int j = r;
    char *v = a[r];
    while (i < j) {
        while (less(a[++i], v));
        while (less(v, a[--j]))
            if (j == l)
                break;
        if (i < j)
            exch(a[i], a[j]);
    }
    exch(a[i], a[r]);
    return i;
}

/*Quicksort algorithm*/
void quicksort(char *a[], int l, int r)
{
    int i;
    if (r <= l)
        return;
    i = partition(a, l, r);
    quicksort(a, l, i-1);
    quicksort(a, i+1, r);
}

#ifndef PATH_H
#define PATH_H
/*Hash value of every command using the hash_command function*/
#define HASH_HELP 25
#define HASH_QUIT 51
#define HASH_SET 32
#define HASH_PRINT 57
#define HASH_FIND 17
#define HASH_LIST 44
#define HASH_SEARCH 30
#define HASH_DELETE 27

#define MAX_INPUT 65535/*Maximum input size*/

/*Error templates*/
#define NOT_FOUND "not found\n"
#define NO_DATA "no data\n"

/*Help message*/
#define HELP_MESSAGE "help: Imprime os comandos disponíveis.\n"\
"quit: Termina o programa.\n"\
"set: Adiciona ou modifica o valor a armazenar.\n"\
"print: Imprime todos os caminhos e valores.\n"\
"find: Imprime o valor armazenado.\n"\
"list: Lista todos os componentes imediatos de um sub-caminho.\n"\
"search: Procura o caminho dado um valor.\n"\
"delete: Apaga um caminho e todos os subcaminhos.\n"

/*Item structure, contains local name, full name and value attributes of a path.
 *An items local name is the a files name inside a directory while its full name
 *is its name in a global context of the whole file system*/
struct path_item {
    char *path_local_name, *path_full_name, *path_value;
};

typedef struct path_item *Path_item;

/*Node structure, contains an instance of the item structure and 3 pointers that
 * allow linking between nodes in an n-ary tree*/
struct path_node {
    Path_item path;
    struct path_node *parent_path;
    struct path_node *child_paths;
    struct path_node *next_sibling;
};

typedef struct path_node *path_link;

Path_item NEWpath(char *local_name,char *full_name, char *valor);
int EQUALpath(path_link path, char*name);
void ADDvalue(Path_item path, char *new_value);
void FREEpath(path_link path);
void FREEitem(Path_item item);
path_link insertEnd(path_link head, path_link new_path);
path_link New_path_link(Path_item new_path, path_link parent);
path_link lookup(path_link head, char* name);
void* malloc_check(unsigned size);
int partition(char *a[], int l, int r);
void quicksort(char *a[], int l, int r);
void initRoot(path_link root, Path_item new_path);
#endif


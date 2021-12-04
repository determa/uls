#ifndef HEADER_H
#define HEADER_H
#define INT_MIN -2147483648
#define INT_MAX +2147483647

#include "../libmx/inc/libmx.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include <sys/acl.h>

#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

typedef struct p_list {
    char *name;
    struct p_list *next;
}               direct;

typedef struct n_list {
    struct dirent *entry;
    struct stat statbuf;
    struct group *group;
    char *directory;
    struct n_list *next;
    struct n_list *prev;
}           files;

typedef struct e_list {
    int argc;
    char **argv;
    bool flags[16];
    bool directory;
    int winsize;
    int count_files;
    int count_direct;
}           arguments;

direct *create_node_dir(char *name);
void push_back_dir(direct **list, char *name);
void clear_list_dir(direct **list);
void pop_back(direct **head);

files *create_node(struct dirent *entry, struct stat statbuf, char *directory, struct group *group);
void node_push_back(files **list, struct dirent *entry, struct stat statbuf, char *directory, struct group *group);
void clear_list_files(files **list);
void sort_list(files *lst, arguments *uls, bool is_file);
files *find_node(files *list, int index);

void define_flags(arguments *uls);
void argv_files(arguments *uls);
files *fill_list(arguments *uls, char *directory, files *lil, bool delete);
void print_list(arguments *uls, files *list, bool is_file);

bool is_dir(char *dir);

#endif


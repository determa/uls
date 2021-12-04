#include "../inc/header.h"

files *create_node(struct dirent *entry, struct stat statbuf, char *directory, struct group *group) {
    files *node = malloc(sizeof(files));
    node->entry = entry;
    node->statbuf = statbuf;
    node->directory = directory;
    node->group = group;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void node_push_back(files **list, struct dirent *entry, struct stat statbuf, char *directory, struct group *group) {
    // printf("%s ", entry->d_name);
    if (!*list) {
        *list = create_node(entry, statbuf, directory, group);
        return;
    }
    files *temp = *list;
    while (temp->next) {
        temp = temp->next;
    }
    files *prev_node = temp;
    temp->next = create_node(entry, statbuf, directory, group);
    temp = temp->next;
    temp->prev = prev_node;
}

void clear_list_files(files **list) {
    if (!*list) return; 
    files *temp;
    while (*list) {
        temp = (*list)->next;
        (*list)->next = NULL;
        free(*list);
        *list = temp;
    }
    list = NULL;
}


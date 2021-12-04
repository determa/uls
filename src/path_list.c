#include "../inc/header.h"

direct *create_node_dir(char *name) {
    direct *new_node = NULL;
    new_node = (direct *)malloc(sizeof(direct));
    if (!new_node)
        return NULL;
    new_node->name = name;
    new_node->next = NULL;
    return new_node;
}

void push_back_dir(direct **list, char *name) {
    if (!*list) {
        *list = create_node_dir(name);
        return;
    }
    direct *temp = *list;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = create_node_dir(name);
}

void pop_back(direct **head) {
    direct *temp = NULL;

    if (head == NULL || *head == NULL)
        return;
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
    }
    else {
        temp = *head;
        while (temp && temp->next->next != NULL)
            temp = temp->next;
        free(temp->next);
        temp->next = NULL;
    }
}

void clear_list_dir(direct **list) {
    if (!*list) return; 
    direct *temp;
    while (*list) {
        temp = (*list)->next;
        (*list)->next = NULL;
        free(*list);
        *list = temp;
    }
    list = NULL;
}


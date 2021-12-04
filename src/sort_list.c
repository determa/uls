#include "../inc/header.h"

static void swap(files *s1, files *s2) {
    struct dirent *entry = s1->entry;
    struct stat statbuf = s1->statbuf;
    struct group *group = s1->group;
    char *directory = s1->directory;

    s1->directory = s2->directory;
    s1->entry = s2->entry;
    s1->statbuf = s2->statbuf;
    s1->group = s2->group;

    s2->directory = directory;
    s2->entry = entry;
    s2->statbuf = statbuf;
    s2->group = group;
}

void sort_list(files *lst, arguments *uls, bool is_file) {
    for (files *temp1 = lst; temp1; temp1 = temp1->next) {
        for (files *temp2 = lst; temp2->next; temp2 = temp2->next) {
            if (uls->flags[12] == true) {
                if (temp2->statbuf.st_mtime < temp2->next->statbuf.st_mtime) {
                    swap(temp2, temp2->next);
                }
            }
            else {
                if (is_file) {
                    if (mx_strcmp(temp2->directory, temp2->next->directory) > 0)
                        swap(temp2, temp2->next);  
                }
                else {
                    if (mx_strcmp(temp2->entry->d_name, temp2->next->entry->d_name) > 0)
                        swap(temp2, temp2->next);  
                }
            }
            if (uls->flags[11] == true) {
                if (mx_strcmp(temp2->entry->d_name, temp2->next->entry->d_name) < 0) {
                    swap(temp2, temp2->next);
                }
            }
        }
    }
}


#include "../inc/header.h"

static void swap(direct *s1, direct *s2) {
    char *temp = s1->name;
    s1->name = s2->name;
    s2->name = temp;
}

void argv_files(arguments *uls) {
    direct *fil = NULL;
    bool is_print = true;
    int count = 0;
    int dires = 0;
    for (int i = 1; i < uls->argc; i++) {
        if (uls->argv[i][0] != '-') {
            if (!is_dir(uls->argv[i])) {
                push_back_dir(&fil, uls->argv[i]);
                count++;
            }
            if (is_dir(uls->argv[i]))
                dires++;
        }
    }
    uls->count_direct = dires;
    uls->count_files = count;
    if (fil) {
        direct *temp = fil;
        files *list = NULL;
        while (temp) {
            list = fill_list(uls, temp->name, list, is_print);
            temp = temp->next;
        }
        print_list(uls, list, is_print);
        clear_list_files(&list);
    }
    clear_list_dir(&fil);
}

void sort(direct *lst, arguments *uls) {
    uls->argv[1] = uls->argv[1];
    for (direct *temp1 = lst; temp1; temp1 = temp1->next) {
        for (direct *temp2 = lst; temp2->next; temp2 = temp2->next) {
            if (uls->flags[11] == true) {
                if (mx_strcmp(temp2->name, temp2->next->name) < 0) {
                    swap(temp2, temp2->next);
                }
            } else {
                if (mx_strcmp(temp2->name, temp2->next->name) > 0) {
                    swap(temp2, temp2->next);
                }
            }
            
        }
    }
}

files *fill_list(arguments *uls, char *directory, files *lil, bool delete) {
    direct *names = NULL;
    bool recursion = false;
    bool file = false;
    files *list = NULL;
    if (lil != NULL) {
        list = lil;
    }
    DIR *dh;
    struct group *group;
    struct stat statbuf; // для информации о файле
	struct dirent *entry; // для имени и индекса файла
    int index_for_dir = 0;

    dh = opendir(directory); // поток каталога
    if (!dh) { 
        if (errno == ENOENT) { // проверка на файл или директорию
            mx_printerr("uls: ");
            mx_printerr(directory);
            mx_printerr(": No such file or directory\n");
            return list;
        }
        if (errno == EACCES) { // проверка доступ
            mx_printerr("uls: ");
            mx_printerr(directory);
            mx_printerr(": Permission denied\n");
            return list;
        }
    }
    if (dh == NULL) {
        file = true;
        for (int j = mx_strlen(directory) - 1; j >= 0; j--) {
            if (directory[j] == '/') {
                index_for_dir = j + 1;
                break;
            }
        }
        if (index_for_dir == 0) {
            dh = opendir(".");
        }
        else {
            char *direr = NULL;
            direr = mx_strndup(directory, mx_strlen(directory) - mx_strlen(&directory[index_for_dir]));
            dh = opendir(direr);
            mx_strdel(&direr);
        }
    }
    while ((entry = readdir(dh)) != NULL) {
        if (file) {
            lstat(directory, &statbuf);
        }
        else {
            lstat(mx_strjoin(directory, mx_strjoin("/", entry->d_name)), &statbuf); // считываем информацию о файле в структуру
        }
        group = getgrgid(statbuf.st_gid);
        if (file != true || mx_strcmp(&directory[index_for_dir], entry->d_name) == 0) {
            if (uls->flags[2] == true) {
                if (file != true && mx_strcmp(entry->d_name, ".") != 0 
                    && mx_strcmp(entry->d_name, "..") != 0) {
                    if (is_dir(mx_strjoin(directory, mx_strjoin("/", entry->d_name)))) {
                        if (entry->d_name[0] != '.' || uls->flags[3] == true || uls->flags[4] == true) {
                            if (!(statbuf.st_mode & S_IFLNK)) {
                                push_back_dir(&names, entry->d_name);
                            }
                        }
                        recursion = true;
                    }
                }
            }
            if (file) {
                node_push_back(&list, entry, statbuf, directory, group);
            }
            else {
                node_push_back(&list, entry, statbuf, mx_strjoin(directory, mx_strjoin("/", entry->d_name)), group);
            }
        }
    }
    if (!delete) {
        print_list(uls, list, delete);
        clear_list_files(&list);
    }
    if (recursion == true) {
        char *dir = mx_strdup(directory);
        sort(names, uls);
        while (names) {
            mx_printstr("\n");
            mx_printstr(mx_strjoin(dir, mx_strjoin("/", names->name)));
            mx_printstr(":\n");
            fill_list(uls, mx_strjoin(dir, mx_strjoin("/", names->name)), NULL, delete);
            names = names->next;
        }
        recursion = false;
        mx_strdel(&dir);
    }
    file = false;
    closedir(dh);
    clear_list_dir(&names);
    return list;
}


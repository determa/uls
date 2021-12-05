#include "../inc/header.h"

void mx_print_space(int len, int max_len) {
    while (len <= max_len) {
        mx_printstr(" ");
        len++;
    }
}

char* permissions(mode_t perm){ // доступ в формате "rwxr-r -"
    char *modeval = malloc(sizeof(char) * 10 + 1);
    modeval[0] = (perm & S_IFDIR) ? 'd' : '-';
    modeval[1] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[2] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[3] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[4] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[5] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[6] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[7] = (perm & S_IROTH) ? 'r' : '-';
    modeval[8] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[9] = (perm & S_IXOTH) ? 'x' : '-';
    if (perm & S_ISVTX) 
        modeval[9]= 't';
    if(S_ISLNK(perm))
        modeval[0]= 'l';
    modeval[10] = '\0';
    return modeval;
}

void print_list(arguments *uls, files *list, bool is_file) {
    if (!list) {
        return;
    }
    if (uls->flags[4] == true && !is_file) { // -A
        list = list->next;
        list = list->next;
        uls->flags[3] = true;
    }
    while (uls->flags[3] == false && list && list->entry->d_name[0] == '.' && !is_file) {
        list = list->next;
    }
    if (!list) {
        return;
    }
    sort_list(list, uls, is_file);
    acl_t acl = NULL;
    files temp = *list;
    files *tmp_head = list;
    files *tmp = NULL;
    
    int t = 0; // temp номер столбца
    int ind = 0; // temp номер строки
    int total = 0;
    int max_len = 0;
    int l_perm = 0;
    int l_st_size = 0;
    int count_dname = 0;
    int count_tab_col = 1;
    bool xattr_print = false;
    bool acl_print = false;
    char *time_temp = NULL;
    char *time_lol = NULL;
    int l_pw_name = 0;
    int l_gr_name = 0;
    char *c = NULL;
    char *c2 = NULL;
    struct passwd *pwd;

    while (list) {
        // if (uls->flags[3] == false && list->entry->d_name[0] == '.') { //-a
        //     list = list->next;
        //     continue;
        // }
        count_dname++;
        total += list->statbuf.st_blocks;
        max_len = max_len > mx_strlen(list->entry->d_name) ? max_len : mx_strlen(list->entry->d_name);
        l_perm = l_perm > list->statbuf.st_nlink ? l_perm : list->statbuf.st_nlink;
        l_st_size = l_st_size > list->statbuf.st_size ? l_st_size : list->statbuf.st_size;
        pwd = getpwuid(list->statbuf.st_uid);
        l_pw_name = l_pw_name > mx_strlen(pwd->pw_name) ? l_pw_name : mx_strlen(pwd->pw_name);
        if ((getgrgid(list->statbuf.st_gid)) == NULL)
            l_gr_name = l_gr_name > mx_strlen(mx_itoa(list->statbuf.st_gid)) ? l_gr_name : mx_strlen(mx_itoa(list->statbuf.st_gid));
        else
            l_gr_name = l_gr_name > mx_strlen(list->group->gr_name) ? l_gr_name : mx_strlen(list->group->gr_name);
        list = list->next;
    }
    list = &temp;

    if(uls->flags[1] == true && !is_file) { // если у нас -l
        mx_printstr("total ");
        mx_printint(total);
        mx_printstr("\n");
    }
    
    for (int i = 8; i <= max_len; i += 8) {
        count_tab_col++;
    }

    int count_rows = uls->winsize / (count_tab_col * 8);
    int count_cols = 0;
    if (count_rows != 0)
        count_cols = count_dname % count_rows != 0 ? count_dname/count_rows + 1 : count_dname/count_rows;

    if (uls->flags[9] == true) {
        count_cols = 0;
        count_rows = 0;
    }
    
    while (list) {
        time_temp = 4 + ctime((time_t *)&list->statbuf.st_mtime);
        
        if (time(NULL) - list->statbuf.st_mtime > 15811200) {
            time_lol = mx_strndup(time_temp, mx_strlen(time_temp) - 14);
            time_lol = mx_strjoin(time_lol, mx_strndup(19 + ctime((time_t *)&list->statbuf.st_mtime), 5));
        } 
        else {
            time_lol = mx_strndup(time_temp, mx_strlen(time_temp) - 9);
        }
        // if (uls->flags[3] == false && list->entry->d_name[0] == '.') {
        //     list = list->next;
        //     tmp_head = tmp_head->next;
        //     continue;
        // } // если у нас не -а, то не печатаем скрытые файлы
        t++;

        // вывод размера и название файлов/папок
        if(uls->flags[1] == true) { // если у нас -l
            mx_printstr(permissions(list->statbuf.st_mode));
            c = mx_strnew(1024);
            if (listxattr(list->directory, c, 1024, XATTR_NOFOLLOW) > 0) {
                mx_printchar('@');
                xattr_print = true;
            }
            else if ((acl = acl_get_file(list->directory, ACL_TYPE_EXTENDED))) {
                acl_free(acl);
                mx_printchar('+');
                acl_print = true;
            }
            else
                mx_printchar(' ');
            mx_print_space(mx_strlen(mx_itoa(list->statbuf.st_nlink)), mx_strlen(mx_itoa(l_perm)));
            mx_printint(list->statbuf.st_nlink);
            mx_printstr(" ");
            pwd = getpwuid(list->statbuf.st_uid);
            mx_printstr(pwd->pw_name);
            mx_print_space(mx_strlen(pwd->pw_name), l_pw_name);
            
            mx_printstr(" ");
            if ((getgrgid(list->statbuf.st_gid)) == NULL) {
                mx_printint(list->statbuf.st_gid);
                mx_print_space(mx_strlen(mx_itoa(list->statbuf.st_gid)), l_gr_name);
            }
            else {
                mx_printstr(list->group->gr_name);
                mx_print_space(mx_strlen(list->group->gr_name), l_gr_name);
            }
            mx_print_space(mx_strlen(mx_itoa(list->statbuf.st_size)), mx_strlen(mx_itoa(l_st_size)));
            mx_printint(list->statbuf.st_size);
            mx_printstr(" ");
            mx_printstr(time_lol);
            mx_printstr(" ");
            if (is_file) {
                mx_printstr(list->directory);
            }
            else {
                mx_printstr(list->entry->d_name);
            }
            
            c2 = mx_strnew(1024);
            if (S_ISLNK(list->statbuf.st_mode)) {
                readlink(list->directory, c2, 1024);
                mx_printstr(" -> ");
                mx_printstr(c2);
            }
            mx_printstr("\n");
            if (xattr_print == true && uls->flags[6] == true) {
                mx_printstr("\t");
                mx_printstr(c);
                mx_printstr("       ");
                mx_printint(getxattr(list->directory, c, NULL, 1024, 0, 0));
                mx_printstr("\n");
                xattr_print = false;
            }
            if (acl_print == true && uls->flags[7] == true) {
                mx_printstr("\t");
                if (list->group->gr_gid)
                    mx_printint(list->group->gr_gid);
                mx_printstr(": ");
                if (list->group->gr_name)
                    mx_printstr(list->group->gr_name);
                mx_printstr("\n");
                acl_print = false;
            }
            mx_strdel(&c2);
            mx_strdel(&c);
        }
        else {
            
            if (count_cols * t + ind - count_cols >= count_dname) {
                t = 1;
                mx_printstr("\n");
                ind++;
            }
            tmp = find_node(tmp_head, count_cols * t + ind - count_cols);
            if (is_file) {
                mx_printstr(list->directory);
            }
            else {
                mx_printstr(tmp->entry->d_name);
            }
            if (t >= count_rows) {
                mx_printstr("\n");
                ind++;
                t = 0;
            } else {
                if (t < count_dname && list->next) {
                    for (int i = tmp->entry->d_namlen; i < count_tab_col * 8; i += 8) 
                        mx_printstr("\t");
                } else {
                    mx_printstr("\n");
                }
            }
        }
        list = list->next;
        mx_strdel(&time_lol);
    }
}


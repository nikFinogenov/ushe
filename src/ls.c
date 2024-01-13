#include "ush.h"

void switch_strcolor(struct stat sb) {
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFCHR:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFDIR:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFIFO:
        mx_printstr(YELLOW_COLOR);
        break;

    case S_IFLNK:
        mx_printstr(MAGENTA_COLOR);
        break;

    case S_IFSOCK:
        mx_printstr(GREEN_COLOR);
        break;

    default:
        if (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            mx_printstr(RED_COLOR);
        }
        break;
    }
}

double custom_round(double value) {
    double result = value + 0.5;
    return (long)result;
}

int compare_names(const void *a, const void *b, bool rev) {
    if (!rev)
        return mx_strcmp(*(const char **)a, *(const char **)b);
    return mx_strcmp(*(const char **)b, *(const char **)a);
}

int compare_file_entries_name(const void *a, const void *b, bool rev) {
    if (!rev)
        return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
    return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
}

int compare_file_entries_date_time(const void *a, const void *b, bool rev) {
    if (!rev) {
        if (((t_file_entry_s *)b)->cmptime.tv_sec - ((t_file_entry_s *)a)->cmptime.tv_sec == 0) {
            if (((t_file_entry_s *)b)->cmptime.tv_nsec - ((t_file_entry_s *)a)->cmptime.tv_nsec == 0)
                return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
            return ((t_file_entry_s *)b)->cmptime.tv_nsec - ((t_file_entry_s *)a)->cmptime.tv_nsec;
        }
        return ((t_file_entry_s *)b)->cmptime.tv_sec - ((t_file_entry_s *)a)->cmptime.tv_sec;
    }
    else {
        if (((t_file_entry_s *)a)->cmptime.tv_sec - ((t_file_entry_s *)b)->cmptime.tv_sec == 0) {
            if (((t_file_entry_s *)a)->cmptime.tv_nsec - ((t_file_entry_s *)b)->cmptime.tv_nsec == 0)
                return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
            return ((t_file_entry_s *)a)->cmptime.tv_nsec - ((t_file_entry_s *)b)->cmptime.tv_nsec;
        }
        return ((t_file_entry_s *)a)->cmptime.tv_sec - ((t_file_entry_s *)b)->cmptime.tv_sec;
    }
}

int compare_file_entries_size(const void *a, const void *b, bool rev) {
    if (!rev) {
        if (((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size == 0)
            return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
        return ((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size;
    }
    else if (((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size == 0)
        return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
    return ((t_file_entry_s *)a)->size - ((t_file_entry_s *)b)->size;
}

bool is_dir(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_executable(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_fifo(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISFIFO(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_link(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_socket(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISSOCK(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_whiteout(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISCHR(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_smth(const char *filename) {
    return is_dir(filename) || is_executable(filename) || is_fifo(filename) || is_link(filename) || is_socket(filename) || is_whiteout(filename);
}

bool is_smth_except_dir(const char *filename) {
    return is_executable(filename) || is_fifo(filename) || is_link(filename) || is_socket(filename) || is_whiteout(filename);
}


void printint_formatted(int n, int width) {
    int num_width = 0;
    int temp = n;

    while (temp != 0) {
        temp /= 10;
        num_width++;
    }

    for (int i = 0; i < width - num_width; i++)
        mx_printchar(' ');

    mx_printint(n);
}

void printstr_formatted(char *str, int wid, bool right) {
    if (!right)
        mx_printstr(str);
    for (int i = 0; i < wid - mx_strlen(str); i++)
        mx_printchar(' ');
    if (right)
        mx_printstr(str);
}

void print_multicolumn(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }

    int max_name_length = 0;
    for (int i = 0; i < count; ++i) {
        int name_length = mx_strlen(file_entries[i].name);
        if (flags->i)
            name_length += 9;
        if (flags->p)
            name_length += 1;
        if (name_length > max_name_length) {
            max_name_length = name_length;
        }
    }
    int tab = 8;
    if (flags->G)
        tab = 1;
    int width = (max_name_length + tab) & ~(tab - 1);
    int num_columns = terminal_width / width;
    int index = 0;
    int rows = (count + num_columns - 1) / num_columns;
    for (int i = 0; i < rows; ++i) {
        if (!flags->x)
            index = i;
        for (int j = 0; j < num_columns; j++) {
            if (flags->i) {
                printstr_formatted(mx_ultoa(file_entries[index].inode), 8, true);
                mx_printchar(' ');
            }
            if (flags->G) {
                struct stat sb;

                if (lstat(file_entries[index].path, &sb) == -1) continue;
                switch_strcolor(sb);
            }
            mx_printstr(file_entries[index].name);
            if (flags->G)
                mx_printstr(DEFAULT_COLOR);
            int name_len = mx_strlen(file_entries[index].name);
            if (flags->i)
                name_len += 9;
            if (flags->p && is_dir(file_entries[index].path)) {
                mx_printchar('/');
                name_len += 1;
            }
            else if (flags->F && is_executable(file_entries[index].path)) {
                mx_printchar('*');
                name_len += 1;
            }
            else if (flags->F && is_link(file_entries[index].path)) {
                mx_printchar('@');
                name_len += 1;
            }
            else if (flags->F && is_socket(file_entries[index].path)) {
                mx_printchar('=');
                name_len += 1;
            }
            else if (flags->F && is_whiteout(file_entries[index].path)) {
                mx_printchar('%');
                name_len += 1;
            }
            else if (flags->F && is_fifo(file_entries[index].path)) {
                mx_printchar('|');
                name_len += 1;
            }

            if (!flags->x)
                index = index + rows;
            else
                index++;
            if (index >= count)
                break;
            int tabs = (width - name_len + tab - 1) / tab;
            if ((j == num_columns - 1) && flags->x)
                continue;
            for (int k = 0; k < tabs; k++) {
                if (tab == 1)
                    mx_printchar(' ');
                else
                    mx_printchar('\t');
            }
        }
        mx_printchar('\n');
    }
}

void print_xattr(const t_file_entry_s *file_entry, t_flags_s *flags) {
    for (char **ptr = file_entry->xattr_keys; *ptr != NULL; ptr++) {
        mx_printchar('\t');
        mx_printstr(*ptr);
        mx_printchar('\t');
        ssize_t value_size = getxattr(file_entry->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);

        if (flags->h) {
            const char *suffixes[] = {"B", "K", "M", "G", "T", "P"};
            double size_f = (double)value_size;
            int i = 0;

            while (size_f >= 1000) {
                size_f /= 1024;
                i++;
            }

            double size_r = custom_round(size_f * 10) / 10;
            char *result = (char *)malloc(20);

            if (size_r >= 10 || i == 0) {
                char *str = mx_lltoa(custom_round(size_f));
                result = mx_strjoin(result, str);
                free(str);
            }
            else {
                char *str = mx_lltoa(size_r);
                result = mx_strjoin(result, str);
                free(str);
                result = mx_strjoin(result, ".");
                str = mx_lltoa((long long)(size_r * 10) % 10);
                result = mx_strjoin(result, str);
                free(str);
            }

            result = mx_strjoin(result, suffixes[i]);
            printstr_formatted(result, 5, true);
        }
        else {
            printint_formatted(value_size, 5);
        }

        mx_printstr(" \n");
    }
}

void print_perline(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    for (int i = 0; i < count; i++) {
        if (flags->i) {
            printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
            mx_printchar(' ');
        }
        if (flags->G) {
            struct stat sb;
            if (lstat(file_entries[i].path, &sb) == -1) continue;
            switch_strcolor(sb);
        }
        mx_printstr(file_entries[i].name);
        if (flags->G)
            mx_printstr(DEFAULT_COLOR);
        if (flags->p && is_dir(file_entries[i].path))
            mx_printchar('/');
        else if (flags->F && is_executable(file_entries[i].path)) {
            mx_printchar('*');
        }
        else if (flags->F && is_link(file_entries[i].path)) {
            mx_printchar('@');
        }
        else if (flags->F && is_socket(file_entries[i].path)) {
            mx_printchar('=');
        }
        else if (flags->F && is_whiteout(file_entries[i].path)) {
            mx_printchar('%');
        }
        else if (flags->F && is_fifo(file_entries[i].path)) {
            mx_printchar('|');
        }
        mx_printchar('\n');
    }
}

void print_file_entry_s(const t_file_entry_s *file_entries, int i, t_max_sizes_s mxsize, t_flags_s *flags) {
    if (flags->i) {
        printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
        mx_printchar(' ');
    }
    mx_printchar(file_entries[i].type);
    for (int j = 0; j < mx_strlen(file_entries[i].permissions); j++) {
        mx_printchar(file_entries[i].permissions[j]);
    }
    mx_printchar(' ');

    if (file_entries[i].nlinks) {
        printint_formatted(file_entries[i].nlinks, mxsize.max_nlinks_len);
    }
    else {
        mx_printstr("  ");
    }

    if (!flags->g) {
        mx_printchar(' ');
        printstr_formatted(mx_char_to_str(file_entries[i].owner), mxsize.max_username_len, true);
    }
    else
        mx_printchar(' ');
    if (!flags->g && !flags->o)
        mx_printstr("  ");
    if (!flags->o) {
        printstr_formatted(mx_char_to_str(file_entries[i].group), mxsize.max_groupname_len, true);
    }
    if (flags->h) {
        mx_printstr("   ");
        if (mxsize.max_h_size_len == 2)
            mx_printstr("  ");
        printstr_formatted(mx_char_to_str(file_entries[i].human_size), mxsize.max_h_size_len, true);
    }
    else {
        (file_entries[i].size == 0) ? mx_printstr(" ") : mx_printstr("  ");
        printint_formatted(file_entries[i].size, mxsize.max_size_len);
    }

    mx_printchar(' ');
    mx_printstr(file_entries[i].date_time);
    mx_printchar(' ');
    if (flags->G) {
        struct stat sb;
        if (lstat(file_entries[i].path, &sb) == -1) {};
        switch_strcolor(sb);
    }
    mx_printstr(file_entries[i].name);
    if (flags->G)
        mx_printstr(DEFAULT_COLOR);
    if (flags->p && is_dir(file_entries[i].path))
        mx_printchar('/');
    else if (flags->F && is_executable(file_entries[i].path)) {
        mx_printchar('*');
    }
    else if (flags->F && is_link(file_entries[i].path)) {
        mx_printchar('@');
    }
    else if (flags->F && is_socket(file_entries[i].path)) {
        mx_printchar('=');
    }
    else if (flags->F && is_whiteout(file_entries[i].path)) {
        mx_printchar('%');
    }
    else if (flags->F && is_fifo(file_entries[i].path)) {
        mx_printchar('|');
    }

    if (file_entries[i].type == 'l') {
        mx_printstr(" -> ");
        mx_printstr(file_entries[i].symlink);
    }
    mx_printchar('\n');
    if (flags->at && file_entries[i].xattr_keys != NULL) {
        print_xattr(&file_entries[i], flags);
    }
}

void print_longlist(const char *dirname, t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int total_blocks = 0;
    struct stat sb;
    if (lstat(dirname, &sb) == -1) return;
    if ((!S_ISLNK(sb.st_mode) || dirname[mx_strlen(dirname) - 1] == '/') && is_dir(dirname) && count != 0) {
        mx_printstr("total ");

        for (int i = 0; i < count; ++i) {
            if (lstat(file_entries[i].path, &sb) == -1) continue;
            total_blocks += (int)(sb.st_blocks);
        }
        mx_printint(total_blocks);
        mx_printchar('\n');
    }
    t_max_sizes_s t_mxsize = {.max_groupname_len = 0, .max_nlinks_len = 0, .max_size_len = 0, .max_h_size_len = 0, .max_username_len = 0};
    for (int i = 0; i < count; ++i) {
        int group_len = mx_strlen(file_entries[i].group);
        int nlinks_len = mx_intlen(file_entries[i].nlinks);
        int size_len = mx_intlen(file_entries[i].size);
        int name_len = mx_strlen(file_entries[i].group);
        int h_size_len = mx_strlen(file_entries[i].human_size);
        if (group_len > t_mxsize.max_groupname_len) {
            t_mxsize.max_groupname_len = group_len;
        }
        if (nlinks_len > t_mxsize.max_nlinks_len) {
            t_mxsize.max_nlinks_len = nlinks_len;
        }
        if (flags->h) {
            if (h_size_len > t_mxsize.max_h_size_len) {
                t_mxsize.max_h_size_len = h_size_len;
            }
        }
        else {
            if (size_len > t_mxsize.max_size_len) {
                t_mxsize.max_size_len = size_len;
            }
        }
        if (name_len > t_mxsize.max_username_len) {
            t_mxsize.max_username_len = name_len;
        }
    }
    for (int i = 0; i < count; ++i) {
        print_file_entry_s(file_entries, i, t_mxsize, flags);
        if (acl_get_file(file_entries[i].path, ACL_TYPE_EXTENDED) && flags->e) {
            char **str_split = mx_strsplit(acl_to_text(acl_get_file(file_entries[i].path, ACL_TYPE_EXTENDED), NULL), '\n');
            for (int i = 1; str_split[i] != NULL; i++) {
                char **parts = mx_strsplit(str_split[i], ':');
                mx_printchar(' ');
                mx_printint(i - 1);
                mx_printstr(": ");
                mx_printstr(parts[0]);
                mx_printchar(':');
                mx_printstr(parts[2]);
                mx_printchar(' ');
                mx_printstr(parts[4]);
                mx_printchar(' ');
                mx_printstr(parts[5]);
                mx_printchar('\n');
                mx_del_strarr(&parts);
            }
            mx_del_strarr(&str_split);
        }
    }
}

void print_coma(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }
    int total_width = 0;
    for (int i = 0; i < count; i++) {
        if ((i + 1 == count) && (flags->p || flags->F) && (flags->i) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 9 + 1) <= terminal_width)) {
        }
        else if ((flags->p || flags->F) && (flags->i) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 2 + 1 + 9) > terminal_width)) {
            mx_printchar('\n');
            total_width = 0;
        }
        else if ((i + 1 == count) && (flags->p || flags->F) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 1) <= terminal_width)) {
        }
        else if ((flags->p || flags->F) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 2 + 1) > terminal_width)) {
            mx_printchar('\n');
            total_width = 0;
        }
        else if ((i + 1 == count) && ((total_width + mx_strlen(file_entries[i].name)) <= terminal_width)) {
        }
        else if ((total_width + mx_strlen(file_entries[i].name) + 2) >= terminal_width) {
            mx_printchar('\n');
            total_width = 0;
        }
        if (flags->i) {
            printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
            mx_printchar(' ');
        }
        if (flags->G) {
            struct stat sb;
            if (lstat(file_entries[i].path, &sb) == -1) {}
            switch_strcolor(sb);
        }
        mx_printstr(file_entries[i].name);
        if (flags->G)
            mx_printstr(DEFAULT_COLOR);
        if (flags->p && is_dir(file_entries[i].path))
            mx_printchar('/');
        else if (flags->F && is_executable(file_entries[i].path)) {
            mx_printchar('*');
        }
        else if (flags->F && is_link(file_entries[i].path)) {
            mx_printchar('@');
        }
        else if (flags->F && is_socket(file_entries[i].path)) {
            mx_printchar('=');
        }
        else if (flags->F && is_whiteout(file_entries[i].path)) {
            mx_printchar('%');
        }
        else if (flags->F && is_fifo(file_entries[i].path)) {
            mx_printchar('|');
        }
        if (i + 1 != count)
            mx_printstr(", ");
        total_width = total_width + mx_strlen(file_entries[i].name) + 2;
        if ((flags->p && is_dir(file_entries[i].path)) || (flags->F && is_smth(file_entries[i].path)))
            total_width++;
        if (flags->i)
            total_width += 9;
    }
    mx_printchar('\n');
}

void print_err(const char *filename) {
    mx_printerr("ls: ");
    mx_printerr(filename);
    mx_printerr(": ");
    mx_printerr(strerror(errno));
    mx_printerr("\n");
}


void custom_qsort(void *base, size_t num_elements, size_t element_size, int (*comparator)(const void *, const void *, bool), t_flags_s *flags) {
    for (size_t i = 0; i < num_elements - 1; ++i) {
        for (size_t j = i + 1; j < num_elements; ++j) {
            void *element_i = (char *)base + i * element_size;
            void *element_j = (char *)base + j * element_size;
            if (comparator(element_i, element_j, flags->r) > 0) {
                char temp[element_size];
                mx_memcpy(temp, element_i, element_size);
                mx_memcpy(element_i, element_j, element_size);
                mx_memcpy(element_j, temp, element_size);
            }
        }
    }
}

char *format_size(long size) {
    char *suffix[] = {"B", "K", "M", "G", "T"};
    int i;
    double d_size = (double)size;

    for (i = 0; i < 6; i++) {
        if (d_size < 1000) {
            break;
        }
        d_size /= 1024;
    }
    double size_r = custom_round(d_size * 10) / 10;
    char *result = (char *)malloc(20);
    if (size_r >= 10 || i == 0) {
        char *str = mx_lltoa(custom_round(d_size));
        result = mx_strjoin(result, str);
        free(str);
    }
    else {
        char *str = mx_lltoa(size_r);
        result = mx_strjoin(mx_strjoin(result, str), ".");
        free(str);
        str = mx_lltoa((long long)(size_r * 10) % 10);
        result = mx_strjoin(result, str);
        free(str);
    }
    result = mx_strjoin(result, suffix[i]);
    return result;
}

int is_directory_exists(const char *dirname) {
    DIR *dir = opendir(dirname);

    if (dir != NULL) {
        closedir(dir);
        return 1;
    }

    return 0;
}

int is_directory_empty(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) return 1;

    struct dirent *entry;
    int file = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        file++;
        break;
    }

    closedir(dir);

    return file == 0;
}

int is_file_exists(const char *dirname) {
    struct stat buffer;
    return stat(dirname, &buffer) == 0;
}

void init_flags(t_flags_s *flags) {
    flags->one = false;
    flags->C = false;
    flags->m = false;
    flags->l = false;
    flags->c = false;
    flags->u = false;
    flags->U = false;
    flags->t = false;
    flags->S = false;
    flags->f = false;
    flags->A = false;
    flags->a = false;
    flags->R = false;
    flags->G = false;
    flags->F = false;
    flags->p = false;
    flags->r = false;
    flags->h = false;
    flags->e = false;
    flags->T = false;
    flags->at = false;
    flags->g = false;
    flags->o = false;
    flags->x = false;
    flags->n = false;
    flags->i = false;
}

char **get_xattr(const char *filename) {
    char buffer[1024] = {'\0'};
    ssize_t count = listxattr(filename, buffer, sizeof(buffer), XATTR_NOFOLLOW);
    for (int i = 0; i < count - 1; i++)
        if (buffer[i] == '\0')
            buffer[i] = '|';

    if (count > 0)
        return mx_strsplit(buffer, '|');

    return NULL;
}


t_file_entry_s *fill_entry(const char *name, t_flags_s *flags) {
    struct stat sb;
    if (lstat(name, &sb) == -1) {
        return NULL;
    }
    t_file_entry_s *file_entry = malloc(sizeof(t_file_entry_s));
    if (flags->i)
        file_entry->inode = sb.st_ino;
    file_entry->name = mx_strdup(name);
    file_entry->path = mx_strdup(name);
    if (S_ISLNK(sb.st_mode))
        file_entry->type = 'l';
    else if (S_ISSOCK(sb.st_mode))
        file_entry->type = 's';
    else if (S_ISCHR(sb.st_mode))
        file_entry->type = 'c';
    else if (S_ISFIFO(sb.st_mode))
        file_entry->type = 'p';
    else if (S_ISBLK(sb.st_mode))
        file_entry->type = 'b';
    else
        file_entry->type = '-';

    char *new_permissions = NULL;
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRUSR) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWUSR) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXUSR) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRGRP) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWGRP) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXGRP) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IROTH) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWOTH) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXOTH) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (listxattr(name, NULL, 0, 0x0001) > 0) ? "@" : (acl_get_file(name, ACL_TYPE_EXTENDED)) ? "+"
                                                                                                                                         : " ");

    if (S_ISLNK(sb.st_mode))
        readlink(name, file_entry->symlink, sizeof(file_entry->symlink));
    if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
        mx_strcpy(file_entry->permissions, new_permissions);
    }
    else {
        exit(1);
    }
    free(new_permissions);
    file_entry->nlinks = (int)sb.st_nlink;

    struct passwd *pw = getpwuid(sb.st_uid);
    struct group *gr = getgrgid(sb.st_gid);
    if (!flags->n) {
        mx_strcpy(file_entry->owner, pw->pw_name);
        mx_strcpy(file_entry->group, gr->gr_name);
    }
    else {
        mx_strcpy(file_entry->owner, mx_itoa(pw->pw_uid));
        mx_strcpy(file_entry->group, mx_itoa(gr->gr_gid));
    }
    if (flags->h) {
        char *new_size = NULL;
        new_size = mx_strjoin(new_size, format_size(sb.st_size));
        mx_strcpy(file_entry->human_size, new_size);
    }
    else {
        file_entry->size = sb.st_size;
    }

    struct timespec timesp = sb.st_mtimespec;
    if (flags->c)
        timesp = sb.st_ctimespec;
    if (flags->u)
        timesp = sb.st_atimespec;
    if (flags->U)
        timesp = sb.st_birthtimespec;
    if (flags->t)
        file_entry->cmptime = timesp;
    char *str = ctime(&timesp.tv_sec);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
    arr[4][4] = '\0';
    char *new_date_time = NULL;
    if (flags->T) {
        for (int i = 1; arr[i] != NULL; i++) {
            int spaces = 2 - mx_strlen(arr[i]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[i]);

            if (arr[i + 1] != NULL) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
        }
    }
    else if (timesp.tv_sec + six_months_sec <= now || timesp.tv_sec >= now + six_months_sec) {
        new_date_time = mx_strjoin(new_date_time, arr[1]);
        new_date_time = mx_strjoin(new_date_time, " ");
        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_date_time = mx_strjoin(new_date_time, " ");
        }
        new_date_time = mx_strjoin(new_date_time, arr[2]);
        new_date_time = mx_strjoin(new_date_time, "  ");
        new_date_time = mx_strjoin(new_date_time, arr[4]);
    }
    else {
        new_date_time = mx_strjoin(new_date_time, arr[1]);
        new_date_time = mx_strjoin(new_date_time, " ");

        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_date_time = mx_strjoin(new_date_time, " ");
        }
        new_date_time = mx_strjoin(new_date_time, arr[2]);

        new_date_time = mx_strjoin(new_date_time, " ");
        char **arr_time = mx_strsplit(arr[3], ':');
        new_date_time = mx_strjoin(new_date_time, arr_time[0]);
        new_date_time = mx_strjoin(new_date_time, ":");
        new_date_time = mx_strjoin(new_date_time, arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    mx_del_strarr(&arr);
    if ((unsigned long)mx_strlen(new_date_time) <= sizeof(file_entry->date_time)) {
        mx_strcpy(file_entry->date_time, new_date_time);
    }
    else exit(1);
    free(new_date_time);

    if (S_ISLNK(sb.st_mode)) {
        ssize_t len = readlink(name, file_entry->symlink, sizeof(file_entry->symlink) - 1);
        if (len != -1) {
            file_entry->symlink[len] = '\0';
        }
    }
    else {
        file_entry->symlink[0] = '\0';
    }
    file_entry->xattr_keys = get_xattr(name);
    return file_entry;
}

t_file_entry_s *fill_file_entries(const char *dirname, int *count, t_flags_s *flags) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char *file_path;
    if (!(dir = opendir(dirname))) {
        return NULL;
    }
    *count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if ((mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) && flags->A)
            continue;
        if (entry->d_name[0] == '.' && !flags->a)
            continue;
        (*count)++;
    }
    closedir(dir);
    t_file_entry_s *file_entries = malloc(*count * sizeof(t_file_entry_s));
    if (file_entries == NULL) {
        return NULL;
    }

    dir = opendir(dirname);
    if (dir == NULL) {
        free(file_entries);
        return NULL;
    }

    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if ((mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) && flags->A)
            continue;
        if (entry->d_name[0] == '.' && !flags->a)
            continue;

        file_path = mx_strjoin(mx_strjoin(dirname, "/"), entry->d_name);

        if (lstat(file_path, &sb) == -1) continue;
        t_file_entry_s *file_entry = &file_entries[index];
        if (flags->i)
            file_entry->inode = sb.st_ino;
        file_entry->name = mx_strdup(entry->d_name);
        file_entry->path = mx_strdup(file_path);
        if (file_entry->name == NULL) {
            free(file_entries->name);
            closedir(dir);
            return NULL;
        }
        if (S_ISDIR(sb.st_mode))
            file_entry->type = 'd';
        else if (S_ISLNK(sb.st_mode))
            file_entry->type = 'l';
        else if (S_ISSOCK(sb.st_mode))
            file_entry->type = 's';
        else if (S_ISCHR(sb.st_mode))
            file_entry->type = 'c';
        else if (S_ISFIFO(sb.st_mode))
            file_entry->type = 'p';
        else if (S_ISBLK(sb.st_mode))
            file_entry->type = 'b';
        else
            file_entry->type = '-';

        char *new_permissions = NULL;
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRUSR) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWUSR) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXUSR) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRGRP) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWGRP) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXGRP) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IROTH) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWOTH) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXOTH) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (listxattr(file_path, NULL, 0, XATTR_NOFOLLOW) > 0) ? "@" : (acl_get_file(file_path, ACL_TYPE_EXTENDED)) ? "+"
                                                                                                                                                               : " ");
        if (S_ISLNK(sb.st_mode))
            readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink));
        if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
            mx_strcpy(file_entry->permissions, new_permissions);
        }
        else exit(1);
        free(new_permissions);
        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        if (!flags->n) {
            mx_strcpy(file_entry->owner, pw->pw_name);
            mx_strcpy(file_entry->group, gr->gr_name);
        }
        else {
            mx_strcpy(file_entry->owner, mx_itoa(pw->pw_uid));
            mx_strcpy(file_entry->group, mx_itoa(gr->gr_gid));
        }
        if (flags->h) {
            char *new_size = NULL;
            new_size = mx_strjoin(new_size, format_size(sb.st_size));
            mx_strcpy(file_entry->human_size, new_size);
        }
        else {
            file_entry->size = sb.st_size;
        }

        struct timespec timesp = sb.st_mtimespec;
        if (flags->c)
            timesp = sb.st_ctimespec;
        if (flags->u)
            timesp = sb.st_atimespec;
        if (flags->U)
            timesp = sb.st_birthtimespec;
        if (flags->t)
            file_entry->cmptime = timesp;
        char *str = ctime(&timesp.tv_sec);
        char **arr = mx_strsplit(str, ' ');
        time_t now = time(NULL);
        time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
        arr[4][4] = '\0';
        char *new_date_time = NULL;
        if (flags->T) {

            for (int i = 1; arr[i] != NULL; i++) {

                int spaces = 2 - mx_strlen(arr[i]);
                for (int i = 0; i < spaces; i++) {
                    new_date_time = mx_strjoin(new_date_time, " ");
                }
                new_date_time = mx_strjoin(new_date_time, arr[i]);

                if (arr[i + 1] != NULL) {
                    new_date_time = mx_strjoin(new_date_time, " ");
                }
            }
        }
        else if (timesp.tv_sec + six_months_sec <= now || timesp.tv_sec >= now + six_months_sec) {
            new_date_time = mx_strjoin(new_date_time, arr[1]);
            new_date_time = mx_strjoin(new_date_time, " ");
            int spaces = 2 - mx_strlen(arr[2]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[2]);
            new_date_time = mx_strjoin(new_date_time, "  ");
            new_date_time = mx_strjoin(new_date_time, arr[4]);
        }
        else {
            new_date_time = mx_strjoin(new_date_time, arr[1]);
            new_date_time = mx_strjoin(new_date_time, " ");

            int spaces = 2 - mx_strlen(arr[2]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[2]);

            new_date_time = mx_strjoin(new_date_time, " ");
            char **arr_time = mx_strsplit(arr[3], ':');
            new_date_time = mx_strjoin(new_date_time, arr_time[0]);
            new_date_time = mx_strjoin(new_date_time, ":");
            new_date_time = mx_strjoin(new_date_time, arr_time[1]);
            mx_del_strarr(&arr_time);
        }

        mx_del_strarr(&arr);
        if ((unsigned long)mx_strlen(new_date_time) <= sizeof(file_entry->date_time)) {
            mx_strcpy(file_entry->date_time, new_date_time);
        }
        else exit(1);
        free(new_date_time);

        if (S_ISLNK(sb.st_mode)) {
            ssize_t len = readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink) - 1);
            if (len != -1) {
                file_entry->symlink[len] = '\0';
            }
        }
        else {
            file_entry->symlink[0] = '\0';
        }
        index++;

        file_entry->xattr_keys = get_xattr(file_path);
    }

    closedir(dir);

    return file_entries;
}

int parse_args(int argc, char *argv[], t_flags_s *flags) {
    int i, count = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                switch (argv[i][j]) {
                case 'a':
                    flags->a = true;
                    flags->A = false;
                    break;
                case 'A':
                    if (!flags->a)
                        flags->A = true;
                    flags->a = true;
                    break;
                case 'l':
                    flags->l = true;
                    flags->C = false;
                    flags->one = false;
                    flags->m = false;
                    flags->x = false;
                    break;
                case 'r':
                    if (!flags->f)
                        flags->r = true;
                    break;
                case 'R':
                    flags->R = true;
                    break;
                case 't':
                    if (!flags->f)
                        flags->t = true;
                    flags->S = false;
                    break;
                case 'u':
                    flags->u = true;
                    flags->c = false;
                    flags->U = false;
                    break;
                case 'c':
                    flags->c = true;
                    flags->U = false;
                    flags->u = false;
                    break;
                case 'G':
                    flags->G = true;
                    break;
                case 'h':
                    flags->h = true;
                    break;
                case 'e':
                    flags->e = true;
                    break;
                case 'S':
                    if (!flags->f)
                        flags->S = true;
                    flags->t = false;
                    break;
                case 'T':
                    flags->T = true;
                    break;
                case 'x':
                    flags->x = true;
                    flags->l = false;
                    flags->C = false;
                    flags->one = false;
                    break;
                case 'p':
                    flags->p = true;
                    flags->F = false;
                    break;
                case 'f':
                    flags->f = true;
                    flags->a = true;
                    flags->S = false;
                    flags->t = false;
                    flags->r = false;
                    break;
                case 'n':
                    flags->n = true;
                    flags->l = true;
                    break;
                case 'g':
                    flags->g = true;
                    flags->l = true;
                    break;
                case 'o':
                    flags->o = true;
                    flags->l = true;
                    break;
                case 'F':
                    flags->F = true;
                    flags->p = true;
                    break;
                case '1':
                    flags->one = true;
                    flags->C = false;
                    flags->l = false;
                    flags->m = false;
                    flags->x = false;
                    break;
                case '@':
                    flags->at = true;
                    break;
                case 'C':
                    flags->C = true;
                    flags->one = false;
                    flags->l = false;
                    flags->x = false;
                    break;
                case 'm':
                    flags->m = true;
                    flags->l = false;
                    flags->one = false;
                    break;
                case 'U':
                    flags->U = true;
                    flags->u = false;
                    flags->c = false;
                    break;
                case 'i':
                    flags->i = true;
                    break;
                default:
                    mx_printerr("ls: illegal option -- ");
                    mx_printerr(&argv[i][j]);
                    mx_printerr("\n");
                    mx_printerr("usage: ls [-@ACFGRSTUacefghlmnoprtux1] [file ...]\n");
                    exit(1);
                }
            }
            count++;
        }
    }
    return count;
}

int parse_dirs(int argc, char *argv[], char ***dirs) {
    int i, dir_count = 0;

    *dirs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (is_dir(argv[i]) && (is_directory_exists(argv[i]) || is_file_exists(argv[i]))) {
                (*dirs)[dir_count] = mx_strdup(argv[i]);
                dir_count++;
            }
        }
    }
    return dir_count;
}

int parse_err_dirs(int argc, char *argv[], char ***err_dirs) {
    int i, err_dir_count = 0;

    *err_dirs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!(is_directory_exists(argv[i]) || is_file_exists(argv[i]))) {
                (*err_dirs)[err_dir_count] = mx_strdup(argv[i]);
                err_dir_count++;
            }
        }
    }
    return err_dir_count;
}

int parse_file_input(int argc, char *argv[], char ***file_inputs) {
    int i, file_inputs_count = 0;

    *file_inputs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!is_dir(argv[i]) && is_file_exists(argv[i])) {
                (*file_inputs)[file_inputs_count] = mx_strdup(argv[i]);
                file_inputs_count++;
            }
        }
    }
    return file_inputs_count;
}
void recursive_flag(const char *path, t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    if (flags->l)
        print_longlist(path, file_entries, count, flags);
    else if (flags->m)
        print_coma(file_entries, count, flags);
    else if (flags->C)
        print_multicolumn(file_entries, count, flags);
    else if (flags->one)
        print_perline(file_entries, count, flags);
    else if (isatty(1))
        print_multicolumn(file_entries, count, flags);
    else if (flags->x)
        print_multicolumn(file_entries, count, flags);
    else
        print_perline(file_entries, count, flags);

    for (int i = 0; i < count; i++) {
        if ((file_entries[i].name[0] == '.' && !flags->a) || file_entries[i].type != 'd' || (mx_strcmp(file_entries[i].name, ".") == 0 || mx_strcmp(file_entries[i].name, "..") == 0))
            continue;

        mx_printchar('\n');
        mx_printstr(file_entries[i].path);
        mx_printchar(':');
        mx_printchar('\n');
        int inner_count;
        t_file_entry_s *inner_file_entries = fill_file_entries(file_entries[i].path, &inner_count, flags);
            if(inner_file_entries == NULL && !flags->l) {
                print_err(file_entries[i].path);
                continue;
            }
            if (is_directory_empty(file_entries[i].path) && !flags->a && !flags->l)
                continue;
            else if (is_directory_empty(file_entries[i].path) && flags->l) {
                if(inner_file_entries == NULL) print_err(file_entries[i].path);
                continue;
            }
        if(inner_count) {
        if (flags->S)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_size, flags);
        else if (flags->t)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_date_time, flags);
        else if (!flags->f)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_name, flags);
        }
        recursive_flag(file_entries[i].path, inner_file_entries, inner_count, flags);
        free(inner_file_entries);
    }
}

void ls(int argc, char *argv[]) {
    char *dirname;
    t_flags_s flags;
    init_flags(&flags);
    char **dirs;
    char **err_dirs;
    char **input_files;
    parse_args(argc, argv, &flags);
    if (!isatty(1))
        flags.G = false;
    int count_dirs = parse_dirs(argc, argv, &dirs);
    int count_err_dirs = parse_err_dirs(argc, argv, &err_dirs);
    int count_file_input = parse_file_input(argc, argv, &input_files);
    int real_count_dirs = count_dirs + count_err_dirs + count_file_input;
    t_file_entry_s *file_entries = malloc(count_file_input * sizeof(t_file_entry_s));
    t_file_entry_s *dir_entries = malloc(count_dirs * sizeof(t_file_entry_s));
    for (int i = 0; i < count_dirs; i++) {
        dirname = dirs[i];
        dir_entries[i] = *fill_entry(dirname, &flags);
    }
    if (real_count_dirs == 0) {
        dirname = ".";
        count_dirs = 1;
        dir_entries[0].name = mx_strdup(dirname);
    }
    if (count_dirs != 0){
        if (flags.S)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
        else if (flags.t)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
        else if (!flags.f)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_name, &flags);
    }
    if (count_err_dirs != 0)
        custom_qsort(err_dirs, count_err_dirs, sizeof(char *), compare_names, &flags);
    if (count_file_input != 0)
        custom_qsort(input_files, count_file_input, sizeof(char *), compare_names, &flags);
    for (int i = 0; i < count_err_dirs; i++) {
        print_err(err_dirs[i]);
    }
    for (int i = 0; i < count_file_input; i++) {
        dirname = input_files[i];
        file_entries[i] = *fill_entry(dirname, &flags);
    }
    if(count_file_input) {
        if (flags.S)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
        else if (flags.t)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
        else if (!flags.f)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_name, &flags);

        if (flags.R)
            recursive_flag(dirname, file_entries, count_file_input, &flags);
        else if (flags.l) {
            print_longlist(dirname, file_entries, count_file_input, &flags);
        }
        else if (flags.m)
            print_coma(file_entries, count_file_input, &flags);
        else if (flags.C)
            print_multicolumn(file_entries, count_file_input, &flags);
        else if (flags.one) {
            print_perline(file_entries, count_file_input, &flags);
        }
        else if (isatty(1))
            print_multicolumn(file_entries, count_file_input, &flags);
        else if (flags.x)
            print_multicolumn(file_entries, count_file_input, &flags);
        else
            print_perline(file_entries, count_file_input, &flags);
    }
    if(count_file_input && (count_dirs || count_err_dirs)) mx_printchar('\n');
    for (int i = 0; i < count_dirs; i++) {
        dirname = dir_entries[i].name;
        if (real_count_dirs >= 2) {
            mx_printstr(dirname);
            mx_printstr(":\n");
        }
        int count_files;
        struct stat sb;
        if (lstat(dirname, &sb) == -1) {
            print_err(dirname);
        }
        t_file_entry_s *file_entries = fill_file_entries(dirname, &count_files, &flags);
        if(file_entries == NULL && !flags.l) {
            print_err(dirname);
            continue;
        }
            if (is_directory_empty(dirname) && count_dirs >= 2 && !flags.l && i+1 != count_dirs)
                mx_printchar('\n');
            if (is_directory_empty(dirname) && !flags.a && !flags.l)
                continue;
            else if (is_directory_empty(dirname) && flags.l) {
                if(file_entries == NULL) print_err(dirname);
                if(count_dirs >= 2 && i + 1 != count_dirs) mx_printchar('\n');
                continue;
            }
            if(count_files) {
                if (flags.S)
                    custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
                else if (flags.t)
                    custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
                else if (!flags.f){
                    custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_name, &flags);
            }
            }

            
        
        if (flags.R)
            recursive_flag(dirname, file_entries, count_files, &flags);
        else if (flags.l) {
            print_longlist(dirname, file_entries, count_files, &flags);
        }
        else if (flags.m)
            print_coma(file_entries, count_files, &flags);
        else if (flags.C)
            print_multicolumn(file_entries, count_files, &flags);
        else if (flags.one) {
            print_perline(file_entries, count_files, &flags);
        }
        else if (isatty(1))
            print_multicolumn(file_entries, count_files, &flags);
        else if (flags.x)
            print_multicolumn(file_entries, count_files, &flags);
        else
            print_perline(file_entries, count_files, &flags);
        if (i + 1 != count_dirs)
            mx_printchar('\n');
    }
    return;
}

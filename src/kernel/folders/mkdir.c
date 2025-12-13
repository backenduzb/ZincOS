// src/kernel/folders/mkdir.c

#ifndef MKDIR_C
#define MKDIR_C

#include "../config/colors.c"
#include "../config/settings.c"

// string.h emas, o'zingizning funksiyalaringizni include qiling
// #include <string.h>  // BU QATORNI O'CHIRING
#include "../config/functions.c"  // functions.h fayl yaratish kerak

#define MAX_DIRS 100
#define MAX_FILES_IN_DIR 50
#define NAME_LEN 32

typedef struct {
    char name[NAME_LEN];
    int is_dir;
} DirEntry;

typedef struct {
    DirEntry entries[MAX_FILES_IN_DIR];
    int count;
    char name[NAME_LEN];
} Directory;

Directory dirs[MAX_DIRS];
int dir_count = 0;
int current_dir = 0;

void init_dirs() {
    dir_count = 1;
    dirs[0].count = 0;
    strcpy(dirs[0].name, "root");
}

void mkdir_cmd(char* name) {
    if (dir_count >= MAX_DIRS) {
        return;
    }
    
    if (dirs[current_dir].count >= MAX_FILES_IN_DIR) {
        return;
    }
    
    Directory* new_dir = &dirs[dir_count];
    new_dir->count = 0;
    strcpy(new_dir->name, name);
    
    Directory* cur = &dirs[current_dir];
    strcpy(cur->entries[cur->count].name, name);
    cur->entries[cur->count].is_dir = 1;
    cur->count++;
    
    dir_count++;
}

void ls_cmd() {
    Directory* cur = &dirs[current_dir];
    
    print_string("Contents:", 0, 15, WHITE_ON_BLACK);
    
    for (int i = 0; i < cur->count; i++) {
        char display[40];
        if (cur->entries[i].is_dir) {
            strcpy(display, "[DIR]  ");
            strcat(display, cur->entries[i].name);
            strcat(display, "/");
        } else {
            strcpy(display, "[FILE] ");
            strcat(display, cur->entries[i].name);
        }
        
        print_string(display, 0, 16 + i, WHITE_ON_BLACK);
    }
}

#endif // MKDIR_C
// dir.h
#ifndef DIR_H
#define DIR_H

#define MAX_DIRS 100
#define MAX_FILES_IN_DIR 50
#define NAME_LEN 32

typedef struct {
    char name[NAME_LEN];
    int is_dir;  // 1=directory, 0=file
} DirEntry;

typedef struct {
    DirEntry entries[MAX_FILES_IN_DIR];
    int count;
    char name[NAME_LEN];
} Directory;

// Global directory array
Directory dirs[MAX_DIRS];
int dir_count = 0;
int current_dir = 0;  // 0 = root directory

#endif
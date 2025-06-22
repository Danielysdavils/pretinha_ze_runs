#ifndef __DATA__
#define __DATA__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILE_PATH "data/data.txt"

typedef struct {
    int lavel;

} Database;

Database* CreateDataBase();

bool ExistDataBase();

bool UpdateDataBase(int lavel, Database* db);

bool DeleteDataBase();

Database* GetDataBase();

#endif
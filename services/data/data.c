/*
    AUTOR: DANIELYS DAVILA
    DATA: 09/06/2025
    VERSÃO: 0.0.1

    DATABASE PARA ARMAZENAR O NIVEL ATUAL DO JOGO

*/

#include "data.h"

Database* CreateDataBase()
{
    FILE* database = fopen(FILE_PATH, "w");
    if(!database) return false;

    fprintf(database, "lavel: 0\n");
    fclose(database);

    Database *db = malloc(sizeof(Database));
    if(!db) return NULL;
    
    db->lavel = 0;
    return db;
}

bool ExistDataBase()
{
    FILE* database = fopen(FILE_PATH, "r");
    if(!database) return false;

    fclose(database);
    return true;
}

bool UpdateDataBase(int lavel, Database* db)
{
    FILE* database = fopen(FILE_PATH, "w");
    if(!database) return false;

    fprintf(database, "lavel: %d\n", lavel);
    fclose(database);

    db->lavel = lavel;
    return true;
}

bool RemoveDataBase()
{
    return remove(FILE_PATH) == 0;
}

Database* GetDataBase()
{
    FILE* database_local = fopen(FILE_PATH, "r");
    if(!database_local) return false;

    Database *database = malloc(sizeof(Database));
    if(!database)
    {
        fclose(database_local);
        return false;
    } 

    if(fscanf(database_local, "lavel: %d", &database->lavel) != 1)
    {
        free(database);
        fclose(database_local);
        return NULL;
    }

    fclose(database_local);
    return database;
}

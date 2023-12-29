#ifndef FUNCOES_H
#define FUNCOES_H

void createDatabase(char* dbname);
void showDatabases();
void dropDatabase(char* dbname);
void useDatabase(char* dbname);
void whichdb();
void createTable(char* tableName, int numColumns);
void showTables();

#endif // FUNCOES_H
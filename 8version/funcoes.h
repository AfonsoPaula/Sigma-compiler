#ifndef FUNCOES_H
#define FUNCOES_H
// Databases Functions
void createDatabase(char* dbname);
void showDatabases();
void useDatabase(char* dbname);
void dropDatabase(char* dbname);
void whichDatabase();
// Tables Functions
void createTable(char* tableName, int numColumns);
void showTables();
void useTable(char* tableName);
void whichTable();

#endif // FUNCOES_H
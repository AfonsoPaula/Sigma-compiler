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
void insertIntoTable(char* tableName);
void selectTable(char* tableName);
void deleteRowFromTable(char* tableName, int rowNum);
void deleteColFromTable(char* tableName, char* columnName);
void updateRow(char* tableName, int rowNum);
void dropTable(char* tableName);

#endif // FUNCOES_H
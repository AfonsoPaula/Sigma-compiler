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
void selectIgualNum(char* tableName, char* columnName, int number);
void selectIgualStr(char* tableName, char* columnName, char* searchString);
void selectMaiorIgualNum(char* tableName, char* columnName, int number);
void selectMenorIgualNum(char* tableName, char* columnName, int number);
void selectMaiorNum(char* tableName, char* columnName, int number);
void selectMenorNum(char* tableName, char* columnName, int number);
void selectDiffNum(char* tableName, char* columnName, int number);
void selectDiffStr(char* tableName, char* columnName, char* searchString);

// Aux Functions
int checkActiveDatabase();
int isDatabaseActive();
int findTableIndex(const char* tableName);
int findColumnIndex(int tableIndex, const char* columnName);
void printTableHeader(int tableIndex);
void printTableFooter(int tableIndex);

#endif // FUNCOES_H
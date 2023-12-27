/* ----------------- Definition Section ------------------ */
%{
void yyerror(const char* s);
int yylex(void);
extern char *yyget_text(void);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATABASES     100
#define MAX_TABLES        100
#define MAX_COLUMNS        50
#define MAX_ROWS          100
#define MAX_TABLES_PER_DB  50

/* ----------------------- TABLES ------------------------ */
typedef struct {
    char name[256];
    char columns[MAX_COLUMNS][256];
    int numColumns;
    char data[MAX_ROWS][MAX_COLUMNS][256];
    int numRows;
} Table;

Table tables[MAX_TABLES];
int numTables = 0;

/* --------------------- DATABASES ----------------------- */
typedef struct{
    char name[256];
    Table tables[MAX_TABLES_PER_DB];
    int numTables;
} Database;

Database databases[MAX_DATABASES];
int numDatabases = 0;

/* ---------------------- FUNCTIONS ---------------------- */
void createDatabase(char* dbName);
void showDatabases();
void dropDatabase(char* dbname);

void useDatabase(char* dbname);
void createTable(char* tableName, int numColumns);

int activeDatabaseIndex = -1;
%}

%union { int num; char* id;}
%start query

%token EXTCMD LINE
%token CREATE SHOW DROP DATABASE DATABASES USE TABLE
%token SEMICOLON COMMA OPENPAR CLOSEPAR

%token <id> IDENTIFIER
%token <num> NUMBER

/* -------------------- Rule Section --------------------- */
%%

query: command SEMICOLON             { ; }
    |  EXTCMD                        { exit(EXIT_SUCCESS); }
    |  query command SEMICOLON       { ; }
    ;

command: createdb 
        |showdbs
        |dropdb
        |usedb
        |createtb
        ;

createdb: CREATE DATABASE IDENTIFIER SEMICOLON LINE   { createDatabase($3); }
        | CREATE DATABASE error LINE                  { printf("\n [Query 'CREATE DATABASE' não se encontra bem construida!]\n");
                                                        printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        | CREATE error LINE {printf(" [Pretende 'CREATE DATABASE' ou 'CREATE TABLE'?]\n");}
        ;

showdbs:  SHOW DATABASES SEMICOLON LINE               { showDatabases(); }
        | SHOW error LINE                             { printf("\n [Query 'SHOW' não se encontra bem construida!]\n");
                                                        printf(" [DICA: SHOW DATABASES;]\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

dropdb:  DROP DATABASE IDENTIFIER SEMICOLON LINE      { dropDatabase($3); }
        | DROP error LINE                             { printf("\n [Query 'DROP' não se encontra bem construida!]\n");
                                                        printf(" [DICA: DROP DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

usedb: USE IDENTIFIER SEMICOLON LINE                  { useDatabase($2); }
    | USE error LINE                                  { printf("\n [Query 'USE' não se encontra bem construida!]\n");
                                                        printf(" [DICA: USE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }

createtb: CREATE TABLE IDENTIFIER OPENPAR NUMBER CLOSEPAR SEMICOLON LINE      { createTable($3,$5); }
        | CREATE TABLE error LINE                                             { printf("\n [Query 'CREATE TABLE' não está bem construída!]\n");
                                                                                printf(" [DICA: CREATE TABLE table_name (numColumns);]\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
        ;
%%

/* -------------------------- CREATE DATABASE ------------------------- */
void createDatabase(char* dbname) 
{
    for (int i = 0; i < numDatabases; ++i) {
        if(strcmp(databases[i].name, dbname) == 0){
            printf("\n [Erro: A base de dados '%s' já existe]\n", dbname);
            printf(" [ENTER] para tentar de novo\n");
            return;
        }
    }
    if (numDatabases < MAX_DATABASES) {
        strcpy(databases[numDatabases].name, dbname);
        numDatabases++;
        printf("\n [Base de dados '%s' criada com sucesso!]\n", dbname);
        printf(" [ENTER] para prosseguir\n");
    } else {
        printf("\n [Erro: Limite de bases de dados atingido]\n");
        printf(" [ENTER] para tentar de novo\n");
    }
}
/* -------------------------- SHOW DATABASES -------------------------- */
void showDatabases()
{
    if (numDatabases == 0) {
        printf("\n [Nenhuma base de dados foi criada ainda]\n");
    } else {
        printf("\n Bases de dados armazenadas:\n");
        for (int i = 0; i < numDatabases; ++i) {
            printf("   [%d] %s;\n", i, databases[i].name);
        }
    }
    printf("\n [ENTER] para prosseguir\n");
}
/* -------------------------- DROP DATABASES -------------------------- */
void dropDatabase(char* dbname)
{
    if (numDatabases == 0) {
        printf("\n [Não existem bases de dados guardadas, utilize o comando 'CREATE' para criar uma]\n");
        printf(" [ENTER] para tentar de novo\n");
    } else {
        int found = 0;
        for (int i = 0; i < numDatabases; ++i) {
            if (strcmp(databases[i].name, dbname) == 0) {
                for (int j = i; j < numDatabases - 1; ++j) {
                    strcpy(databases[j].name, databases[j + 1].name);
                }
                numDatabases--;
                found = 1;
                printf("\n [A base de dados '%s' foi eliminada com sucesso!]\n", dbname);
                printf(" [ENTER] para prosseguir\n");
                break;
            }
        }

        if (!found) {
            printf("\n [Erro: A base de dados '%s' não foi encontrada]\n", dbname);
            printf(" [ENTER] para tentar de novo\n");
        }
    }
}
/* --------------------------- USE DATABASE ---------------------------- */
void useDatabase(char* dbname)
{
    int found = 0;

    for (int i = 0; i < numDatabases; ++i) {
        if (strcmp(databases[i].name, dbname) == 0) {
            activeDatabaseIndex = i;
            found = 1;
            printf("\n [Base de dados '%s' (index:[%i]) ativa]\n", dbname, i);
            printf(" [A partir de agora, você pode criar tabelas nesta base de dados]\n");
            printf(" [ENTER] para prosseguir\n");
            break;
        }
    }

    if (!found) {
        printf("\n [Erro: A base de dados '%s' não foi encontrada]\n", dbname);
        printf(" [ENTER] para tentar de novo\n");
    }
}
/* --------------------------- CREATE TABLE ---------------------------- */
void createTable(char* tableName, int numColumns)
{
    char columnName[256];

    // Verificar se há uma base de dados ativa
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma base de dados ativa]\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Verificar se o número inserido é válido
    if (numColumns <= 0 || numColumns > MAX_COLUMNS) {
        printf("\n [Número inválido de colunas (entre 0 e 50)]\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    Table newTable;
    strcpy(newTable.name, tableName);
    newTable.numColumns = numColumns;

    for (int i = 0; i < numColumns; ++i) {
        printf("\n [Digite o nome da coluna %d]: ", i+1);
        scanf("%s", columnName);
        strcpy(newTable.columns[i], columnName);
    }

    // Adicione a nova tabela à base de dados ativa
    databases[activeDatabaseIndex].tables[databases[activeDatabaseIndex].numTables] = newTable;
    databases[activeDatabaseIndex].numTables++;

    // Verificar se a tabela foi armazenada na base de dados
    int found = 0;
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            found = 1;
            break;
        }
    }

    // Mensagem de confirmação
    if (found) {
        printf("\n Tabela '%s' criada com sucesso na base de dados '%s' com as seguintes colunas:\n", tableName, databases[activeDatabaseIndex].name);
        for (int i = 0; i < newTable.numColumns; ++i) {
            printf("  -> %s\n", newTable.columns[i]);
        }
        printf("\n");
    } else {
        printf("\n [Erro: A tabela não foi armazenada corretamente na base de dados '%s']\n", databases[activeDatabaseIndex].name);
    }
}
/* -------------------------- YYERROR ANALISE -------------------------- */
void yyerror(const char* s)
{ 
    const char* text = yyget_text();

    // Verifica se a string contém apenas o caractere de nova linha
    if (strcmp(text, "\n") != 0) {
        fprintf(stderr, " [Erro na análise - '%s']\n", text);
    }
}
/* Definition Section*/
%{
void yyerror(const char* s);
int yylex(void);
extern char *yyget_text(void);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATABASES 100

typedef struct{
    char name[256];
} Database;

Database databases[MAX_DATABASES];
int numDatabases = 0;

void showDatabases();
void createDatabase(char* dbName);
void dropDatabase(char* dbname);
%}

%union { int num; char* id;}
%start query
%token EXTCMD LINE
%token CREATE SHOW DROP DATABASE DATABASES TABLE
%token SEMICOLON COMMA OPENPAR CLOSEPAR
%token <id> IDENTIFIER
%token <num> NUMBER

/* Rule Section */
%%

query: command SEMICOLON             { ; }
    |  EXTCMD                        { exit(EXIT_SUCCESS); }
    |  query command SEMICOLON       { ; }
    ;

command: createdb 
        |showdbs
        |dropdbs
        ;

createdb: CREATE DATABASE IDENTIFIER SEMICOLON LINE   { createDatabase($3); }
        | CREATE error LINE                           { printf(" [Query 'CREATE DATABASE' não se encontra bem construida!]\n");
                                                        printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

showdbs:  SHOW DATABASES SEMICOLON LINE               { showDatabases(); }
        | SHOW error LINE                             { printf(" [Query 'SHOW' não se encontra bem construida!]\n");
                                                        printf(" [DICA: SHOW DATABASES;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

dropdbs:  DROP DATABASE IDENTIFIER SEMICOLON LINE     { dropDatabase($3); }
        | DROP error LINE                             { printf(" [Query 'DROP' não se encontra bem construida!]\n");
                                                        printf(" [DICA: DROP DATABASE db_nome;])\n\n"); 
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
        printf("\n Bases de dados criadas:\n");
        for (int i = 0; i < numDatabases; ++i) {
            printf(" - %s\n", databases[i].name);
        }
    }
    printf("\n [ENTER] para prosseguir\n");
}
/* -------------------------- DROP DATABASES -------------------------- */
void dropDatabase(char* dbname)
{
    if(numDatabases == 0){
        printf("\n [Não existem bases de dados guardadas, utilize o camando 'CREATE' para criar uma]\n");
        printf(" [ENTER] para tentar de novo\n");
    }else{
        for(int i = 0; i < numDatabases; ++i){
            if(strcmp(databases[i].name, dbname) == 0){
                for(int j = 0; j < numDatabases-1; ++j){
                    strcpy(databases[j].name, databases[j+1].name);
                }
                numDatabases--;
                printf("\n [A base de dados '%s' foi eliminada com sucesso!]\n", dbname);
                printf(" [ENTER] para prosseguir\n");
                break;
            }else{
                printf("\n [Erro: A base de dados '%s' não foi encontrada]\n", dbname);
                printf(" [ENTER] para tentar de novo\n");
            }
        }
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
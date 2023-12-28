/* Definition Section*/
%{
void yyerror(const char* s);
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATABASES 2

typedef struct{ char name[256]; } Database;
Database databases[MAX_DATABASES];
int numDatabases = 0;
void createDatabase(char* dbName);
extern char *yyget_text(void);
%}

%union { int num; char* id;}
%start query
%token EXTCMD LINE
%token CREATE DATABASE SEMICOLON
%token <id> IDENTIFIER
%token <num> NUMBER

/* Rule Section */
%%

query: command SEMICOLON             { ; }
    |  EXTCMD                        { exit(EXIT_SUCCESS); }
    |  query command SEMICOLON       { ; }
    ;

command: createdb

createdb: CREATE DATABASE IDENTIFIER SEMICOLON LINE   { createDatabase($3); }
        | CREATE error LINE                           { printf(" [Query 'CREATE DATABASE' não se encontra bem construida!]\n");
                                                        printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo ");}
        ;

%%

void createDatabase(char* dbname) {

    for (int i = 0; i < numDatabases; ++i) {
        if (strcmp(databases[i].name, dbname) == 0) {
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

void yyerror(const char* s) { 
    const char* text = yyget_text();

    // Verifica se a string contém apenas o caractere de nova linha
    if (strcmp(text, "\n") != 0) {
        fprintf(stderr, " [Erro na análise - '%s']\n", text);
    }
}
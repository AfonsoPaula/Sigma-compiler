%{
    /* Definition Section */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    void yyerror(const char* s);

    typedef struct {
        char name[256];
    } Database;

    Database databases[100];

    int numDatabases = 0;

    void createDatabase(char* dbName);
%}

%union { char* strval; }
%token <strval> IDENTIFIER
%token CREATE DATABASE SEMICOLON NEWLINE

/* Rule Section */
%%

statement: CREATE DATABASE IDENTIFIER SEMICOLON NEWLINE { printf("Reconheceu!\n"); createDatabase($3); }
        | statement NEWLINE
        ;

%%

#include "lex.yy.c" /* ou extern int yylex() */

void createDatabase(char* dbname) {

    for (int i = 0; i < numDatabases; ++i) {
        if (strcmp(databases[i].name, dbname) == 0) {
            printf("Erro: A base de dados '%s' já existe.\n", dbname);
            return;
        }
    }

    if (numDatabases < 100) {
        strcpy(databases[numDatabases].name, dbname);
        numDatabases++;
        printf("Base de dados '%s' criada com sucesso.\n", dbname);
    } else {
        printf("Erro: Limite de bases de dados atingido.\n");
    }
}

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}
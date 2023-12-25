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
        | CREATE error LINE                           { printf("\n [Query 'CREATE DATABASE' não se encontra bem construida!]\n");
                                                        printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

showdbs:  SHOW DATABASES SEMICOLON LINE               { showDatabases(); }
        | SHOW error LINE                             { printf("\n [Query 'SHOW' não se encontra bem construida!]\n");
                                                        printf(" [DICA: SHOW DATABASES;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

dropdbs:  DROP DATABASE IDENTIFIER SEMICOLON LINE     { dropDatabase($3); }
        | DROP error LINE                             { printf("\n [Query 'DROP' não se encontra bem construida!]\n");
                                                        printf(" [DICA: DROP DATABASE db_nome;])\n\n"); 
                                                        printf(" [ENTER] para tentar de novo "); }
        ;

%%
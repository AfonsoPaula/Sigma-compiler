/* ----------------- Definition Section ------------------ */
%{
void yyerror(const char* s);
int yylex(void);
extern char *yyget_text(void);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
%}

%union { int num; char* id;}
%start query

%token EXTCMD LINE
%token CREATE SHOW DROP DATABASE DATABASES USE TABLE TABLES WHICH
%token SEMICOLON COMMA OPENPAR CLOSEPAR

%token <id> IDENTIFIER
%token <num> NUMBER

/* -------------------- Prod. and Rule Section --------------------- */
%%

query:  command SEMICOLON                                                     { ; }
     |  EXTCMD                                                                { printf("\n A sair do Sigma Compiler...\n");
                                                                                printf(" Até breve!\n");
                                                                                printf("_________________________________________________________________________________________________\n\n\n\n");
                                                                                exit(EXIT_SUCCESS); }
     |  query command SEMICOLON                                               { ; }
    ;

command: createdb 
        |showSom
        |dropdb
        |usedb
        |whichdb
        |createtb
        ;

createdb: CREATE DATABASE IDENTIFIER SEMICOLON LINE                           { createDatabase($3); }
        | CREATE DATABASE error LINE                                          { printf("\n [Query 'CREATE DATABASE' não se encontra bem construida]\n");
                                                                                printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
        | CREATE error LINE                                                   { printf(" [DICA: 'CREATE DATABASE' ou 'CREATE TABLE'?]\n\n");
                                                                                printf(" [ENTER] para tentar de novo ");}
        ;

showSom:  SHOW DATABASES SEMICOLON LINE                                       { showDatabases(); }
        | SHOW TABLES SEMICOLON LINE                                          { showTables(); }
        | SHOW error LINE                                                     { printf("\n [Query 'SHOW' não se encontra bem construida]\n");
                                                                                printf(" [DICA: 'SHOW DATABASES;' ou 'SHOW TABLES;']\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
        ;

dropdb:  DROP DATABASE IDENTIFIER SEMICOLON LINE                              { dropDatabase($3); }
        |DROP error LINE                                                      { printf("\n [Query 'DROP' não se encontra bem construida]\n");
                                                                                printf(" [DICA: DROP DATABASE db_nome;])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
        ;

usedb: USE IDENTIFIER SEMICOLON LINE                                          { useDatabase($2); }
    |  USE error LINE                                                         { printf("\n [Query 'USE' não se encontra bem construida]\n");
                                                                                printf(" [DICA: USE db_nome;])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
    ;

whichdb: WHICH DATABASE SEMICOLON LINE                                        { whichdb(); }
        |WHICH error                                                          { printf("\n [Query 'WHICH' não se encontra bem construida]\n");
                                                                                printf("\n [DICA: WHICH DATABASE;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo "); }


createtb: CREATE TABLE IDENTIFIER OPENPAR NUMBER CLOSEPAR SEMICOLON LINE      { createTable($3,$5); }
        | CREATE TABLE error LINE                                             { printf("\n [Query 'CREATE TABLE' não está bem construida]\n");
                                                                                printf(" [DICA: CREATE TABLE table_name (numColumns);]\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo "); }
        ;
%%

/* -------------------------- YYERROR ANALISE ------------------------- */
void yyerror(const char* s)
{ 
    const char* text = yyget_text();

    // Verifica se a string contém apenas o caractere de nova linha
    if (strcmp(text, "\n") != 0) {
        fprintf(stderr, " [Erro na análise - '%s']\n", text);
    }
}
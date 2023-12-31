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
%token CREATE SHOW DROP DATABASE DATABASES USE TABLE TABLES WHICH INSERT VALUES SELECT DELETE FROM ROW COL UPDATE
%token SEMICOLON COMMA OPENPAR CLOSEPAR

%token <id> IDENTIFIER
%token <num> NUMBER

/* ---------------- Prod. and Rule Section --------------- */
%%

query: query command                                                          { ; }
     | command                                                                { ; }
     | EXTCMD                                                                 { printf("\n A sair do Sigma Compiler...\n");
                                                                                printf(" Até breve!\n");
                                                                                printf("_________________________________________________________________________________________________\n\n\n\n");
                                                                                exit(EXIT_SUCCESS); }
     ;

command: show 
       | usedb
       | whichdb
       | createdb
       | drop
       | createtb
       | insertvals
       | selecttb
       | delete
       | rowupdate
       ;

show: SHOW DATABASES SEMICOLON LINE                                           { showDatabases(); }
    | SHOW TABLES SEMICOLON LINE                                              { showTables(); }
    | SHOW error LINE                                                         { printf("\n [Query 'SHOW' não se encontra bem construida]\n");
                                                                                printf(" [DICA: 'SHOW DATABASES;' ou 'SHOW TABLES;']\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo \n"); }
    ;

usedb: USE IDENTIFIER SEMICOLON LINE                                          { useDatabase($2); }
     | USE error LINE                                                         { printf("\n [Query 'USE' não se encontra bem construida]\n");
                                                                                printf(" [DICA: USE db_nome;])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo \n"); }
     ;

whichdb: WHICH DATABASE SEMICOLON LINE                                        { whichDatabase(); }
       | WHICH error                                                          { printf("\n [Query 'WHICH' não se encontra bem construida]\n");
                                                                                printf(" [DICA: WHICH DATABASE;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n"); }
       ;

createdb: CREATE DATABASE IDENTIFIER SEMICOLON LINE                           { createDatabase($3); }
        | CREATE DATABASE error LINE                                          { printf("\n [Query 'CREATE DATABASE' não se encontra bem construida]\n");
                                                                                printf(" [DICA: CREATE DATABASE db_nome;])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo \n"); }
        | CREATE error LINE                                                   { printf("\n [Query CREATE incompleta]\n");
                                                                                printf(" [DICA: 'CREATE DATABASE db_name;' ou 'CREATE TABLE tb_name;'?]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
        ;

drop: DROP DATABASE IDENTIFIER SEMICOLON LINE                                 { dropDatabase($3); }
    | DROP TABLE IDENTIFIER SEMICOLON LINE                                    { dropTable($3); }
    | DROP error LINE                                                         { printf("\n [Query 'DROP' não se encontra bem construida]\n");
                                                                                printf(" [DICA: 'DROP DATABASE db_name;' ou 'DROP TABLE tableName'])\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo \n"); }
    ;

createtb: CREATE TABLE IDENTIFIER OPENPAR NUMBER CLOSEPAR SEMICOLON LINE      { createTable($3,$5); }
        | CREATE TABLE error LINE                                             { printf("\n [Query 'CREATE TABLE' não se encontra bem construida]\n");
                                                                                printf(" [DICA: CREATE TABLE table_name (numColumns);]\n\n"); 
                                                                                printf(" [ENTER] para tentar de novo \n"); }
        ;

insertvals: INSERT IDENTIFIER VALUES SEMICOLON LINE                           { insertIntoTable($2); }
          | INSERT error LINE                                                 { printf("\n [Query 'INSERT INTO' não se encontra bem construida]\n");
                                                                                printf(" [DICA: INSERT INTO tableName VALUES;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
          ;

selecttb: SELECT IDENTIFIER SEMICOLON LINE                                    { selectTable($2); }
        | SELECT error LINE                                                   { printf("\n [Query 'SELECT' não se encontra bem construida]\n");
                                                                                printf(" [DICA: SELECT tableName;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
        ;

delete: DELETE ROW NUMBER FROM IDENTIFIER SEMICOLON LINE                      { deleteRowFromTable($5,$3); }
      | DELETE COL IDENTIFIER FROM IDENTIFIER SEMICOLON LINE                  { deleteColFromTable($5,$3); }
      | DELETE ROW error LINE                                                 { printf("\n [Query 'DELETE ROW' não se encontra bem construida]\n");
                                                                                printf(" [DICA: DELETE ROW numRow FROM tableName;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
      | DELETE COL error LINE                                                 { printf("\n [Query 'DELETE COL' não se encontra bem construida]\n");
                                                                                printf(" [DICA: DELETE COL colName FROM tableName;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
      | DELETE error LINE                                                     { printf("\n [Query DELETE incompleta]\n");
                                                                                printf(" [DICA: 'DELETE ROW' ou 'DELETE COL'?]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
      ;

rowupdate: UPDATE ROW NUMBER FROM IDENTIFIER SEMICOLON LINE                   { updateRow($5, $3); }
         | UPDATE error LINE                                                  { printf("\n [Query UPDATE incompleta]\n");
                                                                                printf(" [DICA: UPDATE ROW numRow FROM tableName;]\n\n");
                                                                                printf(" [ENTER] para tentar de novo \n");}
         ;

%%

/* ------------------- YYERROR ANALISE ------------------- */
void yyerror(const char* s)
{ 
    const char* text = yyget_text();

    // Verifica se a string contém apenas o caractere de nova linha
    if (strcmp(text, "\n") != 0) {
        fprintf(stderr, " [Erro na análise - '%s']\n", text);
    }
}
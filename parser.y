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
%token CREATE SHOW DROP DATABASE DATABASES USE TABLE TABLES WHICH INSERT VALUES SELECT DELETE FROM ROW COL UPDATE WHERE FOR
%token MAIGUAL MEIGUAL IGUAL MAIOR MENOR DIFF
%token SEMICOLON COMMA OPENPAR CLOSEPAR APTS

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
       | forcicle
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

selecttb: SELECT IDENTIFIER SEMICOLON LINE                                                { selectTable($2); }
        | SELECT IDENTIFIER WHERE IDENTIFIER IGUAL NUMBER SEMICOLON LINE                  { selectIgualNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER IGUAL APTS IDENTIFIER APTS SEMICOLON LINE    { selectIgualStr($2,$4,$7); }
        | SELECT IDENTIFIER WHERE IDENTIFIER MAIGUAL NUMBER SEMICOLON LINE                { selectMaiorIgualNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER MEIGUAL NUMBER SEMICOLON LINE                { selectMenorIgualNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER MAIOR NUMBER SEMICOLON LINE                  { selectMaiorNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER MENOR NUMBER SEMICOLON LINE                  { selectMenorNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER DIFF NUMBER SEMICOLON LINE                   { selectDiffNum($2,$4,$6); }
        | SELECT IDENTIFIER WHERE IDENTIFIER DIFF APTS IDENTIFIER APTS SEMICOLON LINE     { selectDiffStr($2,$4,$7); }
        | SELECT error LINE                                                               { printf("\n [Query 'SELECT' não se encontra bem construida]\n");
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

forcicle: FOR OPENPAR IDENTIFIER COMMA IDENTIFIER COMMA NUMBER COMMA NUMBER CLOSEPAR SEMICOLON LINE   { printf("Reconheceu!\n"); }
        | FOR error LINE                                                                              { printf("\n [Query FOR incompleta]\n");
                                                                                                        printf(" [DICA: FOR (tableName, colName, startRow, numberLines]\n\n");
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
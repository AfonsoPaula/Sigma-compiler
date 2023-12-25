#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int main() {
    printf("_________________________________________________________________________________________________\n\n");
    printf(":::::::::::::::::::::::::::::::::::: Bem-vindo ao SQL Compiler ::::::::::::::::::::::::::::::::::\n\n");
    printf("[Insira as suas queries]                                                  Digite [exit] para sair \n");
    printf("_________________________________________________________________________________________________\n\n");

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
    /* -------------------------- YYERROR ANALISE ------------------------- */
    void yyerror(const char* s)
{ 
    const char* text = yyget_text();

    // Verifica se a string contém apenas o caractere de nova linha
    if (strcmp(text, "\n") != 0) {
        fprintf(stderr, " [Erro na análise - '%s']\n", text);
    }
}

    while (1) {
        printf("\n              ");
        yyparse();
    }

    return 0;
}
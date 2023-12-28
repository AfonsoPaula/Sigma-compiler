#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "parser.tab.h"

// Definições para limites de BD, tabelas e colunas
#define MAX_DATABASES     100
#define MAX_TABLES        100
#define MAX_COLUMNS        50
#define MAX_ROWS          100
#define MAX_TABLES_PER_DB  50

// Variável que segue a BD ativa
int activeDatabaseIndex = -1;

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

/* -------------------------- CREATE DATABASE ------------------------- */
void createDatabase(char* dbname) 
{
    for (int i = 0; i < numDatabases; ++i) {
        if(strcmp(databases[i].name, dbname) == 0){
            printf("\n [Erro: A base de dados '%s' já existe]\n\n", dbname);
            printf(" [ENTER] para tentar de novo\n");
            return;
        }
    }
    if (numDatabases < MAX_DATABASES) {
        strcpy(databases[numDatabases].name, dbname);
        numDatabases++;
        printf("\n [Base de dados '%s' criada com sucesso!]\n\n", dbname);
        printf(" [ENTER] para prosseguir\n");
    } else {
        printf("\n [Erro: Limite de bases de dados atingido]\n\n");
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
/* --------------------------- USE DATABASE --------------------------- */
void useDatabase(char* dbname)
{
    int found = 0;

    for (int i = 0; i < numDatabases; ++i) {
        if (strcmp(databases[i].name, dbname) == 0) {
            activeDatabaseIndex = i;
            found = 1;
            printf("\n [Base de dados '%s' (index:[%i]) ativa]\n", dbname, i);
            printf(" [A partir de agora, você pode criar tabelas nesta base de dados]\n\n");
            printf(" [ENTER] para prosseguir\n");
            break;
        }
    }

    if (!found) {
        printf("\n [Erro: A base de dados '%s' não foi encontrada]\n", dbname);
        printf(" [ENTER] para tentar de novo\n");
    }
}
/* -------------------------- WHICH DATABASE -------------------------- */
void whichdb()
{
    if(activeDatabaseIndex == -1){
        printf("\n [Até ao momento nenhuma base de dados ativa]\n");
        printf(" [DICA: 'USE db_name;' Para poder ativar uma BD]\n\n");
        printf(" [ENTER] para tentar de novo ");
    }else{
        printf("\n [Base de dados '%s' ativa de momento]\n\n", databases[activeDatabaseIndex].name);
        printf(" [ENTER] para prosseguir ");
    }
}
/* --------------------------- CREATE TABLE --------------------------- */
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


int main() {
    printf("_________________________________________________________________________________________________\n\n");
    printf(":::::::::::::::::::::::::::::::::: Bem-vindo ao Sigma Compiler ::::::::::::::::::::::::::::::::::\n\n");
    printf("[Insira as suas queries]                                                  Digite [exit] para sair \n");
    printf("_________________________________________________________________________________________________\n\n");

    while (1) {
        printf("\n              ");
        yyparse();
    }

    return 0;
}
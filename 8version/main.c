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

// Variáveis que seguem as BD e TB ativas
int activeDatabaseIndex = -1;
int activeTableIndex    = -1;

/* ----------------------- TABLES ------------------------ */
typedef struct {
    char name[256];
    char columns[MAX_COLUMNS][256];
    int  numColumns;
    char data[MAX_ROWS][MAX_COLUMNS][256];
    int  numRows;
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
        printf("\n [Nenhuma base de dados foi criada ainda]\n\n");
    } else {
        printf("\n Bases de dados armazenadas:\n");
        for (int i = 0; i < numDatabases; ++i) {
            printf(" |\n");
            printf(" ├──  [%d] %s;\n", i, databases[i].name);
        }
    }
    printf("\n [ENTER] para prosseguir\n");
}
/* -------------------------- DROP DATABASES -------------------------- */
void dropDatabase(char* dbname)
{
    // Caso a base de dados esteja a ser usada (não deixa eliminar)
    if(strcmp(databases[activeDatabaseIndex].name, dbname) == 0){
        printf("\n [Encontra-se com a BD '%s' ativa de momento]\n", dbname);
        printf(" [DICA: Selecione outra BD para poder eliminar '%s']\n\n", dbname);
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Confere se não existem bases de dados
    if (numDatabases == 0) {
        printf("\n [Não existem bases de dados guardadas]\n");
        printf(" [DICA: Utilize o comando 'CREATE' para criar uma]\n\n");
        printf(" [ENTER] para tentar de novo\n");
    } else {
        int found = 0;
        for (int i = 0; i < numDatabases; ++i) {
            // Mover até à última posição e diminuir 1 ao nº de BDs
            if (strcmp(databases[i].name, dbname) == 0) {
                // Confirmar a opção do utilizador
                char response;
                printf("\n Tem a certeza de que deseja eliminar a base de dados '%s'? (S/N): ", databases[i].name);
                scanf(" %c", &response);
                // Caso a resposta tenha sido 'sim' (elimina)
                if (response == 'S' || response == 's') {
                    for (int j = i; j < numDatabases - 1; ++j) {
                        strcpy(databases[j].name, databases[j + 1].name);
                    }
                    numDatabases--;
                    found = 1;
                    printf("\n [A base de dados '%s' foi eliminada com sucesso!]\n\n", dbname);
                    printf(" [ENTER] para prosseguir\n");
                }else{
                    printf("\n [Operação de eliminação cancelada]\n");
                    return;
                }
                break;
            }
        }
        // Caso não tenha sido encontrada
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
void whichDatabase()
{
    if(activeDatabaseIndex == -1){
        printf("\n [Até ao momento nenhuma base de dados ativa]\n");
        printf(" [DICA: 'USE DATABASE db_name;' Para poder ativar uma BD]\n\n");
        printf(" [ENTER] para tentar de novo\n");
    }else{
        printf("\n [Base de dados '%s' ativa de momento]\n\n", databases[activeDatabaseIndex].name);
        printf(" [ENTER] para prosseguir\n");
    }
}



/* --------------------------- CREATE TABLE --------------------------- */
void createTable(char* tableName, int numColumns)
{
    char columnName[256];

    // Verificar se há uma base de dados ativa
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma base de dados ativa]\n");
        printf(" [DICA: Utilize a query 'USE' para ativar uma BD]\n\n");
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
/* --------------------------- SHOW TABLES ---------------------------- */
void showTables()
{
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma BD ativa]\n");
        printf(" [DICA: Use 'USE db_name;' para ativar uma base de dados]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    if (databases[activeDatabaseIndex].numTables == 0) {
        printf("\n [Nenhuma tabela na base de dados '%s']\n", databases[activeDatabaseIndex].name);
    } else {
        printf("\n Tabelas na base de dados '%s':\n", databases[activeDatabaseIndex].name);
        for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
            printf(" |\n");
            printf(" ├── [%d] %s;\n", i, databases[activeDatabaseIndex].tables[i].name);
        }
    }
    printf("\n [ENTER] para prosseguir\n");
}
/* ---------------------------- USE TABLE ----------------------------- */
void useTable(char* tableName)
{
    int found = 0;

    // Confirma se existe uma base de dados ativa
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma base de dados ativa]\n");
        printf(" [DICA: Use 'USE db_name;' para ativar uma base de dados]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Procura pela tabela na base de dados ativa
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            activeTableIndex = i;
            found = 1;
            printf("\n [Tabela '%s' (index:[%i]) ativa]\n", tableName, i);
            printf(" [A partir de agora, você pode realizar operações nesta tabela]\n\n");
            printf(" [ENTER] para prosseguir\n");
            break;
        }
    }

    // Mensagem de erro se a tabela não for encontrada
    if (!found) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
    }
}
/* --------------------------- WHICH TABLE ---------------------------- */
void whichTable()
{
    if (activeTableIndex == -1) {
        printf("\n [Até ao momento nenhuma tabela ativa]\n");
        printf(" [DICA: 'USE TABLE table_name;' para poder ativar uma tabela]\n\n");
        printf(" [ENTER] para tentar de novo\n");
    } else {
        printf("\n [Tabela '%s' ativa de momento]\n\n", databases[activeDatabaseIndex].tables[activeTableIndex].name);
        printf(" [ENTER] para prosseguir\n");
    }
}
/* ------------------------ INSERT INTO TABLE- ------------------------ */
void insertIntoTable(char* tableName)
{
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma base de dados ativa]\n");
        printf(" [DICA: Use 'USE db_name;' para ativar uma base de dados]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    int tableIndex = -1;
    // Procura pela tabela na base de dados ativa
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            tableIndex = i;
            break;
        }
    }

    // Verifica se a tabela foi encontrada
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
    }

    // Verifica se há espaço para mais uma linha
    if (databases[activeDatabaseIndex].tables[tableIndex].numRows >= MAX_ROWS) {
        printf("\n [Erro: Limite de linhas atingido para a tabela '%s']\n\n", tableName);
        // printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Preenche os valores da nova linha
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf("\n [Digite o valor para a coluna '%s']: ", databases[activeDatabaseIndex].tables[tableIndex].columns[i]);
        scanf("%s", databases[activeDatabaseIndex].tables[tableIndex].data[databases[activeDatabaseIndex].tables[tableIndex].numRows][i]);
    }

    // Incrementa o número de linhas na tabela
    databases[activeDatabaseIndex].tables[tableIndex].numRows++;

    printf("\n\n [Valores inseridos com sucesso na tabela '%s']\n\n", tableName);
    // printf(" [ENTER] para prosseguir\n");
    return;
}
/* -------------------------- SELECT TABLE ---------------------------- */
void selectTable(char* tableName)
{
    if (activeDatabaseIndex == -1) {
        printf("\n [Erro: Nenhuma base de dados ativa]\n");
        printf(" [DICA: Use 'USE db_name;' para ativar uma base de dados]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    int tableIndex = -1;
    // Procura pela tabela na base de dados ativa
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            tableIndex = i;
            break;
        }
    }

    // Verifica se a tabela foi encontrada
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Exibe o cabeçalho da tabela
    printf("\n Tabela: %s\n\n", databases[activeDatabaseIndex].tables[tableIndex].name);
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf("+---------------------");
    }
    printf("+\n");

    // Exibe os nomes das colunas
    printf("|");
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].columns[i]);
    }
    printf("\n");

    // Exibe a linha horizontal separadora
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf("+---------------------");
    }
    printf("+\n");

    // Exibe os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        printf("|");
        for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
            printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
        }
        printf("\n");
    }

    // Linha final
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf("+---------------------");
    }
    printf("+\n");

    printf("\n [ENTER] para prosseguir\n");
}







int main() {
    printf("_________________________________________________________________________________________________\n\n");
    printf(":::::::::::::::::::::::::::::::::: Bem-vindo ao Sigma Compiler ::::::::::::::::::::::::::::::::::\n\n");
    printf("[Insira as suas queries]                                                  Digite [exit] para sair \n");
    printf("_________________________________________________________________________________________________\n\n");

    while (1) {
        printf("\n              ");
        yyparse();
        printf("=================================================================================================\n");
    }

    return 0;
}
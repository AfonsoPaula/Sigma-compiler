#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "parser.tab.h"

// Definições para limites de BD, tabelas e colunas
#define MAX_DATABASES      50
#define MAX_TABLES         50
#define MAX_COLUMNS        50
#define MAX_ROWS          100

// Variável que segue a BD ativa
int activeDatabaseIndex = -1;

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
    Table tables[MAX_TABLES];
    int numTables;
} Database;

Database databases[MAX_DATABASES];
int numDatabases = 0;

/* ==================================================================== */

/* -------------------------- CREATE DATABASE ------------------------- */
void createDatabase(char* dbname) 
{
    // Verificar se a BD já existe
    for (int i = 0; i < numDatabases; ++i) {
        if(strcmp(databases[i].name, dbname) == 0){
            printf("\n [Erro: A base de dados '%s' já existe]\n\n", dbname);
            printf(" [ENTER] para tentar de novo\n");
            return;
        }
    }
    // Verificar se o número max de BDs já foi atingido
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
    // Verificar se existe alguma BD no sistema
    if (numDatabases == 0) {
        printf("\n [Nenhuma base de dados armazenada]\n");
        printf(" [DICA: 'CREATE DATABASE db_name;' para criar uma]\n");
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
                    printf(" [ENTER] para prosseguir\n\n");
                }else{
                    printf("\n [Operação de eliminação cancelada]\n\n");
                    return;
                }
                break;
            }
        }
        // Caso não tenha sido encontrada
        if (!found) {
            printf("\n [Erro: A base de dados '%s' não foi encontrada]\n\n", dbname);
            printf(" [ENTER] para tentar de novo\n");
        }
    }
}
/* --------------------------- USE DATABASE --------------------------- */
void useDatabase(char* dbname)
{
    int found = 0;

    // Verificar se a base de dados já está ativa
    if (activeDatabaseIndex != -1 && strcmp(databases[activeDatabaseIndex].name, dbname) == 0) {
        printf("\n [Aviso: Base de dados '%s' já está ativa]\n\n", dbname);
        printf(" [ENTER] para tentar de novo\n");
        return;
    }
    // Encontrar a BD no sistema e ativá-la
    for (int i = 0; i < numDatabases; ++i) {
        if (strcmp(databases[i].name, dbname) == 0) {
            activeDatabaseIndex = i;
            found = 1;
            printf("\n [Base de dados '%s' (index:[%i]) ativa]\n", dbname, i);
            printf(" [A partir de agora pode criar tabelas nesta base de dados]\n\n");
            printf(" [ENTER] para prosseguir\n");
            break;
        }
    }
    // Caso não seja encontrada no sistema
    if (!found) {
        printf("\n [Erro: Base de dados '%s' não foi encontrada]\n\n", dbname);
        printf(" [ENTER] para tentar de novo\n");
    }
}
/* -------------------------- WHICH DATABASE -------------------------- */
void whichDatabase()
{
    // Caso nenhuma BD esteja ativa no sistema
    if(activeDatabaseIndex == -1){
        printf("\n [Até ao momento nenhuma base de dados ativa]\n");
        printf(" [DICA: 'USE DATABASE db_name;' para poder ativar uma BD]\n\n");
        printf(" [ENTER] para tentar de novo\n");
    }else{
        printf("\n [Base de dados '%s' ativa de momento]\n\n", databases[activeDatabaseIndex].name);
        printf(" [ENTER] para prosseguir\n");
    }
}

/* ==================================================================== */

/* --------------------------- CREATE TABLE --------------------------- */
void createTable(char* tableName, int numColumns)
{
    char columnName[256];

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se o número inserido é válido
    if (numColumns <= 0 || numColumns > MAX_COLUMNS) {
        printf("\n [Número inválido de colunas (entre 0 e 50)]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Verificar se a tabela já existe na BD ativa
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            printf("\n [Erro: A tabela '%s' já existe na BD '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
            printf(" [ENTER] para tentar de novo\n");
            return;
        }
    }

    // Verificar se o limite de tabelas foi atingido na base de dados ativa
    if (databases[activeDatabaseIndex].numTables >= MAX_TABLES) {
        printf("\n [Erro: Limite de tabelas atingido na base de dados '%s']\n\n", databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Criar a nova tabela
    Table newTable;
    strcpy(newTable.name, tableName);
    newTable.numColumns = numColumns;

    // Adicionar o nome de cada coluna
    for (int i = 0; i < numColumns; ++i) {
        printf("\n [Digite o nome da coluna %d]: ", i+1);
        scanf("%s", columnName);
        strcpy(newTable.columns[i], columnName);
    }

    // Adicionar a nova tabela à base de dados ativa
    databases[activeDatabaseIndex].tables[databases[activeDatabaseIndex].numTables] = newTable;
    databases[activeDatabaseIndex].numTables++;

    // Mostrar a confirmação e o resumo ao utilizador 
    printf("\n Tabela '%s' criada com sucesso na base de dados '%s' com as seguintes colunas:\n", tableName, databases[activeDatabaseIndex].name);
    for (int i = 0; i < newTable.numColumns; ++i) {
        printf("  -> %s\n", newTable.columns[i]);
    }
    printf("\n");
}
/* --------------------------- SHOW TABLES ---------------------------- */
void showTables()
{
    if (!checkActiveDatabase()) {
        return;
    }

    // Caso não não haja nenhuma tabela na BD ativa
    if (databases[activeDatabaseIndex].numTables == 0) {
        printf("\n [Nenhuma tabela na base de dados '%s']\n", databases[activeDatabaseIndex].name);
        printf(" [DICA: 'CREATE TABLE tableName (numColumns); para criar uma]\n");
    } else {
        printf("\n Tabelas na base de dados '%s':\n", databases[activeDatabaseIndex].name);
        // Printar cada tabela da BD ativa
        for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
            printf(" |\n");
            printf(" ├── [%d] %s (", i, databases[activeDatabaseIndex].tables[i].name);
            // Printar cada coluna da tabela em questão
            for (int j = 0; j < databases[activeDatabaseIndex].tables[i].numColumns; ++j) {
                printf("%s", databases[activeDatabaseIndex].tables[i].columns[j]);
                // Adicionar vírgula se não for a última coluna
                if (j < databases[activeDatabaseIndex].tables[i].numColumns - 1) {
                    printf(", ");
                }
            }
            printf(");\n");
        }
    }
    printf("\n [ENTER] para prosseguir\n");
}
/* ------------------------ INSERT INTO TABLE- ------------------------ */
void insertIntoTable(char* tableName)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Verificar se há espaço para mais uma linha
    if (databases[activeDatabaseIndex].tables[tableIndex].numRows >= MAX_ROWS) {
        printf("\n [Erro: Limite de linhas atingido para a tabela '%s']\n\n", tableName);
        return;
    }

    // Preencher os valores para a nova linha
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf("\n Digite o valor para a coluna '%s': ", databases[activeDatabaseIndex].tables[tableIndex].columns[i]);
        scanf("%s", databases[activeDatabaseIndex].tables[tableIndex].data[databases[activeDatabaseIndex].tables[tableIndex].numRows][i]);
    }

    // Incrementar o número de linhas na tabela
    databases[activeDatabaseIndex].tables[tableIndex].numRows++;

    printf("\n\n [Valores inseridos com sucesso na linha %d da tabela '%s']\n\n", databases[activeDatabaseIndex].tables[tableIndex].numRows, tableName);
    return;
}
/* -------------------------- SELECT TABLE ---------------------------- */
void selectTable(char* tableName)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    printTableHeader(tableIndex);
    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        printf("| %-19d |", row + 1);
        for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
            printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
        }
        printf("\n");
    }
    printTableFooter(tableIndex);

    printf("\n [ENTER] para prosseguir\n");
}
/* --------------------------- DELETE ROW ----------------------------- */
void deleteRowFromTable(char* tableName, int rowNum)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Verificar se o número da linha fornecido é válido
    if (rowNum <= 0 || rowNum > databases[activeDatabaseIndex].tables[tableIndex].numRows) {
        printf("\n [Erro: Número de linha inválido]\n");
        printf(" [DICA: 'SELECT tableName;' para visualizar a tabela]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Exibir os dados da linha que será excluída
    printf("\n [Dados da linha %d da tabela '%s']:\n\n", rowNum, tableName);
    for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
        printf("   %s: %s\n", databases[activeDatabaseIndex].tables[tableIndex].columns[col], databases[activeDatabaseIndex].tables[tableIndex].data[rowNum - 1][col]);
    }

    // Confirmar se pretende eliminar a linha
    char response;
    printf("\n Tem a certeza de que deseja eliminar esta linha? (S/N): ");
    scanf(" %c", &response);

    if (response == 'S' || response == 's') {
        // Remover a linha da tabela
        for (int i = rowNum - 1; i < databases[activeDatabaseIndex].tables[tableIndex].numRows - 1; ++i) {
            for (int j = 0; j < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++j) {
                strcpy(databases[activeDatabaseIndex].tables[tableIndex].data[i][j], databases[activeDatabaseIndex].tables[tableIndex].data[i + 1][j]);
            }
        }

        // Decrementar o número de linhas na tabela
        databases[activeDatabaseIndex].tables[tableIndex].numRows--;

        printf("\n [Linha %d da tabela '%s' eliminada com sucesso!]\n\n", rowNum, tableName);
    } else {
        printf("\n [Operação de eliminação cancelada]\n\n");
    }
}
/* --------------------------- DELETE COL ----------------------------- */
void deleteColFromTable(char* tableName, char* columnName)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    // Confirmar com o utilizador a exclusão da coluna
    char response;
    printf("\n Tem a certeza de que deseja eliminar a coluna '%s' da tabela '%s'? (S/N): ", columnName, tableName);
    scanf(" %c", &response);

    if (response == 'S' || response == 's') {
        // Remover a coluna da tabela
        for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++i) {
            for (int j = columnIndex; j < databases[activeDatabaseIndex].tables[tableIndex].numColumns - 1; ++j) {
                strcpy(databases[activeDatabaseIndex].tables[tableIndex].data[i][j], databases[activeDatabaseIndex].tables[tableIndex].data[i][j + 1]);
            }
        }

        // Decrementar o número de colunas na tabela
        databases[activeDatabaseIndex].tables[tableIndex].numColumns--;

        // Remover o nome da coluna do array de colunas
        for (int i = columnIndex; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
            strcpy(databases[activeDatabaseIndex].tables[tableIndex].columns[i], databases[activeDatabaseIndex].tables[tableIndex].columns[i + 1]);
        }

        printf("\n [Coluna '%s' da tabela '%s' eliminada com sucesso!]\n\n", columnName, tableName);
    } else {
        printf("\n [Operação de eliminação cancelada]\n\n");
    }
}
/* --------------------------- UPDATE ROW ----------------------------- */
void updateRow(char* tableName, int rowNum)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Verificar se o número da linha fornecido é válido
    if (rowNum <= 0 || rowNum > databases[activeDatabaseIndex].tables[tableIndex].numRows) {
        printf("\n [Erro: Número de linha inválido]\n");
        printf(" [DICA: 'SELECT tableName;' para visualizar a tabela]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return;
    }

    // Exibir os dados da linha que será atualizada
    printf("\n [Dados da linha %d da tabela '%s']:\n", rowNum, tableName);
    for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
        printf("   %s: %s\n", databases[activeDatabaseIndex].tables[tableIndex].columns[col], databases[activeDatabaseIndex].tables[tableIndex].data[rowNum - 1][col]);
    }

    // Solicitar ao usuário para fornecer os novos valores
    printf("\n [Insira os novos valores para a linha %d da tabela '%s']:\n", rowNum, tableName);
    for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
        printf("   %s: ", databases[activeDatabaseIndex].tables[tableIndex].columns[col]);
        scanf("%s", databases[activeDatabaseIndex].tables[tableIndex].data[rowNum - 1][col]);
    }

    printf("\n [Linha %d da tabela '%s' atualizada com sucesso!]\n\n", rowNum, tableName);
}
/* --------------------------- DROP TABLE ----------------------------- */
void dropTable(char* tableName)
{
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Confirmar com o utilizador a exclusão da tabela
    char response;
    printf("\n Tem a certeza de que deseja eliminar a tabela '%s'? (S/N): ", tableName);
    scanf(" %c", &response);

    if (response == 'S' || response == 's') {
        // Remover a tabela da base de dados
        for (int i = tableIndex; i < databases[activeDatabaseIndex].numTables - 1; ++i) {
            databases[activeDatabaseIndex].tables[i] = databases[activeDatabaseIndex].tables[i + 1];
        }

        // Decrementar o número de tabelas na base de dados
        databases[activeDatabaseIndex].numTables--;

        printf("\n [A tabela '%s' foi eliminada com sucesso!]\n\n", tableName);
        
    } else {
        printf("\n [Operação de eliminação cancelada]\n\n");
    }
}
/* ----------------------- SELECT IGUAL NUMBER ------------------------ */
void selectIgualNum(char* tableName, char* columnName, int number) {
    
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) == number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");

}
/* ----------------------- SELECT IGUAL STRING ------------------------ */
void selectIgualStr(char* tableName, char* columnName, char* searchString) {
    
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(strcmp(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex], searchString) == 0){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");

}
/* -------------------- SELECT MAIOR/IGUAL NUMBER --------------------- */
void selectMaiorIgualNum(char* tableName, char* columnName, int number) {

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) >= number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");
}
/* -------------------- SELECT MENOR/IGUAL NUMBER --------------------- */
void selectMenorIgualNum(char* tableName, char* columnName, int number) {

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) <= number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");
}
/* ----------------------- SELECT MAIOR NUMBER ------------------------ */
void selectMaiorNum(char* tableName, char* columnName, int number) {

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }

    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) > number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");
}
/* ----------------------- SELECT MENOR NUMBER ------------------------ */
void selectMenorNum(char* tableName, char* columnName, int number) {

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) < number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");
}
/* ----------------------- SELECT DIFF NUMBER ------------------------- */
void selectDiffNum(char* tableName, char* columnName, int number) {

    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(atoi(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex]) != number){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");
}
/* ----------------------- SELECT DIFF STRING ------------------------- */
void selectDiffStr(char* tableName, char* columnName, char* searchString) {
    
    // Verificar se há uma base de dados ativa
    if (!checkActiveDatabase()) {
        return;
    }

    // Verificar se a tabela foi encontrada
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("\n [Erro: A tabela '%s' não foi encontrada na base de dados '%s']\n\n", tableName, databases[activeDatabaseIndex].name);
        printf(" [ENTER] para tentar de novo\n");
        // A tabela não foi encontrada, a execução não deve continuar
        return;
    }
    
    // Verificar se a coluna fornecida existe na tabela
    int columnIndex = findColumnIndex(tableIndex, columnName);
    if (columnIndex == -1) {
        printf("\n [Erro: A coluna '%s' não foi encontrada na tabela '%s']\n", columnName, tableName);
        printf("\n [ENTER] para tentar de novo\n");
        return;
    }

    printTableHeader(tableIndex);

    // Variável de controle para verificar se pelo menos uma linha foi encontrada
    int linhasEncontradas = 0;

    // Exibir os dados da tabela
    for (int row = 0; row < databases[activeDatabaseIndex].tables[tableIndex].numRows; ++row) {
        if(strcmp(databases[activeDatabaseIndex].tables[tableIndex].data[row][columnIndex], searchString) != 0){
            linhasEncontradas++;
            printf("| %-19d |", row + 1);
            for (int col = 0; col < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++col) {
                printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].data[row][col]);
            }
            printf("\n");
        }
    }

    printTableFooter(tableIndex);

    // Se nenhuma linha foi encontrada, emitir um aviso
    if (linhasEncontradas == 0) {
        printf("\n [Aviso: Nenhum resultado encontrado para a condição fornecida]\n");
    }

    printf("\n [ENTER] para prosseguir\n");

}

/* ==================================================================== */

//                Verificar se há uma base de dados ativa
int isDatabaseActive() {
    return activeDatabaseIndex != -1;
}
//                 Lidar com a verificação da bd ativa
int checkActiveDatabase() {
    if (!isDatabaseActive()) {
        printf("\n [Erro: Nenhuma BD ativa]\n");
        printf(" [DICA: Use 'USE db_name;' para ativar uma base de dados]\n\n");
        printf(" [ENTER] para tentar de novo\n");
        return 0;
    } else {
        printf("\n. A utilizar a base de dados '%s'\n", databases[activeDatabaseIndex].name);
        return 1;
    }
}
//               Procurar uma tabela na base de dados ativa
int findTableIndex(const char* tableName) {

    int tableIndex = -1;
    // Procurar pela tabela na base de dados ativa
    for (int i = 0; i < databases[activeDatabaseIndex].numTables; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[i].name, tableName) == 0) {
            tableIndex = i;
            break;
        }
    }

    // Verificar se a tabela foi encontrada
    if (tableIndex == -1) {
        // Retorna -1 indicando que a tabela não foi encontrada
        return -1; 
    }
    // Retorna o índice da tabela se ela for encontrada
    return tableIndex;
}
//                 Procurar uma coluna numa dada tabela
int findColumnIndex(int tableIndex, const char* columnName) {

    int columnIndex = -1;
    // Procurar pela coluna na tabela
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        if (strcmp(databases[activeDatabaseIndex].tables[tableIndex].columns[i], columnName) == 0) {
            columnIndex = i;
            break;
        }
    }

    // Verificar se a coluna foi encontrada
    if (columnIndex == -1) {
        // Retorna -1 indicando que a coluna não foi encontrada
        return -1;
    }
    // Retorna o índice da coluna se ela for encontrada
    return columnIndex;
}
//                     Printa o cabeçalho da tabela
void printTableHeader(int tableIndex) {
    // Verificar se a tabela foi encontrada
    if (tableIndex == -1) {
        printf("\n [Erro: Tabela não encontrada]\n");
        return;
    }

    // Exibir o cabeçalho da tabela
    printf("\n Tabela: %s\n\n", databases[activeDatabaseIndex].tables[tableIndex].name);
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns + 1; ++i) {
        printf("+---------------------");
    }
    printf("+\n");

    // Exibir os nomes das colunas
    printf("| %-19s |", "numRow");
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns; ++i) {
        printf(" %-19s |", databases[activeDatabaseIndex].tables[tableIndex].columns[i]);
    }
    printf("\n");

    // Exibir a linha horizontal separadora
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns + 1; ++i) {
        printf("+---------------------");
    }
    printf("+\n");
}
//                      Printar o rodapé da tabela
void printTableFooter(int tableIndex) {
    for (int i = 0; i < databases[activeDatabaseIndex].tables[tableIndex].numColumns + 1; ++i) {
        printf("+---------------------");
    }
    printf("+\n");
}

/* ==================================================================== */

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
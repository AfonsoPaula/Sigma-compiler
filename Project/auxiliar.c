#include <stdio.h>
#include <stdlib.h>
#include "sintaxe.tab.h"

int main() {
    printf("Bem-vindo ao SQL Compiler! Insira os seus comandos:\n");

    while (1) {
        char query[1024];
        printf("> ");
        if (fgets(query, sizeof(query), stdin) == NULL) {
            break;  // Se o usuário pressionar Ctrl+D (EOF), encerra o loop
        }

        // Chama a função de análise sintática
        yyparse();
        if (yyparse() == 0) {
            printf("Comando analisado com sucesso!\n");
        } else {
            printf("Erro na análise do comando.\n");
        }
    }

    return 0;
}
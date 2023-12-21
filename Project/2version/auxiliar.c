#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int main() {
    printf("___________________________________________________________________________\n");
    printf(":::::::::::::::::::::::: Bem-vindo ao SQL Compiler ::::::::::::::::::::::::\n\n");
    printf("[Insira as suas queries]                            Digite [exit] para sair\n");
    printf("___________________________________________________________________________\n\n");

    int errors = 0;

    while (1) {
        
        printf("|    > ");
        yyparse();
        // if (yyparse() == 0) {
        //     printf("Comando analisado com sucesso!\n");
        // } else {
        //     errors++;
        // }
        
        // printf("|\n");
        
        // if (errors > 0) {
        //     printf("Total de %d erro(s) de análise encontrados.\n", errors);
        // } else {
        //     printf("Nenhum erro de análise encontrado.\n");
        // }
    }

    printf("Total de %d erro(s) de análise encontrados.\n", errors);


    return 0;
}
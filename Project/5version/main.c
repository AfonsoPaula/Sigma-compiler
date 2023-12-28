#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

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
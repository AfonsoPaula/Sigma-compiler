%{
    /* Definition Section */
    #include "sintaxe.tab.h"
%}

/* Rule Section */
%%

"CREATE"                { return CREATE; }
"DATABASE"              { return DATABASE; }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.strval = strdup(yytext); return IDENTIFIER; }
";"                     { return SEMICOLON; }
[ \t]                   ;
\n                      { return NEWLINE; }
.                       printf("Unexpected Character.\n");

%%

int yywrap() 
{
    return 1;
}
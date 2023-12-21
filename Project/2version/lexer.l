%option noyywrap

/* Definition section */
%{
#include "parser.tab.h"
%}

/* Rule Section */
%%

"exit"                  { return EXTCMD; }

"CREATE"                { return CREATE; }
"DATABASE"              { return DATABASE; }
";"                     { return SEMICOLON; }

[a-zA-Z][a-zA-Z0-9_]*   { yylval.id = strdup(yytext);     return IDENTIFIER; }
[0-9]+                  { yylval.num = atoi(yytext);      return NUMBER; }

[ \t]                   ;
[\n]                    { return LINE; }
.                       { printf("!! ['%s' Caractere não reconhecido !!]\n", yytext);}

%%
%option noyywrap

%{
#include <string>
#include "y.tab.h"
%}

%%

[[:digit:]]     { yyval.i = std::stoi(yytext); return tVAR; }
"VERD"          { yyval.i = !0;                return tVAL; }
"FALS"          { yyval.i = 0;                 return tVAL; }
[!&|~;,()=]                                    return *yytext;
"=="                                           return tEQ;
"!="                                           return tNE;
.|\n            ;

%%
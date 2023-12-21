%{
#include <iostream>
#include <string>
extern int yylex();
namespace { int vars[10]; }
%}

%token tEQ tNE
%token<i> tVAR tVAL
%type<i> expr lval

%right '='
%left tEQ tNE
%right '~'
%left '|'
%left '&'
%nonassoc '!'

%%

list: stmt
    | list stmt
    ;

stmt: expr ','
    | expr ';'          { std::cout << ($1 ? "true" : "false") << std::endl; }
    ;

expr: tVAL              { $$ = $1; }
    | lval              { $$ = vars[$1]; }
    | lval '=' expr     { $$ = vars[$1] = $3; }
    | expr '~' expr     { $$ = $1 != $3; }
    | expr '|' expr     { $$ = $1 |  $3; }
    | expr '&' expr     { $$ = $1 &  $3; }
    | expr tEQ expr     { $$ = $1 == $3; }
    | expr tNE expr     { $$ = $1 != $3; }
    | '!' expr          { $$ = !$2; }
    | '(' expr ')'      { $$ =  $2; }
    ;

lval : tVAR { $$ = $1; } ;

%%
extern int yyparse();
void yyerror(const char *s) { std::cout << s << std::endl; }
int main() { yyparse(); }
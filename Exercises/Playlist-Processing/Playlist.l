%option 8bit noyywrap yylineno
%{
#include <iostream>
#include <string>
#include <map>

static std::map<std::string, int> counts;
static std::string artist;
inline void yyerror(const char *msg) { std::cout << msg << std::endl; }
%}

NAME    [[:alnum:]][- _:[:alnum:]]*
MINUTES [[:digit:]]+
COMMENT ^[ \t]+.*$

%x X_ARTIST X_TITLE X_STRING X_MINUTES

%%
{COMMENT}                       ; /* ignore */

^{NAME}                         artist = yytext; BEGIN(X_TITLE);

^\"                             artist = ""; BEGIN(X_ARTIST);
<X_ARTIST>\"                    BEGIN(X_TITLE);
<X_ARTIST>\\\"                  artist += '"';
<X_ARTIST>.                     artist += *yytext;
<X_ARTIST>\n                    yyerror("should not happen!");

<X_TITLE>{NAME}                 BEGIN(X_MINUTES); /* not needed: ignore */

<X_TITLE>\"                     BEGIN(X_STRING);
<X_STRING>\"                    BEGIN(X_MINUTES);
<X_STRING>.|\\\"                ; /* not needed: ignore */
<X_STRING>\n                    yyerror("should not happen!");

<X_MINUTES>{MINUTES}            {
                                  int minutes = strtol(yytext, NULL, 10);
                                  if (minutes > 2) counts[artist]++;
                                }
<X_MINUTES>":".*$               BEGIN(INITIAL);

<X_TITLE,X_MINUTES>\t+          ;
<INITIAL,X_TITLE,X_MINUTES>.    ;
\n                              ; /* ignore */
%%

int main() {
  yylex();
  for (std::map<std::string, int>::iterator it = counts.begin();
       it != counts.end(); it++) {
    std::cout << it->first << " " << it->second << std::endl;
  }
}
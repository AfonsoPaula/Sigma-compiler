%option noyywrap

/* Definition section */
%{
#include "parser.tab.h"
%}

/* Rule Section */
%%

[eE][xX][iI][tT]        { return EXTCMD;     }

"CREATE"                { return CREATE;     }
"SHOW"                  { return SHOW;       }
"DROP"                  { return DROP;       }
"DATABASE"              { return DATABASE;   }
"DATABASES"             { return DATABASES;  }
"USE"                   { return USE;        }
"TABLE"                 { return TABLE;      }
"TABLES"                { return TABLES;     }
"WHICH"                 { return WHICH;      }
"INSERT INTO"           { return INSERT;     }
"VALUES"                { return VALUES;     }
"SELECT"                { return SELECT;     }
"DELETE"                { return DELETE;     }
"FROM"                  { return FROM;       }
"ROW"                   { return ROW;        }
"COL"                   { return COL;        }
"UPDATE"                { return UPDATE;     }
"WHERE"                 { return WHERE;      }
"FOR"                   { return FOR;        }

">="                    { return MAIGUAL;    }
">"                     { return MAIOR;      }
"<"                     { return MENOR;      }
"<="                    { return MEIGUAL;    }
"="                     { return IGUAL;      }
"!="                    { return DIFF;      }

";"                     { return SEMICOLON;  }
","                     { return COMMA;      }
"("                     { return OPENPAR;    }
")"                     { return CLOSEPAR;   }
"\'"                    { return APTS;       }

[a-zA-Z][a-zA-Z0-9_]*   { yylval.id = strdup(yytext);     return IDENTIFIER; }
[0-9]+                  { yylval.num = atoi(yytext);      return NUMBER;     }


[ \t]                   ;
[\n]                    { return LINE; }
.                       { printf("!! ['%s' Caractere não reconhecido !!]\n", yytext);}

%%
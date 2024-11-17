%{
    #include "y.tab.h"
%}

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[ \t]           { /* Ignore whitespace */ }
\n              { return '\n'; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"("             { return '('; }
")"             { return ')'; }

.               { return yytext[0]; }  /* Return the character as is */

%%

int yywrap() {
    return 1;
}
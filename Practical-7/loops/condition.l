%{
#include "y.tab.h"
%}

%%

"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"switch"            { return SWITCH; }
"case"              { return CASE; }
"default"           { return DEFAULT; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"("                 { return '('; }
")"                 { return ')'; }
";"                 { return ';'; }
"<="                { return LE; }
">="                { return GE; }
"=="                { return EQ; }
"!="                { return NE; }
"<"                 { return '<'; }
">"                 { return '>'; }
"="                 { return '='; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }

[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }

[ \t\n]+            { /* ignore whitespace */ }
.                   { return yytext[0]; }

%%

int yywrap() {
    return 1;
}


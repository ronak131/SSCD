%{
#include <stdio.h>
#include <string.h>

int token_count = 0;
%}

%%

[ \t\n]+                   ; 
[a-zA-Z_][a-zA-Z0-9_]*     { printf("Token: %s, Length: %zu\n", yytext, yyleng); token_count++; }
[0-9]+                     { printf("Token: %s, Length: %zu\n", yytext, yyleng); token_count++; }
.                          { printf("Token: %s, Length: %zu\n", yytext, yyleng); token_count++; }

%%

int main()
{
    yylex();
    printf("Total number of tokens: %d\n", token_count);
    return 0;
}

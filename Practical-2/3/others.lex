%{
#include <stdio.h>
#include <string.h>

int keywords_count = 0;
int identifiers_count = 0;
int numbers_count = 0;
int operators = 0;
int separators=0;
int others_count = 0;
%}

%%

"int"|"return"|"if"|"else"|"while"|"for" { printf("Keyword: %s\n", yytext); keywords_count++; }
[a-zA-Z_][a-zA-Z0-9_]*                   { printf("Identifier: %s\n", yytext); identifiers_count++; }
[0-9]+                                   { printf("Number: %s\n", yytext); numbers_count++; }
"="|"+"|"-"|"*"|"/"|"%"|"<"|">"|"<="|">="|"==" { printf("Operator: %s\n", yytext); operators++; }
[(){},;|]                                { printf("separator: %s\n", yytext); separators++; }
.                                        { printf("Other: %s\n", yytext); others_count++; }

%%

int main()
{
    yylex();
    printf("Identifiers: %d\n", identifiers_count);
    printf("Keyword: %d\n", keywords_count);
    printf("Numbers: %d\n", numbers_count);
    printf("separators: %d\n", separators);
    printf("operators: %d\n", operators);
    printf("Others: %d\n", others_count);
    return 0;
}
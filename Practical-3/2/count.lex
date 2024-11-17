%{
#include <stdio.h>
int char_count = 0;
int word_count = 0;
int line_count = 0;
%}
%%
[a-zA-Z0-9]+     { word_count++; char_count += yyleng; }  
[\n]          { line_count++;}                                
%%
int main() {
    yylex();
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    return 0;
}
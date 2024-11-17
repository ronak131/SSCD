%{
#include <stdio.h>
FILE *commentFile;
FILE *outputFile;
%}
%%
"//".*      { fprintf(commentFile, "%s\n", yytext); }     
"/*"([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*"*"+"/"   { fprintf(commentFile, "%s", yytext); } 
.           { fprintf(outputFile, "%c", yytext[0]); }     
\n          { fprintf(outputFile, "\n"); }               
%%
int main() {
    yyin=fopen("a.c","r");
    commentFile = fopen("comments.txt", "w");
    outputFile = fopen("output.c", "w");
    yylex();
    fclose(commentFile);
    fclose(outputFile);
    fclose(yyin);
    return 0;
}
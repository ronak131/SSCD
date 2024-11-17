%{
#include <stdio.h>
#include <string.h>
int found_html = 0;
int found_sql = 0;
const char *sql_keywords[] = {
    "SELECT", "INSERT", "UPDATE", "DELETE", "FROM", "WHERE"
};
%}
%%
"<"[a-zA-Z/][^>]*">"    { found_html = 1; }
[a-zA-Z]+              { 
                          int i = 0;
                          while (sql_keywords[i]) {
                              if (strcasecmp(yytext, sql_keywords[i]) == 0) {
                                  found_sql = 1;
                                  break;
                              }
                              i++;
                          }
                        }
%%
int main() {
    yylex();
    if (found_html) {
        printf("The input contains HTML tags.\n");
    }
    if (found_sql) {
        printf("The input contains SQL keywords.\n");
    }
    if (!found_html && !found_sql) {
        printf("The input does not contain any HTML tags or SQL keywords.\n");
    }
    return 0;
}
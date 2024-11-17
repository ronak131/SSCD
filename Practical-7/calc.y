%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    void yyerror(const char *s);
    int yylex(void);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'
%right UMINUS

%%

calculation:
    expression '\n'        { printf("Result: %d\n", $1); }
    | calculation expression '\n' { printf("Result: %d\n", $2); }
    ;

expression:
    NUMBER                 { $$ = $1; }
    | expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { if($3 == 0) yyerror("Division by zero!"); else $$ = $1 / $3; }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')'   { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expressions to calculate:\n");
    yyparse();
    return 0;
}
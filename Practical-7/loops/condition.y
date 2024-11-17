%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex();
int sym[26];  // Simple symbol table

%}

%union {
    int num;
    char* str;
}

%token <num> NUMBER
%token <str> IDENTIFIER
%token IF ELSE WHILE FOR SWITCH CASE DEFAULT
%token LE GE EQ NE

%left '+' '-'
%left '*' '/'
%nonassoc '<' '>' LE GE EQ NE

%%

// Starting point of the grammar
program:
    stmt_list
    ;

stmt_list:
    stmt_list stmt
    |
    ;

stmt:
    expr ';'                     // Expression statement
    | if_stmt                    // If statement
    | while_stmt                 // While statement
    | for_stmt                   // For statement
    | switch_stmt                // Switch statement
    | '{' stmt_list '}'          // Block of statements
    ;

if_stmt:
    IF '(' expr ')' stmt %prec IF
    | IF '(' expr ')' stmt ELSE stmt
    ;

while_stmt:
    WHILE '(' expr ')' stmt
    ;

for_stmt:
    FOR '(' expr ';' expr ';' expr ')' stmt
    ;

switch_stmt:
    SWITCH '(' expr ')' '{' case_list '}'
    ;

case_list:
    case_list case_stmt
    | case_list default_stmt
    | /* empty */
    ;

case_stmt:
    CASE expr ':' stmt          // Case statement
    ;

default_stmt:
    DEFAULT ':' stmt            // Default case
    ;

expr:
    IDENTIFIER '=' expr         // Assignment
    | expr '+' expr             // Addition
    | expr '-' expr             // Subtraction
    | expr '*' expr             // Multiplication
    | expr '/' expr             // Division
    | expr '<' expr             // Less than
    | expr '>' expr             // Greater than
    | expr LE expr              // Less than or equal to
    | expr GE expr              // Greater than or equal to
    | expr EQ expr              // Equal to
    | expr NE expr              // Not equal to
    | '(' expr ')'              // Parentheses
    | NUMBER                    // Number literal
    | IDENTIFIER                // Identifier
    ;

%%

// Error handling function
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter your program:\n");
    yyparse();
    return 0;
}


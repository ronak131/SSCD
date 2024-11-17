// E->TE'
// E'->+TE'|-TE'|ε
// T->FT'
// T'->*FT'|/FT'|ε
// F->PP'
// F'->^F|ε
// P->(E)|id

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

char *input;
char lookahead;

void match(char expected){
    if (lookahead == expected){
        lookahead = *++input;
    } else {
        printf("Error, Expected %c\n",expected);
        exit(1);
    }
}

void E();
void E_dash();
void T();
void T_dash();
void F();
void F_dash();
void P();

void E(){
    T();
    E_dash();
}

void E_dash(){
    if (lookahead=='+'){
        match('+');
        T();
        E_dash();
    }
    else if (lookahead=='-'){
        match('-');
        T();
        E_dash();
    }
    // ε : do nothing
}

void T(){
    F();
    T_dash();
}

void T_dash(){
    if (lookahead=='*'){
        match('*');
        F();
        T_dash();
    } else if(lookahead=='/'){
        match('/');
        F();
        T_dash();
    }
    // ε : do nothing
}

void F(){
    P();
    F_dash();
}

void F_dash(){
    if (lookahead=='^'){
        match('^');
        F();
    }
    // ε : do nothing
}

void P() {
    if (lookahead == '(') {
        match('(');
        E();
        match(')');
    } else if (lookahead == 'i') {
        match('i');
        if (lookahead == 'd') {
            match('d');
        } else {
            printf("Error, Expected 'd' after 'i'\n");
            exit(1);
        }
    } else {
        printf("Error, Expected ( or id\n");
        exit(1);
    }
}

int main(){
    char inputString[100];
    printf("enter the string: ");
    scanf("%s",&inputString);
    input=inputString;
    lookahead=*input;
    E();
    if (lookahead=='\0'){
        printf("String accepted\n");
    } else {
        printf("Error, Unexpected character %c\n",lookahead);
    }
}
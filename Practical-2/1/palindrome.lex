%{
#include<stdio.h>
#include<string.h>
%}

%%
[a-zA-Z0-9]+ {
    int len=yyleng;
    int is_palindrome=1;
    for (int i=0;i<len/2;i++){
        if (yytext[i]!=yytext[len-i-1]){
            is_palindrome=0;
            break;
        }
    }
    if(is_palindrome){printf("True\n");}
    else{printf("False\n");}
}
[\n] 
[.] 
%%

int main(){
    yylex();
    return 0;
}
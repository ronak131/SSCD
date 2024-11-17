#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pm();
void plus();
void div_op();
void reverse(char *str); // Custom reverse function

int i, ch, j, l, addr = 100;
char ex[10], exp[10], exp1[10], exp2[10], id1[5], op[5], id2[5];

int main()
{
    while (1)
    {
        printf("\n1. Assignment\n2. Arithmetic\n3. Relational\n4. Exit\nEnter the choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("\nEnter the expression with assignment operator: ");
            scanf("%s", exp);
            l = strlen(exp);
            exp2[0] = '\0';
            i = 0;
            
            // Finding the '=' operator
            while (exp[i] != '=')
            {
                i++;
            }
            strncpy(exp2, exp, i); // Copy left side of '=' into exp2
            exp2[i] = '\0'; // Null-terminate
            
            strcpy(exp1, &exp[i + 1]); // Copy right side of '=' into exp1
            printf("Three address code:\ntemp = %s\n%s = temp\n", exp1, exp2);
            break;

        case 2:
            printf("\nEnter the expression with arithmetic operator: ");
            scanf("%s", ex);
            strcpy(exp, ex);
            l = strlen(exp);
            exp1[0] = '\0';

            for (i = 0; i < l; i++)
            {
                if (exp[i] == '+' || exp[i] == '-')
                {
                    if (exp[i + 2] == '/' || exp[i + 2] == '*')
                    {
                        pm();
                        break;
                    }
                    else
                    {
                        plus();
                        break;
                    }
                }
                else if (exp[i] == '/' || exp[i] == '*')
                {
                    div_op();
                    break;
                }
            }
            break;

        case 3:
            printf("Enter the expression with relational operator: ");
            scanf("%s %s %s", id1, op, id2);
            if ((strcmp(op, "<") != 0) && (strcmp(op, ">") != 0) && (strcmp(op, "<=") != 0) && 
                (strcmp(op, ">=") != 0) && (strcmp(op, "==") != 0) && (strcmp(op, "!=") != 0))
            {
                printf("Expression error\n");
            }
            else
            {
                printf("\n%d\tif %s %s %s goto %d", addr, id1, op, id2, addr + 3);
                addr++;
                printf("\n%d\t T := 0", addr);
                addr++;
                printf("\n%d\t goto %d", addr, addr + 2);
                addr++;
                printf("\n%d\t T := 1", addr);
                addr++;
            }
            break;

        case 4:
            exit(0);

        default:
            printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}

void pm()
{
    reverse(exp);
    j = l - i - 1;
    strncat(exp1, exp, j);
    reverse(exp1);
    printf("Three address code:\ntemp = %s\ntemp1 = %c %c temp\n", exp1, exp[j + 1], exp[j]);
}

void div_op()
{
    strncat(exp1, exp, i + 2);
    printf("Three address code:\ntemp = %s\ntemp1 = temp %c %c\n", exp1, exp[i + 2], exp[i + 3]);
}

void plus()
{
    strncat(exp1, exp, i + 2);
    printf("Three address code:\ntemp = %s\ntemp1 = temp %c %c\n", exp1, exp[i + 2], exp[i + 3]);
}

// Custom reverse function
void reverse(char *str)
{
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++)
    {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pm();
void plus();
void div_op();
void reverse(char *str); // Custom reverse function
void generate_quadruple(char op, char* arg1, char* arg2, char* result);
void generate_triple(char op, char* arg1, char* arg2);
void generate_indirect_triple(int index);

int i, ch, j, l, addr = 100, quad_index = 0, triple_index = 0;
char ex[10], exp[10], exp1[10], exp2[10], id1[5], op[5], id2[5];

// Quadruple structure to store instructions
struct Quadruple {
    char op;
    char arg1[5];
    char arg2[5];
    char result[5];
} quads[100];

// Triple structure to store instructions
struct Triple {
    char op;
    char arg1[5];
    char arg2[5];
} triples[100];

// Array of pointers for indirect triples
int indirect_triples[100];

int main()
{
    while (1)
    {
        printf("\n1. Assignment\n2. Arithmetic\n3. Relational\n4. Quadruple Table\n5. Triple Table\n6. Indirect Triple Table\n7. Exit\nEnter the choice: ");
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

            // Generate Quadruple
            generate_quadruple('=', exp1, "", exp2);

            // Generate Triple
            generate_triple('=', exp1, "");

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
                        pm(); // Handles precedence (multiplication/division before addition/subtraction)
                        break;
                    }
                    else
                    {
                        plus(); // Handles simple addition/subtraction
                        break;
                    }
                }
                else if (exp[i] == '/' || exp[i] == '*')
                {
                    div_op(); // Handles division/multiplication
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

                // Generate Quadruple for relational operator
                generate_quadruple(op[0], id1, id2, "T");

                // Generate Triple for relational operator
                generate_triple(op[0], id1, id2);
            }
            break;

        case 4:
            printf("\nQuadruple Table:\n");
            for (int i = 0; i < quad_index; i++)
            {
                printf("%d: (%c, %s, %s, %s)\n", i, quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
            }
            break;

        case 5:
            printf("\nTriple Table:\n");
            for (int i = 0; i < triple_index; i++)
            {
                printf("%d: (%c, %s, %s)\n", i, triples[i].op, triples[i].arg1, triples[i].arg2);
            }
            break;

        case 6:
            printf("\nIndirect Triple Table:\n");
            for (int i = 0; i < triple_index; i++)
            {
                generate_indirect_triple(i);
            }
            break;

        case 7:
            exit(0);

        default:
            printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}

// Quadruple generation function
void generate_quadruple(char op, char* arg1, char* arg2, char* result)
{
    quads[quad_index].op = op;
    strcpy(quads[quad_index].arg1, arg1);
    strcpy(quads[quad_index].arg2, arg2);
    strcpy(quads[quad_index].result, result);
    quad_index++;
}

// Triple generation function
void generate_triple(char op, char* arg1, char* arg2)
{
    triples[triple_index].op = op;
    strcpy(triples[triple_index].arg1, arg1);
    strcpy(triples[triple_index].arg2, arg2);
    triple_index++;
}

// Indirect triple generation function
void generate_indirect_triple(int index)
{
    indirect_triples[index] = index;
    printf("%d -> %d\n", index, indirect_triples[index]);
}

void pm()
{
    reverse(exp);
    j = l - i - 1;
    strncat(exp1, exp, j);
    reverse(exp1);
    printf("Three address code:\ntemp = %s\ntemp1 = %c %c temp\n", exp1, exp[j + 1], exp[j]);

    // Generate Quadruple and Triple for the operation
    generate_quadruple(exp[j + 1], exp1, "", "temp1");
    generate_triple(exp[j + 1], exp1, "");
}

void div_op()
{
    strncat(exp1, exp, i + 2);
    printf("Three address code:\ntemp = %s\ntemp1 = temp %c %c\n", exp1, exp[i + 2], exp[i + 3]);

    // Generate Quadruple and Triple for division
    generate_quadruple(exp[i + 2], "temp", &exp[i + 3], "temp1");
    generate_triple(exp[i + 2], "temp", &exp[i + 3]);
}

void plus()
{
    strncat(exp1, exp, i + 2);
    printf("Three address code:\ntemp = %s\ntemp1 = temp %c %c\n", exp1, exp[i + 2], exp[i + 3]);

    // Generate Quadruple and Triple for addition/subtraction
    generate_quadruple(exp[i + 2], "temp", &exp[i + 3], "temp1");
    generate_triple(exp[i + 2], "temp", &exp[i + 3]);
}

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
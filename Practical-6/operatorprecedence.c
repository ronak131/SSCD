#include <stdio.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define NUM_OPERATORS 4

const char *operators[NUM_OPERATORS] = {"id", "+", "*", "$"};

char precedence_table[NUM_OPERATORS][NUM_OPERATORS] = {
    // id   +    *    $
    { ' ', '>', '>', '>' }, // id
    { '<', '>', '<', '>' }, // +
    { '<', '>', '>', '>' }, // *
    { '<', '<', '<', ' ' }  // $
};

int findOperatorIndex(const char *op) {
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (strcmp(operators[i], op) == 0) {
            return i;
        }
    }
    return -1;
}

void printStackAndCurrentChar(char stack[][10], int top, const char *currentChar) {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%s ", stack[i]);
    }
    printf(" | Current Character: %s\n", currentChar);
}

int parseExpression(const char *input) {
    char stack[100][10];
    int top = 0;
    
    strcpy(stack[top], "$");
    printf("Initial ");
    printStackAndCurrentChar(stack, top, input);

    const char *currentInput = input;

    while (*currentInput != '\0') {
        char topOfStack[10];
        strcpy(topOfStack, stack[top]);

        char currentSymbol[10];
        if (strncmp(currentInput, "id", 2) == 0) {
            strcpy(currentSymbol, "id");
            currentInput += 2; 
        } else {
            snprintf(currentSymbol, sizeof(currentSymbol), "%c", *currentInput);
            currentInput++;
        }

        if ((strcmp(currentSymbol, "+") == 0 || strcmp(currentSymbol, "*") == 0) && 
            (strcmp(topOfStack, "+") == 0 || strcmp(topOfStack, "*") == 0 || strcmp(topOfStack, "$") == 0)) {
            printf("Error: Invalid sequence of operators\n");
            return 0;
        }

        int topIndex = findOperatorIndex(topOfStack);
        int currentIndex = findOperatorIndex(currentSymbol);

        char precedence = precedence_table[topIndex][currentIndex];

        if (precedence == '<' || precedence == ' ') {
            top++;
            strcpy(stack[top], currentSymbol);
            printf("Push %-3s ", currentSymbol);
            printStackAndCurrentChar(stack, top, currentSymbol);
        } else if (precedence == '>') {
            while (top > 0 && precedence_table[findOperatorIndex(stack[top])][currentIndex] == '>') {
                printf("Pop     ");
                top--;
                printStackAndCurrentChar(stack, top, currentSymbol);
            }
            top++;
            strcpy(stack[top], currentSymbol);
            printf("Push %-3s ", currentSymbol);
            printStackAndCurrentChar(stack, top, currentSymbol);
        } else {
            printf("Error: Invalid precedence relation\n");
            return 0;
        }
    }

    while (top > 0) {
        printf("Pop     ");
        top--;
        printStackAndCurrentChar(stack, top, "$");
    }

    if (top == 0 && strcmp(stack[top], "$") == 0) {
        printf("Successfully parsed\n");
        return 1;
    } else {
        printf("Unsuccessful parsing\n");
        return 0;
    }
}

int main() {
    char input[MAX_STACK_SIZE];

    printf("Enter the input string (e.g., id+id*id$): ");
    fgets(input, MAX_STACK_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove the newline character

    parseExpression(input);
    return 0;
}

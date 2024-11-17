#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char lhs;
    char rhs[10];
} Rule;

Rule rules[10];
int ruleCount ;

char nonTerminals[10];
int nonTerminalCount;

char terminals[10];
int terminalCount;

int firstSet[10][10];
int followSet[10][10];
char parsingTable[10][10][10];

void initSets() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            firstSet[i][j] = 0;
            followSet[i][j] = 0;
        }
    }
}

int findIndex(char c, char array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == c) return i;
    }
    return -1;
}

void computeFirst() {
    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < ruleCount; i++) {
            char lhs = rules[i].lhs;
            char *rhs = rules[i].rhs;
            int lhsIndex = findIndex(lhs, nonTerminals, nonTerminalCount);

            if (rhs[0] == 'x') {
                if (!firstSet[lhsIndex][findIndex('x', terminals, terminalCount)]) {
                    firstSet[lhsIndex][findIndex('x', terminals, terminalCount)] = 1;
                    changed = 1;
                }
            } else {
                for (int j = 0; rhs[j] != '\0'; j++) {
                    char symbol = rhs[j];
                    if (findIndex(symbol, terminals, terminalCount) != -1) {
                        if (!firstSet[lhsIndex][findIndex(symbol, terminals, terminalCount)]) {
                            firstSet[lhsIndex][findIndex(symbol, terminals, terminalCount)] = 1;
                            changed = 1;
                        }
                        break;
                    } else if (findIndex(symbol, nonTerminals, nonTerminalCount) != -1) {
                        int symIndex = findIndex(symbol, nonTerminals, nonTerminalCount);
                        for (int k = 0; k < terminalCount; k++) {
                            if (firstSet[symIndex][k] && k != findIndex('x', terminals, terminalCount)) {
                                if (!firstSet[lhsIndex][k]) {
                                    firstSet[lhsIndex][k] = 1;
                                    changed = 1;
                                }
                            }
                        }
                        if (!firstSet[symIndex][findIndex('x', terminals, terminalCount)]) break;
                    }
                }
            }
        }
    }
}

void computeFollow() {
    followSet[findIndex('E', nonTerminals, nonTerminalCount)][findIndex('$', terminals, terminalCount)] = 1;

    int changed = 1;

    while (changed) {
        changed = 0;

        for (int i = 0; i < ruleCount; i++) {
            char lhs = rules[i].lhs;
            char *rhs = rules[i].rhs;
            int lhsIndex = findIndex(lhs, nonTerminals, nonTerminalCount);

            for (int j = 0; rhs[j] != '\0'; j++) {
                char symbol = rhs[j];
                if (findIndex(symbol, nonTerminals, nonTerminalCount) != -1) {
                    int symbolIndex = findIndex(symbol, nonTerminals, nonTerminalCount);

                    if (rhs[j + 1] != '\0') {
                        char nextSymbol = rhs[j + 1];
                        if (findIndex(nextSymbol, terminals, terminalCount) != -1) {
                            if (!followSet[symbolIndex][findIndex(nextSymbol, terminals, terminalCount)]) {
                                followSet[symbolIndex][findIndex(nextSymbol, terminals, terminalCount)] = 1;
                                changed = 1;
                            }
                        } else if (findIndex(nextSymbol, nonTerminals, nonTerminalCount) != -1) {
                            int nextSymbolIndex = findIndex(nextSymbol, nonTerminals, nonTerminalCount);
                            for (int k = 0; k < terminalCount; k++) {
                                if (firstSet[nextSymbolIndex][k] && k != findIndex('x', terminals, terminalCount)) {
                                    if (!followSet[symbolIndex][k]) {
                                        followSet[symbolIndex][k] = 1;
                                        changed = 1;
                                    }
                                }
                            }
                            if (firstSet[nextSymbolIndex][findIndex('x', terminals, terminalCount)]) {
                                for (int k = 0; k < terminalCount; k++) {
                                    if (followSet[lhsIndex][k]) {
                                        if (!followSet[symbolIndex][k]) {
                                            followSet[symbolIndex][k] = 1;
                                            changed = 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        for (int k = 0; k < terminalCount; k++) {
                            if (followSet[lhsIndex][k]) {
                                if (!followSet[symbolIndex][k]) {
                                    followSet[symbolIndex][k] = 1;
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void constructParsingTable() {
    for (int i = 0; i < ruleCount; i++) {
        char lhs = rules[i].lhs;
        char *rhs = rules[i].rhs;
        int lhsIndex = findIndex(lhs, nonTerminals, nonTerminalCount);

        if (rhs[0] == 'x') {
            for (int j = 0; j < terminalCount; j++) {
                if (followSet[lhsIndex][j]) {
                    strcpy(parsingTable[lhsIndex][j], "x");
                }
            }
        } else {
            for (int j = 0; rhs[j] != '\0'; j++) {
                char symbol = rhs[j];
                if (findIndex(symbol, terminals, terminalCount) != -1) {
                    int symbolIndex = findIndex(symbol, terminals, terminalCount);
                    strcpy(parsingTable[lhsIndex][symbolIndex], rhs);
                    break;
                } else if (findIndex(symbol, nonTerminals, nonTerminalCount) != -1) {
                    int symbolIndex = findIndex(symbol, nonTerminals, nonTerminalCount);
                    for (int k = 0; k < terminalCount; k++) {
                        if (firstSet[symbolIndex][k] && k != findIndex('x', terminals, terminalCount)) {
                            strcpy(parsingTable[lhsIndex][k], rhs);
                        }
                    }
                    if (firstSet[symbolIndex][findIndex('x', terminals, terminalCount)]) {
                        for (int k = 0; k < terminalCount; k++) {
                            if (followSet[lhsIndex][k]) {
                                strcpy(parsingTable[lhsIndex][k], "x");
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

void parse(char input[100]) {
    char stack[100];
    int stackTop = 0;
    int inputIndex = 0;

    stack[stackTop++] = '$';
    stack[stackTop++] = 'E';

    while (stackTop > 0) {
        char top = stack[stackTop - 1];
        char current = input[inputIndex];

        if (top == current) {
            stackTop--;
            inputIndex++;
        } else if (findIndex(top, terminals, terminalCount) != -1) {
            printf("Error: Unexpected symbol %c\n", current);
            return;
        } else {
            int topIndex = findIndex(top, nonTerminals, nonTerminalCount);
            int currentIndex = findIndex(current, terminals, terminalCount);

            if (strcmp(parsingTable[topIndex][currentIndex], "") != 0) {
                stackTop--;
                char *production = parsingTable[topIndex][currentIndex];
                if (strcmp(production, "x") != 0) {
                    int len = strlen(production);
                    for (int i = len - 1; i >= 0; i--) {
                        if (production[i] != 'x') {
                            stack[stackTop++] = production[i];
                        }
                    }
                }
            } else {
                printf("Error: No rule to match %c with %c\n", top, current);
                return;
            }
        }

        // Stack Trace
        printf("Stack: ");
        for (int i = stackTop-1; i >= 0 ; i--) {
            printf("%c", stack[i]);
        }
        printf(" Input: %s\n", &input[inputIndex]);
    }

    if (inputIndex == strlen(input)) {
        printf("Input successfully parsed!\n");
    } else {
        printf("Input not fully parsed!\n");
    }
}

void printFirstSets() {
    printf("FIRST sets:\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("%c: { ", nonTerminals[i]);
        for (int j = 0; j < terminalCount; j++) {
            if (firstSet[i][j]) {
                printf("%c ", terminals[j]);
            }
        }
        printf("}\n");
    }
}

void printFollowSets() {
    printf("FOLLOW sets:\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("%c: { ", nonTerminals[i]);
        for (int j = 0; j < terminalCount; j++) {
            if (followSet[i][j]) {
                printf("%c ", terminals[j]);
            }
        }
        printf("}\n");
    }
}

void printParsingTable() {
    printf("Parsing Table:\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        for (int j = 0; j < terminalCount; j++) {
            if (strcmp(parsingTable[i][j], "") != 0) {
                printf("[%c, %c] -> %s\n", nonTerminals[i], terminals[j], parsingTable[i][j]);
            }
        }
    }
}

int main() {
    // // Define grammar rules
    // rules[0] = (Rule){'E', "Te"};
    // rules[1] = (Rule){'e', "+Te"};
    // rules[2] = (Rule){'e', "x"};
    // rules[3] = (Rule){'T', "Ft"};
    // rules[4] = (Rule){'t', "*Ft"};
    // rules[5] = (Rule){'t', "x"};
    // rules[6] = (Rule){'F', "i"};
    // rules[7] = (Rule){'F', "(E)"};
    // ruleCount = 8;
    // E e T t F
    // + * ( ) i x $
    // E -> Te
    // e -> +Te
    // e -> x
    // T -> Ft
    // t -> *Ft
    // t -> x
    // F -> i
    // F -> (E)

    printf("Enter the number of non-terminals: ");
    scanf("%d", &nonTerminalCount);
    printf("Enter the non-terminals:\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        scanf(" %c", &nonTerminals[i]);
    }

    printf("Enter the number of terminals: ");
    scanf("%d", &terminalCount);
    printf("Enter the terminals:\n");
    for (int i = 0; i < terminalCount; i++) {
        scanf(" %c", &terminals[i]);
    }
    
    printf("Enter the number of grammar rules: ");
    scanf("%d", &ruleCount);
    printf("Enter the grammar rules (lhs rhs) where rhs is a string:\n");
    for (int i = 0; i < ruleCount; i++) {
        scanf(" %c -> %s", &rules[i].lhs, rules[i].rhs);
    }
    initSets();
    computeFirst();
    computeFollow();
    constructParsingTable();

    printFirstSets();
    printFollowSets();
    printParsingTable();

    // char input[100] = "i+i*i";
    char input[100];
    printf("Enter an input string: ");
    scanf("%s",input);
    strcat(input,"$");
    parse(input);
    return 0;
}
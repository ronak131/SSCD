#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main() {
    char ch;
    char word[50];
    int index = 0;
    int inComment = 0;
    int inString = 0;
    int keywordCount = 0;
    int identifierCount = 0;
    int tokenCount = 0;
    int operatorCount = 0;
    FILE *inputFile = fopen("a.c", "r");
    FILE *outputFile = fopen("output.txt", "w");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }
    if (outputFile == NULL) {
        printf("Error creating output file.\n");
        fclose(inputFile);
        return 1;
    }
    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch == '/' && !inString) {
            ch = fgetc(inputFile);
            if (ch == '/') {
                while ((ch = fgetc(inputFile)) != '\n' && ch != EOF);
                continue;
            } else if (ch == '*') {
                inComment = 1;
                while (inComment) {
                    ch = fgetc(inputFile);
                    if (ch == '*' && (ch = fgetc(inputFile)) == '/') {
                        inComment = 0;
                    }
                }
                continue;
            } else {
                ungetc(ch, inputFile);
                ch = '/';
            }
        }

        if (ch == '"' && !inComment) {
            inString = !inString;
        }

        if (!inComment && !inString) {
            if (isalpha(ch) || ch == '_') {
                word[index++] = ch;
            } else {
                if (index > 0) {
                    word[index] = '\0';
                    index = 0;

                    char *keywords[] = {"int", "return", "if", "else", "while", "for", "do", "break", "continue", "void", "main"};
                    int numKeywords = 11;
                    int isKeyword = 0;

                    for (int i = 0; i < numKeywords; ++i) {
                        if (strcmp(word, keywords[i]) == 0) {
                            isKeyword = 1;
                            break;
                        }
                    }

                    if (isKeyword) {
                        keywordCount++;
                    } else {
                        identifierCount++;
                    }
                }

                if (isdigit(ch) || ispunct(ch)) {
                    char operators[] = "+-*/%=&|^!<>";
                    int isOperator = 0;
                    for (int i = 0; i < strlen(operators); ++i) {
                        if (ch == operators[i]) {
                            isOperator = 1;
                            break;
                        }
                    }

                    if (isOperator) {
                        operatorCount++;
                    }
                    tokenCount++;
                }
            }
        }
    }
    if (index > 0) {
        word[index] = '\0';
        char *keywords[] = {"int", "return", "if", "else", "while", "for", "do", "break", "continue", "void", "main"};
        int numKeywords = 11;
        int isKeyword = 0;

        for (int i = 0; i < numKeywords; ++i) {
            if (strcmp(word, keywords[i]) == 0) {
                isKeyword = 1;
                break;
            }
        }
        if (isKeyword) {
            keywordCount++;
        } else {
            identifierCount++;
        }
    }
    fprintf(outputFile, "Keywords: %d\n", keywordCount);
    fprintf(outputFile, "Identifiers: %d\n", identifierCount);
    fprintf(outputFile, "Tokens: %d\n", tokenCount);
    fprintf(outputFile, "Operators: %d\n", operatorCount);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
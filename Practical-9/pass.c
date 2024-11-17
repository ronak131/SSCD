#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_LINE_LENGTH 256
#define MAX_LABEL_LENGTH 32

// Symbol table structure
typedef struct {
    char name[MAX_LABEL_LENGTH];
    int address;
    int isDefined;
} Symbol;

// Global variables
Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int currentAddress = 0;

// Function declarations
void pass1(FILE *input);
void pass2(FILE *input, FILE *output);
int addSymbol(const char *name, int address, int isDefined);
int findSymbol(const char *name);
void processLine(char *line, int isPass2, FILE *output);

// First pass: Build symbol table
void pass1(FILE *input) {
    char line[MAX_LINE_LENGTH];
    currentAddress = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, input)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == ';')
            continue;
            
        // Check for label
        char *label = strtok(line, " \t:");
        if (label && label[0] != '\t' && label[0] != ' ') {
            // Add label to symbol table
            addSymbol(label, currentAddress, 1);
        }
        
        // Handle instruction length
        // This is a simplified version - actual instruction length would depend on 
        // your instruction set
        currentAddress++;
    }
}

// Second pass: Generate code
void pass2(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    currentAddress = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, input)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == ';')
            continue;
            
        processLine(line, 1, output);
        currentAddress++;
    }
}

// Add symbol to symbol table
int addSymbol(const char *name, int address, int isDefined) {
    if (symbolCount >= MAX_SYMBOLS)
        return -1;  // Symbol table full
        
    // Check if symbol already exists
    int existing = findSymbol(name);
    if (existing != -1) {
        if (symbolTable[existing].isDefined && isDefined)
            return -2;  // Multiple definition error
            
        symbolTable[existing].address = address;
        symbolTable[existing].isDefined = isDefined;
        return existing;
    }
    
    // Add new symbol
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].address = address;
    symbolTable[symbolCount].isDefined = isDefined;
    return symbolCount++;
}

// Find symbol in symbol table
int findSymbol(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Process a single line
void processLine(char *line, int isPass2, FILE *output) {
    char *token = strtok(line, " \t:");
    
    // Skip label if present
    if (token && token[0] != '\t' && token[0] != ' ') {
        token = strtok(NULL, " \t");
    }
    
    if (!token)
        return;
        
    // Process instruction
    if (isPass2) {
        // This is where you would implement your actual instruction processing
        // This is a simplified example that just outputs the instruction and address
        fprintf(output, "%04X: ", currentAddress);
        
        // Check if instruction references a symbol
        char *operand = strtok(NULL, " \t");
        if (operand) {
            int symIndex = findSymbol(operand);
            if (symIndex != -1) {
                fprintf(output, "%s %04X\n", token, symbolTable[symIndex].address);
            } else {
                fprintf(output, "%s %s\n", token, operand);
            }
        } else {
            fprintf(output, "%s\n", token);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error opening input file\n");
        return 1;
    }
    
    // First pass
    pass1(input);
    rewind(input);
    
    // Second pass
    FILE *output = fopen(argv[2], "w");
    if (!output) {
        printf("Error opening output file\n");
        fclose(input);
        return 1;
    }
    
    pass2(input, output);
    
    fclose(input);
    fclose(output);
    return 0;
}
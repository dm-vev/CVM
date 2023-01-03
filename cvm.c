#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 1000
#define MAX_HEAP_SIZE 1000
#define MAX_CODE_SIZE 1000
#define MAX_IDENT_LEN 31

// Virtual machine registers
int sp = -1; // Stack pointer
int fp = -1; // Frame pointer
int ip = 0;  // Instruction pointer

// Virtual machine memory
int stack[MAX_STACK_SIZE];
int heap[MAX_HEAP_SIZE];
int code[MAX_CODE_SIZE];

// Identifier table
char ident_table[MAX_CODE_SIZE][MAX_IDENT_LEN];
int num_idents = 0;

// Opcodes
enum {
    LIT,   // Push literal value onto stack
    LOD,   // Load value from stack into top of stack
    STO,   // Store top of stack in memory
    ADD,   // Pop top two values from stack, add them, push result
    SUB,   // Pop top two values from stack, subtract second from first, push result
    MUL,   // Pop top two values from stack, multiply them, push result
    DIV,   // Pop top two values from stack, divide first by second, push result
    MOD,   // Pop top two values from stack, take modulus of first by second, push result
    NEG,   // Negate top of stack
    AND,   // Pop top two values from stack, AND them, push result
    OR,    // Pop top two values from stack, OR them, push result
    NOT,   // NOT top of stack
    JMP,   // Unconditional jump
    JT,    // Jump if top of stack is true
    JF,    // Jump if top of stack is false
    CMP,   // Compare top two values on stack
    LBL,   // Define label
    CALL,  // Call function
    RET,   // Return from function
    PRN,   // Print top of stack
    HLT    // Halt execution
};

// Instruction structure
typedef struct {
    int opcode;
    int operand;
} Instruction;

// Function table
typedef struct {
    char name[MAX_IDENT_LEN];
    int num_args;
    int num_locals;
    int code_size;
    Instruction code[MAX_CODE_SIZE];
} Function;

Function functions[MAX_CODE_SIZE];
int num_functions = 0;

// Parse an identifier from the input stream
char* parse_ident(FILE* input) {
    static char ident[MAX_IDENT_LEN];
    int i = 0;
    char c = fgetc(input);
    while (isalpha(c)) {
        ident[i++] = c;
        c = fgetc(input);
    }
    ungetc(c, input);
    ident[i] = '\0';
    return ident;
}

// Parse a numeric literal from the input stream
int parse_literal(FILE* input) {
    int value = 0;
    char c = fgetc(input);
    while (isdigit(c)) {
        value = value * 10 + (c - '0');
        c = fgetc(input);
    }
    ungetc(c, input);
    return value;
}

// Parse an instruction from the input stream
Instruction parse_instruction(FILE* input) {
    Instruction instr;
    char* ident = parse_ident(input);
    if (strcmp(ident, "LIT") == 0) {
        instr.opcode = LIT;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "LOD") == 0) {
        instr.opcode = LOD;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "STO") == 0) {
        instr.opcode = STO;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "ADD") == 0) {
        instr.opcode = ADD;
    } else if (strcmp(ident, "SUB") == 0) {
        instr.opcode = SUB;
    } else if (strcmp(ident, "MUL") == 0) {
        instr.opcode = MUL;
    } else if (strcmp(ident, "DIV") == 0) {
        instr.opcode = DIV;
    } else if (strcmp(ident, "MOD") == 0) {
        instr.opcode = MOD;
    } else if (strcmp(ident, "NEG") == 0) {
        instr.opcode = NEG;
    } else if (strcmp(ident, "AND") == 0) {
        instr.opcode = AND;
    } else if (strcmp(ident, "OR") == 0) {
        instr.opcode = OR;
    } else if (strcmp(ident, "NOT") == 0) {
        instr.opcode = NOT;
    } else if (strcmp(ident, "JMP") == 0) {
        instr.opcode = JMP;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "JT") == 0) {
        instr.opcode = JT;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "JF") == 0) {
        instr.opcode = JF;
        instr.operand = parse_literal(input);
    } else if (strcmp(ident, "CMP") == 0) {
        instr.opcode = CMP;
    } else if (strcmp(ident, "LBL") == 0) {
        instr.opcode = LBL;
        instr.operand = num_idents;
        strcpy(ident_table[num_idents++], parse_ident(input));
    } else if (strcmp(ident, "CALL") == 0) {
        instr.opcode = CALL;
        instr.operand = num_idents;
        strcpy(ident_table[num_idents++], parse_ident(input));
    } else if (strcmp(ident, "RET") == 0) {
        instr.opcode = RET;
    } else if (strcmp(ident, "PRN") == 0) {
        instr.opcode = PRN;
    } else if (strcmp(ident, "HLT") == 0) {
        instr.opcode = HLT;
    }
    return instr;
}

// Parse a function definition from the input stream
Function parse_function(FILE* input) {
    Function func;
    strcpy(func.name, parse_ident(input));
    func.num_args = parse_literal(input);
    func.num_locals = parse_literal(input);
    func.code_size = 0;
    Instruction instr = parse_instruction(input);
    while (instr.opcode != HLT) {
        func.code[func.code_size++] = instr;
        instr = parse_instruction(input);
    }
    func.code[func.code_size++] = instr;
    return func;
}


// Parse a program from the input stream
void parse_program(FILE* input) {

    char c = fgetc(input);
    while (!feof(input)) {
        if (isalpha(c)) {
            ungetc(c, input);
            functions[num_functions++] = parse_function(input);
        }
        c = fgetc(input);
    }
}

// Find a function by name
int find_function(char* name) {
    for (int i = 0; i < num_functions; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Execute the current instruction
void exec_instruction() {
    switch (code[ip]) {
        case LIT: {
            stack[++sp] = code[++ip];
            break;
        }
        case LOD: {
            stack[++sp] = stack[fp - code[++ip]];
            break;
        }
        case STO: {
            stack[fp - code[++ip]] = stack[sp--];
            break;
        }
        case ADD: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b + a;
            break;
        }
        case SUB: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b - a;
            break;
        }
        case MUL: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b * a;
            break;
        }
        case DIV: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b / a;
            break;
        }
        case MOD: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b % a;
            break;
        }
        case NEG: {
            stack[sp] = -stack[sp];
            break;
        }
        case AND: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b && a;
        }
        case OR: {
            int a = stack[sp--];
            int b = stack[sp--];
            stack[++sp] = b || a;
            break;
        }
        case NOT: {
            stack[sp] = !stack[sp];
            break;
        }
        case JMP: {
            ip = code[++ip] - 1;
            break;
        }
        case JT: {
            int a = stack[sp--];
            if (a) {
                ip = code[++ip] - 1;
            } else {
                ip++;
            }
            break;
        }
        case JF: {
            int a = stack[sp--];
            if (!a) {
                ip = code[++ip] - 1;
            } else {
                ip++;
            }
            break;
        }
        case CMP: {
            int a = stack[sp--];
            int b = stack[sp--];
            if (b < a) {
                stack[++sp] = -1;
            } else if (b > a) {
                stack[++sp] = 1;
            } else {
                stack[++sp] = 0;
            }
            break;
        }
        case LBL: {
            ip++;
            break;
        }
        case CALL: {
            int func_index = find_function(ident_table[code[++ip]]);
            int num_args = functions[func_index].num_args;
            int num_locals = functions[func_index].num_locals;
            int old_fp = fp;
            fp = sp - num_args + 1;
            for (int i = 0; i < num_locals; i++) {
                stack[fp + i] = 0;
            }
            sp += num_locals;
            ip = -1;
            for (int i = 0; i < functions[func_index].code_size; i++) {
                code[++ip] = functions[func_index].code[i].opcode;
                if (functions[func_index].code[i].opcode == LIT || functions[func_index].code[i].opcode == LBL || functions[func_index].code[i].opcode == JMP || functions[func_index].code[i].opcode == JT || functions[func_index].code[i].opcode == JF || functions[func_index].code[i].opcode == CALL) {
                    code[++ip] = functions[func_index].code[i].operand;
                }
            }
            break;
        }
        case RET: {
            sp = fp - 1;
            ip = stack[sp--];
            fp = stack[sp--];
        break;
    }
    case PRN: {
        printf("%d\n", stack[sp--]);
        break;
    }
    case HLT: {
        exit(0);
        break;
    }
    }
    ip++;
}

// Execute the program
void exec_program() {
    while (1) {
        exec_instruction();
    }
}

char* read_program_from_file(char* filename) {
    // Open the file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        exit(1);
    }

    // Determine the size of the file
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    // Allocate a buffer to hold the contents of the file
    char* buffer = (char*)malloc(size + 1);
    if (buffer == NULL) {
        printf("Error: Could not allocate memory for file '%s'\n", filename);
        exit(1);
    }

    // Read the contents of the file into the buffer
    size_t bytes_read = fread(buffer, sizeof(char), size, fp);
    if (bytes_read != size) {
        printf("Error: Could not read file '%s'\n", filename);
        exit(1);
    }

    // Add a null terminator at the end of the string
    buffer[size] = '\0';

    // Remove all whitespace characters from the string
    char* p = buffer;
    char* q = buffer;
    while (*p) {
        if (!isspace(*p)) {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';

    // Close the file and return the buffer
    fclose(fp);
    fflush(fp);
    return buffer;
}

char** parse_program_from_string(char* program) {
    // Count the number of lines in the program
    int num_lines = 0;
    char* p = program;
    while (*p) {
        if (*p == '\n') {
            num_lines++;
        }
        p++;
    }

    // Allocate an array to hold the lines of the program
    char** lines = (char**)malloc((num_lines + 1) * sizeof(char*));
    if (lines == NULL) {
        printf("Error: Could not allocate memory for program lines\n");
        exit(1);
    }

    // Split the program into lines
    p = program;
    int i = 0;
    while (*p) {
        lines[i] = p;
        while (*p && *p != '\n') {
            p++;
        }
        if (*p == '\n') {
            *p = '\0';
            p++;
        }
        i++;
    }
    lines[num_lines] = NULL;

    return lines;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: vm filename\n");
        return 1;
    }
    char* input_file_name = argv[1];
    char* program = read_program_from_file(input_file_name);
    char** program_lines = parse_program_from_string(program);
    parse_program(program_lines);
    exec_program();
    return 0;
}

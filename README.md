# CVM
Fast and efficient stack virtual machine. Writed in pure C99.

## Examples:

### Prime numbers program:

```asm
main:
    LIT 0 2   # Start at 2, the first prime number
    STO 0     # Store it in memory location 0

next:
    LDA 0     # Load the current number from memory location 0
    PRN       # Print it
    LIT 0 1   # Load the value 1
    ADD       # Add 1 to the current number
    STO 0     # Store the result in memory location 0
    LDA 0     # Load the current number again
    LIT 0 2   # Load the value 2
    DIV       # Divide the current number by 2
    JMPZ done # If the result is zero, we have reached the end
    JMP next  # Otherwise, go back and check the next number

done:
    HLT       # End the program
```

### Hello world example:

```asm
main:
    LIT 0 'H'
    PRN
    LIT 0 'e'
    PRN
    LIT 0 'l'
    PRN
    LIT 0 'l'
    PRN
    LIT 0 'o'
    PRN
    LIT 0 ','
    PRN
    LIT 0 ' '
    PRN
    LIT 0 'W'
    PRN
    LIT 0 'o'
    PRN
    LIT 0 'r'
    PRN
    LIT 0 'l'
    PRN
    LIT 0 'd'
    PRN
    LIT 0 '!'
    PRN
    HLT
```

## Operations descriptions:

LIT: Pushes a literal value onto the stack. The operand is the value to be pushed.
LOD: Loads a value from the stack into the top of the stack. The operand is the stack index from which the value should be loaded.
STO: Stores the top value on the stack in memory. The operand is the memory index where the value should be stored.
ADD: Pops the top two values from the stack, adds them together, and pushes the result onto the stack.
SUB: Pops the top two values from the stack, subtracts the second value from the first, and pushes the result onto the stack.
MUL: Pops the top two values from the stack, multiplies them together, and pushes the result onto the stack.
DIV: Pops the top two values from the stack, divides the first value by the second, and pushes the result onto the stack.
MOD: Pops the top two values from the stack, takes the modulus of the first value by the second, and pushes the result onto the stack.
NEG: Negates the top value on the stack.
AND: Pops the top two values from the stack, ANDs them together, and pushes the result onto the stack.
OR: Pops the top two values from the stack, ORs them together, and pushes the result onto the stack.
NOT: NOTs the top value on the stack.
JMP: Unconditionally jumps to the instruction at the specified operand index.
JT: Jumps to the instruction at the specified operand index if the top value on the stack is true.
JF: Jumps to the instruction at the specified operand index if the top value on the stack is false.
CMP: Compares the top two values on the stack and sets the top value to 1 if they are equal, 0 if they are not equal.
LBL: Defines a label at the current instruction index. The operand is the name of the label.
CALL: Calls the function with the specified operand index.
RET: Returns from the current function.
PRN: Prints the top value on the stack.
HLT: Halts execution.

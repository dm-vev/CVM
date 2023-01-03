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

```
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


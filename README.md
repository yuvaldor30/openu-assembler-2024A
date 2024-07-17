# Assembler

This repository contains the code for an assembler developed as a project assignment for the course **'Systems Programming in C'** at the Open University of Israel during the semester 2024A.

## Getting Started 
The project was developed and tested on Ubuntu but should be compatible with all Linux distributions.

### Prerequisites
Ensure that you have a C compiler installed on your system. The code was developed and tested using GCC.

### Usage
To compile the project, use the provided makefile:
```
make
```
After compiling, you can run the assembler by passing assembly file names (without file extensions) as arguments. For example, if you have assembly files named x.as, y.as, and hello.as, you would run:
```
./assembler x y hello
```
The assembler will generate output files with the same filenames and the following extensions:

- `.ob` - Object file

- `.ent` - Entries file

- `.ext` - Externals file

An example of input and output files can be found in the 'tests' folder.


## Directives
A **_directive_** line of the following structure:

1. An **optional** preceding *label*. e.g. `PLACE1:`.
2. A _directive_: `.data`, `.string`, `.entry` or `.extern`.
3. Operands according to the type of the *directive*.

### `.data`
This directive allocates memory in the data image for storing integers later in memory. It increases the data counter and updates the symbol table. The parameters of *`.data`* are valid integers separated by commas.
Example:
```
LABEL1: .data      +4, -26, 77, 9
```
### `.string`
   This directive receives a string as an operand and stores it in the data image. It stores all characters by their order in the string, encoded by their ASCII values.
Example:
```
LABEL1: .string "abcdefg"
```

   ### `.entry`
 This directive adds a received label name to the symbol table, allowing it to be recognized by other assembly files.
Example:
   ```
 ; file1.as
 .entry HELLO
 HELLO: add #1, r1 
   ```
   ### `.extern`
   This directive declares a label as external, meaning it is defined in another file, and the current file will use it.
Example:
   ```
.extern EXTERNAL_LABEL
   ```
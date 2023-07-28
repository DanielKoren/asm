### asm

asm stands for assembler which is a university project, goal is to write an 'imaginary assembler' that basically translates *.as into code specific binary code.

asm consist of 2 main stages called passes
* first_pass() - checks for labels, handles data and code statements while handling any .extern
* second_pass() - updates certain J and I instructions while also fixing 

if both first_pass() and second_pass() succeeded it will generate the following:
+ object file (*.ob)    - contains both code and data images with their 
+ external file (*.ext) - contains all symbols with external attribute and the memory address of where its being used
+ entry file (*.ent)    - contains all symbols with entry attribute and memory address 


### structure
```
main.c - self explanatory, parse_as_file() function calls first_pass() & second_pass(), if both function succeeded it will create output files.
assembler.c - contains first_pass() and second_pass() as well as functions that handle any directive or instruction statements
globals.h - any necessary data structures, constants, macros and generally global variables 
logger.c - functions used to log certain messagse (error,information or success)
images.c - code_image & data_image, using linked list to store data dynamically
symbols.c - represents symbol table, using linked list to store any symbols that found in .as file
helpers.c - mainly consists of functions that simplify the code
output_files.c - functions used to create .ob .ext .ent files
```
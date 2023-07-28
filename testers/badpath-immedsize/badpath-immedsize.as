; This file contains tests for errors in the range of immediate operands.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; Depending on implementation, these errors are detectable
; either in the first or second pass.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; 1. this line is ok
DATA1: .dw 100

; 2. this line is ok (highest positive immediate that fits in 16 bits)
       addi $1,32767,$2

; 3. invalid first operand (positive immediate too high to fit in 16 bits)
       addi $1,32768,$2

; 4. this line is ok (lowest negative immediate that fits in 16 bits)
       addi $1,-32768,$2

; 5. invalid second operand (negative immediate too low to fit in 16 bits)
       addi $1,-32769,$2

; 6. this line is ok
       stop

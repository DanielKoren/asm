; This file contains tests for errors in the range of integer data values.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; Depending on implementation, these errors are detectable
; either in the first or second pass.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; 1. this line is ok
X:   move $1,$2

; 2. this line is ok (highest positive value that fits in 1 byte)
DB1:  .db  127

; 3. data overflow (positive value too high to fit in 1 byte)
DB2:  .db  128

; 4. this line is ok (lowest negative value that fits in 1 byte)
DB3:  .db  -128

; 5. data overflow (negative value too low to fit in 1 byte)
DB4:  .db  -129

; 6. this line is ok (highest positive value that fits in 2 bytes)
DH1:  .dh  32767

; 7. data overflow (positive value too high to fit in 2 bytes)
DH2:  .dh  32768

; 8. this line is ok (lowest negative value that fits in 2 bytes)
DH3:  .dh  -32768

; 9. data overflow (negative value too low to fit in 2 bytes)
DH4:  .dh  -32769

; 10. this line is ok (highest positive value that fits in 4 bytes)
DW1:  .dw  2147483647

; 11. data overflow (positive value too high to fit in 4 bytes)
DW2:  .dw  2147483648

; 12. this line is ok (lowest negative value that fits in 4 bytes)
DW3:  .dw  -2147483648

; 13. data overflow (negative value too low to fit in 4 bytes)
DW4:  .dw  -2147483649
stop
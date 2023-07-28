; This file is intended to check the bad-path of the assembler.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the second pass of the assembler

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not necessarily exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok (label X is ignored, warning may be issued in 1st pass)
X:     .extern DATA1

; 2. this line is ok (label Y is ignored, warning may be issued in 1st pass)
Y:     .entry START

; 3. this line is ok
       .entry END

; 4. this line is ok (DATA1 is an external label)
START:  la   DATA1

; 5. undefined label
        bgt  $3,$24,Q

; 6. external label in conditional branch instruction
        blt  $3,$24,DATA1

; 7. undefined label 
        jmp  Q
        
; 8. undefined label (declaration of label X was ignored)
        call X

; 9. undefined label (declaration of label Y was ignored)
        beq  $5,$6,Y

; 10. this line is ok (it is ok to designate the same entry more than once)
       .entry END

; 11. this line is ok
END:    stop

; 12. undefined label 
       .entry Q

; 13. undefined label (declaration of label X was ignored) 
       .entry X

; 14. undefined label (declaration of label Y was ignored)  
       .entry Y

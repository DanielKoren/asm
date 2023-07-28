; This file is intended to check the bad-path of the assembler.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not necessarily exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok (label DATA4 is declared later)
Start:  la   DATA4

; 2. missing operand
        sub  $1,$4

; 3. missing operand
        sub  $3,

; 4. missing operand
        add

; 5. this line is ok (label declared as external)
       .extern DATA4

; 6. invalid third operand (label)
        add  $15,$2,DATA1

; 7. invalid second operand (label)
        add  $15,DATA1,$1
        
; 8. invalid first operand (label)
        add  DATA1,$15,$2

; 9. invalid third operand (immediate)
        add  $15,$2,2034

; 10. invalid second operand (immediate)
        add  $15,2034,$1
               
; 11. invalid first operand (immediate)
        add  2034,$15,$2

; 12. invalid second operand (immediate)
        move $15,2034

; 13. invalid second operand (label)
        move $15,DATA1

; 14. invalid first operand (immediate)
        mvlo 2034,$1

; 15. invalid first operand (label)
        mvlo DATA1,$1
               
; 16. invalid second operand (register)
        addi $1,$3,$2

; 17. invalid second operand (label)
        addi $1,DATA1,$2

; 18. this line is ok
Next:   blt  $23,$3,Start

; 19. invalid third operand (register)
        bne  $21,$2,$3
        
; 20. invalid third operand (immediate)
        bne  $21,$2,3456

; 21. invalid operand (immediate)
        jmp  100

; 22. undefined instruction name
        xor  $1,$2,$0

; 23. undefined instruction name (inatructions are in lower case only)
        noR  $1,$3,$1

; 24. invalid characters (,$2)
        mvhi $1,,$2

; 25. invalid characters (,$3)
        move $1,$2,$3
        
; 26. invalid characters (,$4)
        sub  $1,$2,$3,$4

; 27. invalid characters (blabla)
        call Start blabla

; 28. invalid operand (invalid register number)
        jmp  $32

; 29. invalid operand (incomplete register operand)  
        jmp  $  23

; 30. invalid characters (XYZ)
        jmp  $XYZ
        
; 31. invalid characters (#xyz)
        jmp  #xyz

; 32. label was already declared earlier
Next:   move $2,$3

; 33. label does not start in first column (optional error!)
     label1:   sub  $1,$7,$31

; 34. invalid operand (or invalid characters (Start))
        stop Start

; 35. this line is ok (.dw directive can be used without a label)
       .dw   400

; 36. this line is ok (.asciz directive can be used without a label)
        .asciz "maman14"

; 37. invalid characters (400) 
Z1:     .dh  200 400

; 38. this line is ok (arbitrary spaces/tabs between fields or at end of line)
DATA1:  .dw  100,    	200 ,  -500,300, 500   				   

; 39. this line is ok (arbitrary spaces/tabs between fields or at end of line)
        add   	  $1  ,      $1       	 ,$2 	        

; 40 invalid characters (,3)
Z2:     .db  1, ,3

; 41. invalid character (,)
Z3:     .db  4,

; 42. invalid characters (#123)
Z4:     .dw  #123

; 43. invalid characters (.4)
Z5:     .db  12.4

; 44. invalid characters (-5)
Z6:     .db  --5

; 45. invalid label (cannot be an instruction name)
move:   .dh  5

; 46. This line is ok (labels are case sensitive)
Move:   .dh  5

; 47. label was already declared earlier
DATA1:  .dw  300

; 48. this line is ok (labels are case sensitive)
Data1:  .dw  100, 200, -300

; 49. invalid label(non-alphabetic first character)
1DATA:  .dh  300

; 50. this line is ok (label X declaration ignored - warning may be issued)
X:      .entry  DATA1

; 51. this line is ok (it is ok to declare the same external more than once)
        .extern DATA4

; 52. local label cannot be declared as external
        .extern Start

; 53. this line is ok (label Y declaration ignored - warning may be issued)
Y:      .extern DATA8

; 54. this line is ok (label STOP is declared later)
        .entry  STOP

; 55. invalid symbol (DATA2 is not an instruction name and not a valid label)
DATA2   .dw  4

; 56. undefined directive (directives are lower case)
        .DW  5

; 57. invalid characters (blabla is not a string)
Z7:     .asciz blabla

; 58. invalid characters (blabla)
Z8:     .asciz "abcdefg" blabla

; 59. this line is ok (comma within string is not a separator)
STR1:   .asciz "abc, ,defg"

; 60. invalid label (too long)
SuperCalifragilisticExpiAlidocious: .dw 4

; 61. missing argument in directive
Z9:     .db

; 62. missing argument in directive
        .entry

; 63. undefined directive
Z10:    .invalid 85, 90

; 64. this line is ok (case sensitivity)
STOP:   stop

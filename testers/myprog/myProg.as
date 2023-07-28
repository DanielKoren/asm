; Write your own assembly language source program.
; The program should contain at least 10 insructions, 5 labels, 5 directives
; No errors: the assembler should generate successfully all output files:
;     myprog.ob, myprog.ext, myprog.ent

sub $3 $9 $8
TOOL: nor $20 $24 $1
OP: mvlo $2 $5
jmp END
END: .dw 32 , 3
M: la Y
.extern Y
.entry M
STR: .asciz "oPxT"
beq $31, $0, STR
.dh 48 , 3 , 1 , 69 , 88
BOW: stop
.db 7 , 1
MAIN: call Y
add $0, $1, $2
mvhi $0, $1

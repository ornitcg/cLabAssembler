;Errors on example file - to be discovered on first scan
; file ps.as

.entry LIST

;Invalid characters in label
BLA%: .extern W

;Invalid characters in label
12BLA: .extern W

;warning message- Label definition before .extern instruction - label is ignored
BLA: .extern W

;invalid command
MAIN:     r3,LIST

;invalid command
   addd r3,LIST

;Invalid characters in label
LOOP:    prn  MA$IN

;Operand is invalid as Immediate
    prn # 48

;Found too many operands
    prn #48 , r2

;Extra operand or extra comma found
        lea W,,r6

;Extra comma found
        lea ,W,r6

;Extra comma found
       lea W,r6,


;Extra comma found
          lea W,

;Missing Operand(s)
        lea W

;Found too many operands
         inc r6 , #2

;Invalid adressing type of operand
        mov r3, #56

;Used reserved word for label
add:     sub r1,r4

;Invalid adressing type of operand
      bne r2

;Operand is invalid as Immediate
    bne #END

;Operand value is out of word range
        cmp K,#-6000

;Missing parameter after .entry instruction
.entry

;Label has already been declared earlier
LOOP:     stop

;Label is too long
L0000000000000000000000000000000000000000OOP:     stop

;Invalid string
STR:    .string "abcd

;Label has already been declared earlier
STR:    .string

;Missing string
    .string

;Invalid data
LIST:   .data 6,-a9


;Extra comma found
   .data 6,-9,

;Missing data
        .data

;Invalid instruction name
K:      .daa 31

Missing parameter after .extern instruction
.extern

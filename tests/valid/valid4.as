jmp LABEL

LABEL: mov r1, r2

hlt

dataaa: .data 12, -1034, 92,14

LABEL2: bne dataaa

cmp LABEL, r4

dec LABEL2

lea LABEL2, r2
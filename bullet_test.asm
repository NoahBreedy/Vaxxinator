.const SHOOT_DIR 0x9000
.const MOVE_DIR 0x9001

;set rA, 1
;str [SHOOT_DIR], rA

!set
    set rB, 0

!main

    inc rB
    str [MOVE_DIR], rB
    str [SHOOT_DIR], rB

    cmp rB, 8
    jl !main
    jmp !set

.const SHOOT_DIR 0x9000
.const MOVE_DIR 0x9001
.const NEAREST_BLOODCELL_DIR 0x9002

set rA, 0
set rB, 0

!main

    lod rA, [NEAREST_BLOODCELL_DIR]

    str [SHOOT_DIR], rA

;!calc_inverse

    ;dec rA
    ;add rA, 4
    ;mod rA, 8
    ;inc rA

    ;str [MOVE_DIR], rA

    jmp !main
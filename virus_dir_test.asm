.const SHOOT_DIR 0x9000
.const MOVE_DIR 0x9001
.const NEAREST_BLOODCELL_DIR 0x9002
.const NEAREST_PLAYER_DIR 0x9003
.const NEAREST_VIRUS_DIR 0x9004

set rA, 0
set rB, 0

!main

    lod rA, [NEAREST_VIRUS_DIR]

    str [SHOOT_DIR], rA

    jmp !main
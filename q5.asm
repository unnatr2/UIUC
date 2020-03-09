                .ORIG x3000
                LD R4,DATA
                ST R4,INSTRUCTION
INSTRUCTION     ADD R4,R4,R4
                HALT
DATA            .FILL x993F
                .END

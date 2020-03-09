            .ORIG x4000
            AND R0, R0, #0 ; Insert instruction a.i) here
            LD      R1, STRING1
            LD      R2, STRING2
NEXTCHAR    LDR     R3, R1, #0
            LDR     R4, R2, #0
            BRz     EQUAL
            ADD R1, R1, #1 ; Insert instruction a.ii) here
            ADD R2, R2, #1 ; Insert instruction a.iii) here
            NOT     R3, R3
            ADD     R3, R3, #1
            ADD     R4, R4, R3
            BRz     NEXTCHAR
            ADD     R0, R0, #-1
            BRnzp STOP ; Insert instruction a.iv) here
EQUAL       ADD     R0, R0, #1
STOP        HALT
STRING1     .FILL   x5000
STRING2     .FILL   x6000
            .END

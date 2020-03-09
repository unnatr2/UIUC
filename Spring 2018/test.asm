            .ORIG x5000
            LEA     R0, MESSAGE
            LD      R1, TABLE
LOOP        LDR     R2, R1, #0
            NOT     R3, R2
            BRz     FINISH
            ADD     R1, R1, #1
            AND     R2, R2, #1
            BRz     LOOP
            PUTS
            BRnzp   LOOP
FINISH      HALT
MESSAGE     .STRINGZ "Found!"
TABLE       .FILL   x3000
            .END

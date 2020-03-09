                .ORIG x3000
                AND     R0, R0, #0
                ADD     R0, R0, #5
SHIFT           BRz     DONE
                ADD     R0, R0, #-1
                ADD     R1, R1, R1
                BR      SHIFT
DONE            TRAP    x25
                .END

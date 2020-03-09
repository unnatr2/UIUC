                .ORIG x3000
                LDI     R1, ADDRESS
                ADD     R1, R1, #-20
                BRnp    FINISH
                LEA     R0, MESSAGE
                PUTS
FINISH          HALT
MESSAGE         .STRINGZ    "M[x3025] is equal to twenty"
ADDRESS         .FILL   x3025
                .END

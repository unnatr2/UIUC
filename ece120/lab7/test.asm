                .ORIG   x3000
                AND R1, R1, #0          ; Initialize R0=0
                LDI R2, CHAR             ; Load Address of Character to R1
                LD R3, LINES            ; Load Number of lines to R2
MULTIPLY        ADD R1, R1, R2          ; R0 <-- R0+R1
                ADD R3, R3, #-1         ; Decrement R2
                BRp MULTIPLY            ; If +ve, loop to MULTIPLY
                LD R3, FONT_ADDR
                ADD R1, R1, R3
                LD R5, LINES
LOOP2_INIT      LD R4, CHARS
                LDR R2, R1, #0          ; Load content in current line to R2
LOOP_INIT      	ADD R2, R2, #0
		BRn NEG                 ; If -ve, move to NEG
                LDI R0, ZERO             ; Load R0 with Char for 0
                OUT
                BRnzp DECR              ; Always, move to DECR
NEG             LDI R0, ONE              ; Load R0 with Char for 1
                OUT
DECR		ADD R2, R2, R2
		ADD R4, R4, #-1		; Decrement R4     
                BRp LOOP_INIT           ; If +ve, move to LOOP_INIT
                LD R0, SENTINEL
		OUT
		ADD R1, R1, #1
                ADD R5, R5, #-1
                BRp LOOP2_INIT
		HALT
FONT_ADDR       .FILL   x3022
CHAR            .FILL   x5002           ; Character Address
LINES           .FILL   #16             ; Number of lines
ZERO            .FILL   x5000
ONE             .FILL   x5001
CHARS           .FILL   #8
SENTINEL        .FILL   x0A

		.FILL	x0000
		.FILL	x0000
		.FILL	xC600
		.FILL	xC600
		.FILL	xC600
		.FILL	xC600
		.FILL	xFE00
                .FILL   xC600
                .FILL   xC600
                .FILL   xC600
                .FILL   xC600
		.FILL   xC600
                .FILL   x0000
                .FILL   x0000
                .FILL   x0000
                .FILL   x0000
	

		.END


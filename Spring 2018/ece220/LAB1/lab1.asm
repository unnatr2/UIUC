				.ORIG x3000
				AND R4, R4, #0
				ADD R4, R4, #4
CHECK_DONE		BRnz DONE
				AND R1, R1, #0
				AND R2, R2, #0
				ADD R2, R2, #4
BACK			BRnz FIND_DIGIT
				ADD R1, R1, R1
				ADD R3, R3, #0				
				BRn NEGATIVE
				ADD R1, R1, #0
				BRnzp OUT_LOOP
NEGATIVE		ADD R1, R1, #1
OUT_LOOP		ADD R3, R3, R3
				ADD R2, R2, #-1
				BRnzp BACK
FIND_DIGIT		AND R5, R5, #0
				ADD R5, R1, #-9
				BRnz PRINT
				LD R6, LETTER
				ADD R5, R5, R6
				AND R1, R1, #0
				ADD R1, R1, R5
PRINT			AND R0, R0, #0
				ADD R0, R0, R1
				LD R6, ZERO
				ADD R0, R0, R6
				OUT
				ADD R4, R4, #-1
				BRnzp CHECK_DONE
DONE			HALT
LETTER			.FILL x0010
ZERO			.FILL x0030
				.END

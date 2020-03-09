				.ORIG x3000

				; load output string's address
				LEA R2, SPRITE
  
				; row counter set to 8
				AND R3, R3, #0
				ADD R3, R3, #8

				; R3 != 0 ?
NEXT_ROW		ADD R3, R3, #0
				BRz DONE

				; column counter set to 8
				AND R4, R4, #0
				ADD R4, R4, #8

				; R4 != 0 ?
NEXT_COLUMN		ADD R4, R4, #0
				BRz DONE_ROW
			
				; print next char
				LDR R0, R2, #0 ; read next char
				OUT
				ADD R2, R2, #1 ; move to next char

				; decrement column counter and move to next
				ADD R4, R4, #-1
				BRnzp NEXT_COLUMN

				; print new line char
DONE_ROW		LD R0, ASCII_NL ; load NewLine ASCII value
				OUT
			
				; move to next row
				ADD R3, R3, #-1
				BRnzp NEXT_ROW

DONE			HALT

ASCII_NL 		.FILL xA

				; row 0
SPRITE			.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				; row 1
				.FILL x2A ; *
				.FILL x20 ; " "
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				; row 2
				.FILL x2A ; *
				.FILL x20
				.FILL x5E ; ^
				.FILL x20
				.FILL x20
				.FILL x5E ; ^
				.FILL x20
				.FILL x2A ; *
				; row 3
				.FILL x2A ; *
				.FILL x20
				.FILL x2A ; *
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				.FILL x20
				.FILL x2A ; *
				; row 4
				.FILL x2A ; *
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				; row 5
				.FILL x2A ; *
				.FILL x20
				.FILL x2A ; *
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				.FILL x20
				.FILL x2A ; *
				; row 6
				.FILL x2A ; *
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x20
				.FILL x20
				.FILL x2A ; *
				; row 7
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				.FILL x2A ; *
				;
				.END

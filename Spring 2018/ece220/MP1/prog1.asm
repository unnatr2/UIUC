;	 ECE220 | MP1 | UNNAT RAMJIYANI | UNNATR2

; 	 This program takes a string starting at x4000 and counts the
;	 number of times every alphabet irrespective of case occurs in
;	 the string. It also counts the number of non-aplhabetic characters
;	 in the string. These bins are stored in x3F00. Also the program
;	 outputs the values of the bins in hexadecimal notation thus creating
;	 a histogram.

;    R0 holds a pointer to the histogram (x3F00) and holds values
;	    to print and holds temporary value to find hex value of digit
;
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization and
;	    holds 4 bits as digit
;
;    R2 holds the current character being counted and is also used
;       to point to the histogram entry and is also used as bit counter
;
;    R3 holds the additive inverse of ASCII '@' (xFFC0) and also works as
; 		holder of bin hex value
;
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6) and also
;       temporarily holds the current histogram address and also works as
;		the digit counter
;
;    R5 holds the difference between ASCII '@' and '`' (xFFE0) and also holds
;		the ASCII value of the bin character
;
;    R6 is used as a temporary register and also as the bin counter
;
;	 R7 is used as a temporary load register


				.ORIG	x3000			; starting address is x3000
				LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
; fill the histogram with zeroes 

				AND R6,R6,#0			; put a zero into R6
				LD R1,NUM_BINS			; initialize loop count to 27
				ADD R2,R0,#0			; copy start of histogram into R2

; loop to fill histogram starts here

HFLOOP			STR R6,R2,#0			; write a zero into histogram
				ADD R2,R2,#1			; point to next histogram entry
				ADD R1,R1,#-1			; decrement loop count
				BRp HFLOOP				; continue until loop count reaches zero

; initialize R1, R3, R4, and R5 from memory

				LD R3,NEG_AT			; set R3 to additive inverse of ASCII '@'
				LD R4,AT_MIN_Z			; set R4 to difference between ASCII '@' and 'Z'
				LD R5,AT_MIN_BQ			; set R5 to difference between ASCII '@' and '`'
				LD R1,STR_START			; point R1 to start of string

; the counting loop starts here

COUNTLOOP		LDR R2,R1,#0			; read the next character from the string
				BRz PRINT_HIST			; found the end of the string
				ADD R2,R2,R3			; subtract '@' from the character
				BRp AT_LEAST_A			; branch if > '@', i.e., >= 'A'
NON_ALPHA		LDR R6,R0,#0			; load the non-alpha count
				ADD R6,R6,#1			; add one to it
				STR R6,R0,#0			; store the new non-alpha count
				BRnzp GET_NEXT			; branch to end of conditional structure
AT_LEAST_A		ADD R6,R2,R4			; compare with 'Z'
				BRp MORE_THAN_Z 	    ; branch if > 'Z'
ALPHA			ADD R2,R2,R0			; point to correct histogram entry
				LDR R6,R2,#0			; load the count
				ADD R6,R6,#1			; add one to it
				STR R6,R2,#0			; store the new count
				BRnzp GET_NEXT			; branch to end of conditional structure
	
; subtracting as below yields the original character minus '`'

MORE_THAN_Z		ADD R2,R2,R5			; subtract '`' - '@' from the character
				BRnz NON_ALPHA			; if <= '`', i.e., < 'a', go increment non-alpha
				ADD R6,R2,R4			; compare with 'z'
				BRnz ALPHA				; if <= 'z', go increment alpha count
				BRnzp NON_ALPHA			; otherwise, go increment non-alpha
GET_NEXT		ADD R1,R1,#1			; point to next character in string
				BRnzp COUNTLOOP			; go to start of counting loop

; print histogram

PRINT_HIST		LD R5, START			; load R5 with '@' 
                LD R6, NUM_BINS			; load R6 with number of bins (bin counter)
				LD R4, HIST_ADDR_TEMP	; load R4 with temporary histogram address
				LDR R3, R4, #0			; load value in R4 in R3 (the hex number to be printed) 
				ST R4, HIST_ADDR_TEMP	; store R4 in temporary histogram address
				BRnzp SHIFT				; move to SHIFT always

; find next histogram hex value

CONTINUE        ADD R5, R5, #1			; increment R5 with #1
				LD R4, HIST_ADDR_TEMP	; load R4 with temporary histogram address
				ADD R4, R4, #1			; increment R4 with #1
				ST R4, HIST_ADDR_TEMP	; store R4 in temporary histogram address
				LDR R3, R4, #0			; load value in R4 in R3 (the next hex number to be printed)
				LD R0, NEW_LINE			; load R0 with new line character
				OUT						; print new line
				ADD R6, R6, #-1			; decrement R6 with #1

; print bin character and space

SHIFT			BRnz DONE				; move to DONE if R6 is zero or negative
				AND R0, R0, #0			; clear R0
				ADD R0, R0, R5			; load R0 with R5
				OUT						; print character
                LD R0, SPACE			; load R0 with space character
                OUT						; print space
               	AND R4, R4, #0			; clear R4
                ADD R4, R4, #4			; initialize R4 with #4 (digit counter)

; checks if digit has been counted

CHECK_DONE		BRnz CONTINUE			; move to CONTINUE if R4 is zero or negative
                AND R1, R1, #0			; clear R1 (digit)
                AND R2, R2, #0			; clear R2
                ADD R2, R2, #4			; initialize R2 wirh #4 (bit counter)

; load digit with 4 bits

BACK            BRnz FIND_DIGIT			; move to FIND_DIGIT if R2 is zero or negative
                ADD R1, R1, R1			; shift left R1
                ADD R3, R3, #0			; set condition codes to R3
                BRn NEGATIVE			; move to NEGATIVE if R3 is negative
                ADD R1, R1, #0			; add #0 to R1
                BRnzp OUT_LOOP			; move to OUT_LOOP always
NEGATIVE        ADD R1, R1, #1			; add #1 to R1
OUT_LOOP        ADD R3, R3, R3			; shift left R3
                ADD R2, R2, #-1			; decrement R2 with #1
                BRnzp BACK				; move to BACK always

; find hex value of digit

FIND_DIGIT      AND R0, R0, #0			; clear R0
                ADD R0, R1, #-9			; load R0 with R1-#9
				BRnz PRINT				; move to PRINT if R0 is zero or negative
                LD R7, LETTER			; load R7 with LETTER
                ADD R0, R0, R7			; add R0 and R7
                AND R1, R1, #0			; clear R1
                ADD R1, R1, R0			; load R1 with R0

; prints digit in hex

PRINT           AND R0, R0, #0			; clear R0
                ADD R0, R0, R1			; load R0 with R1
                LD R7, ZERO				; load R7 with ASCII value of '0'
				ADD R0, R0, R7			; add R0 and R7
            	OUT						; print R0
            	ADD R4, R4, #-1			; decrement R4 with #1	
        		BRnzp CHECK_DONE		; move to CHECK_DONE always				

; done

DONE    		HALT            		; done



; the data needed by the program

NUM_BINS        .FILL #27       		; 27 loop iterations
NEG_AT          .FILL xFFC0   	  		; the additive inverse of ASCII '@'
AT_MIN_Z        .FILL xFFE6  	   		; the difference between ASCII '@' and 'Z'
AT_MIN_BQ       .FILL xFFE0  	   		; the difference between ASCII '@' and '`'
HIST_ADDR       .FILL x3F00  	   		; histogram starting address
HIST_ADDR_TEMP	.FILL x3F00				; temporary histogram address
STR_START       .FILL x4000	  	   		; string starting address
SPACE           .FILL x0020				; ASCII value of 'space'
NEW_LINE        .FILL x000A 			; ASCII value of 'new line'
START			.FILL x0040 			; ASCII value of '@'
LETTER          .FILL x0010  	   		; hex value x10
ZERO            .FILL x0030  	   		; ASCII value of '0'

				.END

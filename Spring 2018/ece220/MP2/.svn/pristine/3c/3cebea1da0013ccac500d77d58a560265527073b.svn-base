; Developer: Unnat Ramjiyani
; NetID: unnatr2
; Course: ECE220
; MP#2

; The following code is a calculator that uses a stack and stack functiions
; to calculate operations like- ADDITION, SUBTRACTION, MULTIPLICATION,
; DIVISION, EXPONENTIAL. The program starts by reading a character and
; evaluating whether it is a number or an operator. If the character is '=',
; the evaluation ends and the result is printed through the screen. The
; evaluation is carried out by the sub-routine EVALUATE and the printing of
; the result is carried out by the sub-routine PRINT_HEX. If at any pointe
; the expression entered by the user is deemed invalid, the program prints
; "Invalid Expression" to the screen using the INVALID sub-routine.

			.ORIG x3000		; Start of program

LOOP_TOP		LD R1, NEG_EQUAL	; Load R1 with -'='
			GETC			; Get character from user
			OUT			; Echo character on screen
			ADD R1, R1, R0		; Add R1 and R0
			BRz LOOP_OUT		; If ZERO, move to LOOP_OUT
			JSR EVALUATE		; Jump to sub-routine EVALUATE
			BRnzp LOOP_TOP		; Always move to LOOP_TOP

LOOP_OUT		LD R3, STACK_START	; Load R3 with starting address of stack
			LD R4, STACK_TOP	; Load R4 with current address of stack
			NOT R3, R3		; Not R3
			ADD R3, R3, #1		; Add #1 to R3
			ADD R4, R4, R3		; Add R3 and R4
			ADD R4, R4, #1		; Add #1 to R4
			BRz PRINT_RESULT	; If ZERO, move to PRINT_RESULT
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp DONE		; Always move to DONE
PRINT_RESULT		LDI R5, STACK_START	; Load R5 with value in the start of stack
			AND R3, R3, #0		; Clear R3
			ADD R3, R3, R5		; Load R3 with R5
			JSR PRINT_HEX		; Jump to sub-routine PRINT_HEX
DONE			HALT			; Halt program


NEG_EQUAL		.FILL xFFC3		; ASCII value of -'='
NEG_SPACE		.FILL xFFE0		; ASCII value of -'(space)'

; The PRINT_HEX sub-routine prints the 4-digit hex value in R3. This program
; was the same program used to print hex numbers in LAB1.

PRINT_HEX
			ST R7, PRINT_HEX_SaveR7	; Save value in R7
			AND R4, R4, #0		; Clear R4
			ADD R4, R4, #4		; Add #4 to R4
CHECK_DONE		BRnz PRINT_HEX_RET	; If NEGATIVE or ZERO, move to PRINT_HEX_RET
			AND R1, R1, #0		; Clear R1
			AND R2, R2, #0		; Clear R2
			ADD R2, R2, #4		; Add #4 to R2
BACK			BRnz FIND_DIGIT		; IF NEGATIVE or ZERO, move to FIND_DIGIT
			ADD R1, R1, R1		; Left shift R1
			ADD R3, R3, #0		; Set condition codes to R3
			BRn NEGATIVE		; If NEGATIVE, move to NEGATIVE
			ADD R1, R1, #0		; Set condition codes to R1
			BRnzp OUT_LOOP		; Always move to OUT_LOOP
NEGATIVE		ADD R1, R1, #1		; Add #1 to R1
OUT_LOOP		ADD R3, R3, R3		; Left sjift R3
			ADD R2, R2, #-1		; Decrement R2 by #1
			BRnzp BACK		; Always move to BACK
FIND_DIGIT		AND R5, R5, #0		; Clear R5
			ADD R5, R1, #-9		; Add R5 with R1-#9
			BRnz PRINT		; If NEGATIVE or ZERO, move to PRINT
			LD R6, LETTER		; Load R6 with LETTER
			ADD R5, R5, R6		; Add R5 and R6
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R5		; Add R1 and R5
PRINT			AND R0, R0, #0		; Clear R0
			ADD R0, R0, R1		; Add R0 and R1
			LD R6, ZERO		; Load R6 with ZERO
			ADD R0, R0, R6		; Add R0 and R6
			OUT			; Print R0
			ADD R4, R4, #-1		; Decrement R4 by #1
			BRnzp CHECK_DONE	; Always move to CHECK_DONE

PRINT_HEX_RET		LD R7, PRINT_HEX_SaveR7 ; Reload R7
			RET			; Return

PRINT_HEX_SaveR7	.FILL x0000		; Memory address to save R7
LETTER			.FILL x0010		; Fill for ASCII value of letter
ZERO			.FILL x0030		; ASCII value of '0'

; This sub-routine evaluates every character that is not '=' and checks
; whether it is an operator or an operand. If the character is an operator,
; the program moves to the sub-routine of that operator. If the character
; is an operand, it pushes it on the stack.

EVALUATE
			ST R1, EVALUATE_SaveR1	; Save R1
			ST R7, EVALUATE_SaveR7	; Save R7
			LD R1, NEG_SPACE	; Load R1 with -'(space)'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DONE_EVALUATE	; If ZERO, move to DONE_EVALUATE
			LD R1, NEG_PLUS		; Load R1 with -'+'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DO_PLUS		; If ZERO, move to DO_PLUS
			LD R1, NEG_MINUS	; Load R1 with -'-'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DO_MINUS		; If ZERO, move to DO_MINUS
			LD R1, NEG_MULT		; Load R1 with -'*'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DO_MULT		; If ZERO, move to DO_MULT
			LD R1, NEG_DIV		; Load R1 with -'/'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DO_DIV		; If ZERO, move to DO_DIV
			LD R1, NEG_POW		; Load R1 with -'^'
			ADD R1, R1, R0		; Add R1 and R0
			BRz DO_POW		; If ZERO, move to DO_POW
			LD R1, NEG_ZERO		; Load R1 with -'0'
			ADD R1, R1, R0		; Add R1 and R0
			BRzp CHECK_NINE		; If ZERO or POSITIVE, move to CHECK_NINE
			BRnzp DO_INVALID	; Always, move to DO_INVALID
CHECK_NINE		LD R1, NEG_NINE		; Load R1 with -'9'
			ADD R1, R1, R0		; Add R1 and R0
			BRnz DO_NUM		; If ZERO, move to DO_NUM

DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_PLUS
			JSR PLUS		; Jump to sub-routine PLUS
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_MINUS
			JSR MINUS		; Jump to sub-routine MINUS
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_MULT
			JSR MULT		; Jump to sub-routine MULT
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_DIV
			JSR DIV			; Jump to sub-routine DIV
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_POW
			JSR POW			; Jump to sub-routine POW
			BRnzp DONE_EVALUATE	; Always move to DONE_EVALUATE
DO_NUM
			LD R1, NEG_ZERO		; Load R1 with -'0'
			ADD R0, R0, R1		; Add R0 and R1
			JSR PUSH		; Jump to sub-routine PUSH

DONE_EVALUATE
			LD R1, EVALUATE_SaveR1	; Reload R1
			LD R7, EVALUATE_SaveR7	; Relaod R7
			RET			; Return

EVALUATE_SaveR1		.FILL x0000		; Address to save R1
EVALUATE_SaveR7		.FILL x0000		; Address to save R7

PRINT_INV		.STRINGZ "Invalid Expression"	; String to print invalid message
NEG_PLUS		.FILL xFFD5		; -'+'
NEG_MINUS		.FILL xFFD3		; -'-'
NEG_MULT		.FILL xFFD6		; -'*'
NEG_DIV			.FILL xFFD1		; -'/'
NEG_POW			.FILL xFFA2		; -'^'
NEG_ZERO		.FILL xFFD0		; -'0'
NEG_NINE		.FILL xFFC7		; -'9'

INVALID
			LEA R0, PRINT_INV	; Load address of PRINT_INV
			PUTS			; Print string
			BRnzp DONE		; Always move to DONE
			RET			; Return

; This sub-routine pops two numbers from the stack, adds them and then
; pushes the result onto the stack.

PLUS
			ST R7, PLUS_SaveR7	; Save value of R7
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp PLUS_DO_INVALID	; If POSITIVE, move to PLUS_DO_INVALID
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R1 adn R0
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp PLUS_DO_INVALID	; If POSITIVE, move to PLUS_DO_INVALID
			ADD R0, R0, R1		; Add R1 and R0
			JSR PUSH		; Jump to sub-routine PUSH
PLUS_DO_RET
			LD R7, PLUS_SaveR7	; Reload R7
			RET			; Return

PLUS_DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp PLUS_DO_RET	; Always move to PLUS_DO_RET

PLUS_SaveR7		.FILL x0000		; Address to save R7

; This sub-routine pops two numbers from the stack, subtracts them and then
; pushes the result onto the stack.

MINUS
			ST R7, MINUS_SaveR7	; Save R7
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp MINUS_DO_INVALID	; If POSITIVE, move to MINUS_DO_INVALID
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R1 and R0
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp MINUS_DO_INVALID	; If POSITIVE, move to MINUS_DO_INVALID
			NOT R1, R1		; Not R1
			ADD R1, R1, #1		; Add #1 to R1
			ADD R0, R0, R1		; Add R1 and R0
			JSR PUSH		; Jump to sub-routine PUSH
MINUS_DO_RET
			LD R7, MINUS_SaveR7	; Reload R7
			RET			; Return

MINUS_DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp MINUS_DO_RET	; Always move to MINUS_DO_RET

MINUS_SaveR7		.FILL x0000		; Address to save R7

; Labels used for storing the start and end addresses of the stack and also the
; current location of stack pointer.

STACK_END		.FILL x3FF0		; Address of top of stack
STACK_START		.FILL x4000		; Address of bottom of stack
STACK_TOP		.FILL x4000		; Address of current address in stack

; This sub-routine pops two numbers from the stack, multiplies them and then
; pushes the result onto the stack.

MULT
			ST R7, MULT_SaveR7	; Save R7
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp MULT_DO_INVALID	; If POSTIVE, move to MULT_DO_INVALID
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R1 and R0
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp MULT_DO_INVALID	; If POSTIVE, move to MULT_DO_INVALID

			AND R7, R7, #0		; Clear R7
MULT_LOOP		ADD R7, R7, R1		; Add R1 and R7
			ADD R0, R0, #-1		; Decrement R0 by #1
			BRnp MULT_LOOP		; If NEGATIVE or POSITIVE, move to MULT_LOOP
			ADD R0, R0, R7		; Add R0 and R7
			JSR PUSH		; Jump to sub-routine PUSH

MULT_DO_RET
			LD R7,  MULT_SaveR7	; Relaod R7
			RET			; Return

MULT_DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp MULT_DO_RET	; Always move to MULT_DO_RET

MULT_SaveR7		.FILL x0000		; Address to save R7

; This sub-routine pops two numbers from the stack, divides them and then
; pushes the result onto the stack.

DIV
			ST R7, DIV_SaveR7	; Save R7
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp DIV_DO_INVALID	; If POSITIVE, move to DIV_DO_INVALID
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R1 and R0
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp DIV_DO_INVALID	; If POSITIVE, move to DIV_DO_INVALID

			AND R7, R7, #0		; Clear R7
			ADD R7, R7, R0		; Add R0 and R7
			AND R0, R0, #0		; Clear R0
			NOT R1, R1		; Not R1
			ADD R1, R1, #1		; Add #1 to R1
DIV_LOOP		ADD R7, R7, R1		; Add R1 and R7
			BRn DIV_OUT		; If NEGATIVE, move to DIV_OUT
			ADD R0, R0, #1		; Add #1 to R0
			BRnzp DIV_LOOP		; Always move to DIV_LOOP

DIV_OUT
			JSR PUSH		; Jump to sub-routine PUSH

DIV_DO_RET
			LD R7,  DIV_SaveR7	; Reload R7
			RET			; Return

DIV_DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp DIV_DO_RET	; Always move to DIV_DO_RET

DIV_SaveR7		.FILL x0000		; Address to save R7

; This sub-routine pops two numbers from the stack, raises one number to the
; power of the other and then pushes the result onto the stack.

POW
			ST R2, POW_SaveR2	; Save R2
			ST R7, POW_SaveR7	; Save R7

			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp POW_DO_INVALID	; If POSITIVE, move to POW_DO_INVALID
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R0 and R1
			JSR POP			; Jump to sub-routine POP
			ADD R5, R5, #0		; Set condition codes to R5
			BRp POW_DO_INVALID	; If POSITIVE, move to POW_DO_INVALID

			AND R2, R2, #0		; Clear R2
			ADD R2, R2, R1		; Add R1 and R2
			AND R1, R1, #0		; Clear R1
			ADD R1, R1, R0		; Add R1 and R0
			AND R0, R0, #0		; Clear R0
			ADD R0, R0, #1		; Add #1 to R0
			ADD R2, R2, #0		; Set condition codes to R2
			BRz POW_OUT		; If ZERO, move to POW_OUT
POW_LOOP		JSR POW_MULT		; Jump to sub-routine POW_MULT
			ADD R2, R2, #-1		; Decrement R2 by #1
			BRnp POW_LOOP		; If NEGATIVE or POSITIVE, move to POW_LOOP

POW_OUT
			JSR PUSH		; Jump to sub-routine PUSH

POW_DO_RET
			LD R2,  POW_SaveR2	; Reload R2
			LD R7,  POW_SaveR7	; Reload R7
			RET			; Return

POW_DO_INVALID
			JSR INVALID		; Jump to sub-routine INVALID
			BRnzp POW_DO_RET	; Always move to POW_DO_RET

POW_SaveR2		.FILL x0000		; Address to save R2
POW_SaveR7		.FILL x0000		; Address to save R7

; This sub-routine uses multiplies the number to itself and then returns
; the value back to sub-routine MULT.

POW_MULT
			ST R7, POW_MULT_SaveR7	; Save R7
			ST R2, POW_MULT_SaveR2	; Save R2
			AND R2, R2, #0		; Clear R2
POW_MULT_LOOP		ADD R2, R2, R1		; Add R1 and R2
			ADD R0, R0, #-1		; Decrement R0 by #1
			BRnp POW_MULT_LOOP	; If NEGATIVE or POSITIVE, move to POW_MULT_LOOP
			ADD R0, R2, #0		; Add R0 and R2

			LD R7, POW_MULT_SaveR7	; Reload R7
			LD R2, POW_MULT_SaveR2	; Reload R2
			RET			; Return

POW_MULT_SaveR7		.FILL x0000		; Address to save R7
POW_MULT_SaveR2		.FILL x0000		; Address to save R2

; This sub-routine pushes R0 onto the top of the stack

PUSH
			ST R3, PUSH_SaveR3	; Save R3
			ST R4, PUSH_SaveR4	; Save R4
			ST R7, PUSH_SaveR7	; Save R7
			AND R5, R5, #0		; Clear R5
			LD R3, STACK_END	; Load R3 with bottom of stack
			LD R4, STACK_TOP	; Load R4 with current address of stack
			ADD R3, R3, #-1		; Decrement R3 by #1
			NOT R3, R3		; Not R3
			ADD R3, R3, #1		; Increment R3 by #1
			ADD R3, R3, R4		; Add R3 and R4
			BRz OVERFLOW		; Stack is full
			STR R0, R4, #0		; No overflow, store value in the stack
			ADD R4, R4, #-1		; Move top of the stack
			ST R4, STACK_TOP	; Store top of stack pointer
			BRnzp DONE_PUSH		; Always move to DONE_PUSH
OVERFLOW
			ADD R5, R5, #1		; Increment R5 by #1
DONE_PUSH
			LD R3, PUSH_SaveR3	; Reload R3
			LD R4, PUSH_SaveR4	; Reload R4
			LD R7, PUSH_SaveR7	; Reload R7
			RET			; Return

PUSH_SaveR3		.BLKW #1		; Address to save R3
PUSH_SaveR4		.BLKW #1		; Address to save R4
PUSH_SaveR7		.BLKW #1		; Address to save R7

; This sub-routine removes the topmost number on the stack and saves it in
; R0

POP
			ST R3, POP_SaveR3	; Save R3
			ST R4, POP_SaveR4	; Save R3
			ST R7, POP_SaveR7	; Save R7
			AND R5, R5, #0		; Clear R5
			LD R3, STACK_START	; Load R3 with bottom of stack
			LD R4, STACK_TOP	; Load R4 with current address of stack
			NOT R3, R3		; Not R3
			ADD R3, R3, #1		; Add #1 to R3
			ADD R3, R3, R4		; Add R3 and R4
			BRz UNDERFLOW		; If ZERO, move to UNDERFLOW
			ADD R4, R4, #1		; Increment R4 by #1
			LDR R0, R4, #0		; Load value in memory address of R4
			ST R4, STACK_TOP	; Store R4 as current address of stack
			BRnzp DONE_POP		; Always move to DONE_POP
UNDERFLOW
			ADD R5, R5, #1		; Add #1 to R5
DONE_POP
			LD R3, POP_SaveR3	; Reload R3
			LD R4, POP_SaveR4	; Reload R4
			LD R7, POP_SaveR7	; Reload R7
			RET			; Return

POP_SaveR3		.BLKW #1		; Address to save R3
POP_SaveR4		.BLKW #1		; Address to save R4
POP_SaveR7		.BLKW #1		; Address to save R7

			.END			; End of program

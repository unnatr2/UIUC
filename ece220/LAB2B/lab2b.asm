;R7 = numerator
;10 = denominator
			.ORIG x3000

LOOP_TOP		ADD R3, R3, #0
			BRnz DO_PRINT
			JSR DIV
			BRnzp LOOP_TOP

DO_PRINT
			LD R5, STACK_TOP
			LD R6, STACK_START
			NOT R6, R6
			ADD R6, R6, #1
			ADD R5, R5, R6
			BRz DONE
			JSR POP
			OUT

DONE 			HALT

DIV
			ST R0, DIV_SaveR0
			ST R7, DIV_SaveR7
DIV_LOOP		ADD R3, R3, #-10
			BRn DIV_OUT
			ST R3, REMAINDER
			ADD R0, R0, #1
			BRnzp DIV_LOOP

DIV_OUT
			AND R3, R3, #0
			ADD R3, R3, R0
			LD R0, REMAINDER
			JSR PUSH

DIV_DO_RET
			LD R0,  DIV_SaveR0
			LD R7,  DIV_SaveR7
			RET


DIV_SaveR0		.FILL x0000
DIV_SaveR7		.FILL x0000
NEG_TEN			.FILL xFFF6
REMAINDER		.FILL x0000


PUSH
			ST R3, PUSH_SaveR3	;save R3
			ST R4, PUSH_SaveR4	;save R4
			ST R7, PUSH_SaveR7
			AND R5, R5, #0		;
			LD R3, STACK_END	;
			LD R4, STACK_TOP	;
			ADD R3, R3, #-1		;
			NOT R3, R3		;
			ADD R3, R3, #1		;
			ADD R3, R3, R4		;
			BRz OVERFLOW		;stack is full
			STR R0, R4, #0		;no overflow, store value in the stack
			ADD R4, R4, #-1		;move top of the stack
			ST R4, STACK_TOP	;store top of stack pointer
			BRnzp DONE_PUSH		;
OVERFLOW
			ADD R5, R5, #1		;
DONE_PUSH
			LD R3, PUSH_SaveR3	;
			LD R4, PUSH_SaveR4	;
			LD R7, PUSH_SaveR7
			RET


PUSH_SaveR3		.BLKW #1	;
PUSH_SaveR4		.BLKW #1	;
PUSH_SaveR7		.BLKW #1	;

;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP
			ST R3, POP_SaveR3	;save R3
			ST R4, POP_SaveR4	;save R3
			ST R7, POP_SaveR7
			AND R5, R5, #0		;clear R5
			LD R3, STACK_START	;
			LD R4, STACK_TOP	;
			NOT R3, R3		;
			ADD R3, R3, #1		;
			ADD R3, R3, R4		;
			BRz UNDERFLOW		;
			ADD R4, R4, #1		;
			LDR R0, R4, #0		;
			ST R4, STACK_TOP	;
			BRnzp DONE_POP		;
UNDERFLOW
			ADD R5, R5, #1		;
DONE_POP
			LD R3, POP_SaveR3	;
			LD R4, POP_SaveR4	;
			LD R7, POP_SaveR7
			RET


POP_SaveR3		.BLKW #1	;
POP_SaveR4		.BLKW #1	;
POP_SaveR7		.BLKW #1
STACK_END		.FILL x3FF0	;
STACK_START		.FILL x4000	;
STACK_TOP		.FILL x4000	;


			.END

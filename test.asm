; This is a blink example
; Written in ARM Assembly

	.text
	.thumb

	.def main
	.def GPIOF_init
	.def RED_ON


SYSCTL_RCGCGPIO_R: .word 400FE608h
GPIO_PORTF_DIR_R: .word 40025400h
GPIO_PORTF_DEN_R: .word 4002551Ch
GPIO_PORTF_DATA_R: .word 400253FCh

		.align 4
main:	.asmfunc
			BL	GPIOF_init
loop:		BL	RED_ON
			B	loop
		.endasmfunc

		.align 4

GPIOF_init: .asmfunc
		LDR	R1,SYSCTL_RCGCGPIO_R
		LDR R0, [R1]
		ORR R0,R0,#0x20
		STR	R0,[R1]

		LDR R1,GPIO_PORTF_DIR_R
		MOV R0,#0x02
		STR R0,[R1]

		LDR R1,GPIO_PORTF_DEN_R
		MOV R0,#0x02
		STR R0,[R1]

		BX	LR
		.endasmfunc

		.align 4

RED_ON: .asmfunc
		LDR R1,GPIO_PORTF_DATA_R
		MOV R0,#0x02
		STR R0,[R1]
		BX	LR
		.endasmfunc

	.align 4
	.end

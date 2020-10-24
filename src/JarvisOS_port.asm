; [Filename]:		JarvisOS_port.asm
; [Description]:	Assembly file used to port Jarvis-OS to ARM-Cortex M4 Processor
; [Engineer]:		Hesham Khaled

			.thumb						;Execute the code in Thumb Mode
			.ref	currPtr				;Extern currPtr from Jarvis-OS-Kernel.c
			.ref	LoadNextThread
			.def	SysTick_Handler		;Define SysTick_Handler Function (Like C Prototypes)
			.def	Scheduler_init

currThread:	.word	currPtr

; [Function Name]:	SysTick_Handler
; [Description]:	Function responsible for context switching between threads
	.align 4
SysTick_Handler: .asmfunc
	CPSID	I					; Disable Global Interrupts
	PUSH	{R4-R11}			; Push the rest of Registers int the stack
	LDR		R0,currThread		; R0 <- Current Thread TCB Address
	LDR		R1,[R0]				; R1 <- The value that R0 is pointing at (Current Thread TCB Address)
	STR		SP,[R1]				; [R1]=Current Thread TCB Stack Pointer Address <- Physical Stack Pointer
	PUSH	{R0,LR}				; Push R0 and Link Register before calling A .c function
	BL		LoadNextThread
	POP		{R0,LR}
	LDR		R1,[R0]
	LDR		SP,[R1]				; SP <- R0
	POP		{R4-R11}			; POP Registers of new Thread to the Register Bank
	CPSIE	I					; Enable Global Interrupts
	BX		LR					; Return
				.endasmfunc

; [Function Name]:	Scheduler_init
; [Description]:	Function responsible for initializing SP to thread stack.
	.align 4
Scheduler_init: .asmfunc
	CPSID	I					; Disable Global Interrupts
	LDR		R0,currThread		; R0 <- Current Thread TCB Address
	LDR		R2,[R0]				; R2 <- The value that R0 is pointing at (Current Thread TCB Address)
	LDR		SP,[R2]				; SP <- stackPtr of the current Thread
	POP		{R4-R11}			; POP Registers R4~R11 of current thread into Register Bank
	POP		{R0-R3}				; POP Registers R0~R3 of current thread into Register Bank
	POP		{R12}				; POP Register R12 of current thread into Register Bank
	ADD		SP,SP,#4
	POP		{LR}
	ADD		SP,SP,#4
	CPSIE	I					; Enable Global Interrupts
	BX		LR					; Return
				.endasmfunc

.end

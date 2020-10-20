/******************************************************************************
 *
 * Module: Micro - Configuration
 *
 * File Name: common_macros.h
 *
 * Description: Header file for the commonly used Macros
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#ifndef COMMON_MACROS
#define COMMON_MACROS

/* Enable Global Interrupt bit */
#define sei()            __asm("CPSIE:")
#define cli()            __asm("CPSID:")

/* Access a specific register of a given port */
#define ACCESS_REG(BASE,OFFSET) (*((volatile uint32_t*)(BASE+OFFSET)))

/* Set a certain bit in any register */
#define SET_BIT(BASE,OFFSET,PIN) ((ACCESS_REG(BASE,OFFSET))|=(1<<PIN))

/* Clear a certain bit in any register */
#define CLEAR_BIT(BASE,OFFSET,PIN) ((ACCESS_REG(BASE,OFFSET))&=(~(1<<PIN)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(BASE,OFFSET,PIN) ((ACCESS_REG(BASE,OFFSET))^=(1<<PIN))

/* Rotate right the register value with specific number of rotates */
#define ROR(BASE,OFFSET,num) ((ACCESS_REG(BASE,OFFSET))= ((ACCESS_REG(BASE,OFFSET))>>num) | ((ACCESS_REG(BASE,OFFSET))<<(8-num)))

/* Rotate left the register value with specific number of rotates */
#define ROL(BASE,OFFSET,num) ((ACCESS_REG(BASE,OFFSET))= ((ACCESS_REG(BASE,OFFSET))<<num) | ((ACCESS_REG(BASE,OFFSET))>>(8-num)))

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(BASE,OFFSET,PIN) ((ACCESS_REG(BASE,OFFSET)) & (1<<PIN))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(BASE,OFFSET,PIN) (!((ACCESS_REG(BASE,OFFSET)) & (1<<PIN)))

#endif /* COMMON_MACROS_H_ */

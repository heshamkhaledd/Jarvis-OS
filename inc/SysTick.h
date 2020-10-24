/******************************************************************************
 *
 *
 * [File Name]:     SysTick.h
 *
 * [Description]:   SysTick Timer Header File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#ifndef _SYSTICK_H
#define _SYSTICK_H

/* Include Required MACROS */
#include "common_macros.h"

/*******************************************************************************
 *                   SysTick Registers and Port Definitions
 ******************************************************************************/
#define SysTick             0xE000E000
#define LEAST_PRIORITY      0xE0000000            /* Priority = 7 in ARM Cortex M4 */
                                                  /* Registers offset */

/* CTCTRL: SysTick Control Register.
 *
 * Bit[0]  ->        0 := Counter Disabled
 *                   1 := Enable SysTick Count Down
 *
 * Bit[1]  ->        0 := Interrupt Generation is Disabled
 *                   1 := Interrupt Generation is Enabled when Counter reaches 0
 *
 * Bit[2]  ->        0 := Internal Oscillator/4 Clock
 *                   1 := System Clock
 *
 * Bit[16] ->        Count down Flag, Cleared by either reading STCTRL or Writing to CTCURRENT
 */
#define STCTRL      0x010


/* STRELOAD: Stores the start value of SysTick down counter
 * Ranges from 0x000000 ~ 0xFFFFFF (Only the first 24-bits are used)
*/
#define STRELOAD    0x014

/* STCURRENT: The actual register that's counted down */
#define STCURRENT   0x018

/* SYSPRI3:
 * Bit[29]~Bit[30]: Responsible for changing SysTick Timer Priority from 0 ~ 7 */
#define SYSPRI3     0xD20

/* INTCTRL: Responsible for Triggering SysTick_Handler */
#define INTCTRL     0xD04


/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/
void SysTick_init (void);


#endif

/******************************************************************************
 *
 *
 * [File Name]:     SysTick.c
 *
 * [Description]:   SysTick Timer Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#include <stdint.h>
#include "SysTick.h"
#include "JarvisOS_CONFIG.h"
#include "JarvisOS_kernel.h"


/******************************************************************************
 *
 * [Function Name]:     SysTick_init
 *
 * [Description]:       Responsible for Initializing SysTick Hardware and configuring it
 *
 * [Arguments]:         void
 * [Return]:            void
 *
 *****************************************************************************/
void SysTick_init (void)
{
    /* Manually Disabling SysTick before configuring it */
    ACCESS_REG(SysTick,STCTRL) = 0;
    ACCESS_REG(SysTick,STCURRENT) = 0;

    /* Set SysTick Timer to Have the Least Interrupt Priority */
    ACCESS_REG(SysTick,SYSPRI3) = (ACCESS_REG(SysTick,SYSPRI3) & 0x07FFFFFF) | (LEAST_PRIORITY);

    /* Load Quanta value to the SysTick Reload Register */
    ACCESS_REG(SysTick,STRELOAD) = (MS_TO_TICKS(QUANTA)) - 1;

    /* 1. Enable Counting
     * 2. Enable SysTick Interrupt IRQs
     * 3. Set SysTick Clock to CPU Clock
     */
    ACCESS_REG(SysTick,STCTRL) |= 0x00000007;

    return;
}

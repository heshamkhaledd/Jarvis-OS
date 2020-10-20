/******************************************************************************
 *
 *
 * File Name: JarvisOS-kernel.c
 *
 * Description: Private Kernel Source File
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "JarvisOS_kernel.h"



TCB Threads[NUM_OF_THREADS+1];                                /* Declare an array of Threads */

TCB *currPtr = NULL;                                          /* Pointer to the current running Thread */

int32_t TCB_STACK[NUM_OF_THREADS+1][STACK_SIZE];

/******************************************************************************
 *
 * Function Name: JARVIS_initKernel
 *
 * Description: Responsible for Initializing SysTick Timer Hardware and
 *              Initializing the Round-Robin Scheduler.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void JARVIS_initKernel(void)
{
    SysTick_init();                                         /* Configure SysTick Timer to Round-Robin Quanta Value (in milliseconds) */

    Scheduler_init();                                       /* Assembly Function @ JarvisOS_port.asm */
}


/******************************************************************************
 *
 * Function Name: JARVIS_initStack
 *
 * Description: Responsible for creating the stack of each thread.
 *              & assigning stack pointer to the top of the exception stack area
 *
 * Arguments:   uint32_t Idx
 * Return:      void
 *
 *****************************************************************************/
void JARVIS_initStack (uint8_t Idx) {

    Threads[Idx].stackPtr = &TCB_STACK[Idx][STACK_SIZE-16];
    TCB_STACK[Idx][STACK_SIZE-1] = 0x1000000;               /* Assign Thread to execute in Thumb mode */
}


/******************************************************************************
 *
 * Function Name: ThreadCreate
 *
 * Description: API Function that's responsible for creating a new thread
 *
 * Arguments:   void(*Thread)(void), uint8_t a_priority
 * Return:      uint8_t
 *
 *****************************************************************************/
uint8_t ThreadCreate(void(*Thread)(void), uint8_t a_priority)
{
    __asm("CPSID:");                                        /* Disable Global Interrupt bit */

    static uint8_t Idx = 0;

    JARVIS_initStack(Idx);                                  /* Initialize Thread Stack */

    TCB_STACK[Idx][STACK_SIZE-2] = (int32_t)(Thread);       /* Thread PC <- Thread Address */

    Threads[Idx].priority = a_priority;                     /* Assign Thread Priority */

    Threads[Idx].status = READY;                            /* Thread is initialized in Ready state */

    Idx++;

    if (Idx == NUM_OF_THREADS)
        Generate_stateIdle (Idx);

    __asm("CPSIE:");                                        /* Enable Global Interrupt bit */

    return 1;
}


/******************************************************************************
 *
 * Function Name: Generate_stateIdle
 *
 * Description: Subroutine responsible for creating a wait state task (Wait Task)
 *              to run when all other threads are suspended.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void Generate_stateIdle (uint8_t Idx)
{
    JARVIS_initStack(Idx);
    TCB_STACK[Idx][STACK_SIZE-2] = (int32_t)(stateIdle);
    Threads[Idx].priority = 0;
    Threads[Idx].status = READY;
}


/******************************************************************************
 *
 * Function Name: stateIdle
 *
 * Description: Waiting state when all tasks are suspended.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void stateIdle (void)
{
    while (1) {}
}


/******************************************************************************
 *
 * Function Name: Thread_Suspend
 *
 * Description: API Function responsible for terminating a specific thread
 *              by triggering SysTick Handler to switch to a new thread.
 *
 * Arguments:   uint32_t port_DELAY
 * Return:      void
 *
 *****************************************************************************/
void Thread_Suspend (uint32_t port_DELAY)
{
    ACCESS_REG(SysTick,INTCTRL) = 0x04000000;
}

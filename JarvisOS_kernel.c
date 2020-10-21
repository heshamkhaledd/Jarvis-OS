/******************************************************************************
 * File Name: JarvisOS-kernel.c
 *
 * Description: Private Kernel Source File
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "JarvisOS_kernel.h"



TCB Threads[NUM_OF_THREADS+1];                                /* Declare an array of Threads. +1 for stateIdle subroutine */

TCB *currPtr = NULL;                                          /* Pointer to the current running Thread */

int32_t TCB_STACK[NUM_OF_THREADS+1][STACK_SIZE];

static uint32_t Jarvis_Ticks = 0;

uint8_t nextThreadIndex (TCB *ThreadsPtr)
{
    uint8_t Idx,nextIdx = 0;

    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)
    {
        if(Threads[Idx].status == READY && Threads[Idx].priority > nextIdx)
            nextIdx = Idx;
    }

    return nextIdx;
}


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
    currPtr = &Threads[nextThreadIndex(Threads)];

    SysTick_init();                                         /* Configure SysTick Timer to Round-Robin Quanta Value (in milliseconds) */

    Scheduler_init();                                       /* Assembly Function @ JarvisOS_port.asm */
}


void checkSuspendedState (void)
{
    uint8_t Idx;
    Jarvis_Ticks++;
    for (Idx = 0 ; Idx <NUM_OF_THREADS ; Idx++)
    {
        if (Threads[Idx].status == SUSPENDED && Threads[Idx].delayTime == Jarvis_Ticks)
            Threads[Idx].status = READY;
    }
    return;
}


void LoadNextThread(void)
{
    uint8_t Idx;
    checkSuspendedState ();

    Idx = nextThreadIndex(Threads);

    currPtr = &Threads[Idx];

    Threads[Idx].status = RUNNING;

    return;
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

    return;
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
    uint8_t Idx;

    if (port_DELAY == 0)
        return;

    for (Idx = 0; Idx<NUM_OF_THREADS ; Idx++)
    {
        if (Threads[Idx].status == RUNNING)
        {
            Threads[Idx].status = SUSPENDED;
            port_DELAY = port_DELAY / QUANTA;

            if (port_DELAY == 0)
                port_DELAY = 1;

            Threads[Idx].delayTime = Jarvis_Ticks + port_DELAY;
        }
        else
            Idx++;
    }

    ACCESS_REG(SysTick,STCURRENT) = 0;
    ACCESS_REG(SysTick,INTCTRL) = 0x04000000;

    return;
}

/******************************************************************************
 *
 * Function Name: Thread_Block
 *
 * Description: API Function responsible for Blocking a thread from executing
 *              till it's resumed
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void Thread_Block (void)
{
    uint8_t Idx;
    for (Idx = 0; Idx<NUM_OF_THREADS ; Idx++)
    {
        if (Threads[Idx].status == RUNNING)
            Threads[Idx].status = BLOCKED;
        else
            Idx++;
    }

    return;
}

/******************************************************************************
 *
 * Function Name: Thread_Resume
 *
 * Description: API Function responsible for continue a blocked thread and get
 *              it back to ready state again.
 *
 * Arguments:   uint32_t port_DELAY
 * Return:      void
 *
 *****************************************************************************/
void Thread_Resume (void)
{

    return;
}

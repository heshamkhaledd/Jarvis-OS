/******************************************************************************
 * [File Name]:     JarvisOS-kernel.c
 *
 * [Description]:   Private Kernel Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#include "JarvisOS_kernel.h"

/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/
TCB Threads[NUM_OF_THREADS+1];                              /* Declare an array of Threads. +1 for stateIdle subroutine */

TCB *currPtr = NULL;                                        /* Pointer to the current running Thread */

int32_t TCB_STACK[NUM_OF_THREADS+1][STACK_SIZE];            /* Declaring TCB (Threads) Stack */

static volatile uint32_t Jarvis_Ticks = 0;                  /* Global Variable to count SysTick countdown times */

/*******************************************************************************
 *                          Basic C String Functions
 ******************************************************************************/
int8_t strcmp(const uint8_t *Str_1, const uint8_t *Str_2)
{
    while (*Str_1)
    {
        if (*Str_1 != *Str_2)
            return -1;

        Str_1++;
        Str_2++;
    }
    return 0;
}
void strcpy (uint8_t *destination, const uint8_t *source)
{
    if (destination == NULL)
            return;

        while (*source != '\0')
        {
            *destination = *source;
            destination++;
            source++;
        }
        *destination = '\0';

        return;
}

/******************************************************************************
 *
 * [Function Name]: nextThreadIndex
 *
 * [Description]:   Responsible for finding the appropriate next Thread to run
 *                  if it's the highest priority and ready thread.
 *
 * [Arguments]:     TCB *ThreadsPtr
 * [Return]:        void
 *
 *****************************************************************************/
uint8_t nextThreadIndex (TCB *ThreadsPtr)
{
    uint8_t Idx,nextIdx=NUM_OF_THREADS;                     /* Next Thread to run index, by default, is assigned to the IdleThread */
    uint8_t max = 0;                                        /* Next Thread to run priority, by defauly, is assigned to the lowest priority */

    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)
    {
        if(ThreadsPtr[Idx].status == READY && ThreadsPtr[Idx].priority > max)
        {
            max = ThreadsPtr[Idx].priority;
            nextIdx = Idx;
        }
    }
    return nextIdx;
}


/******************************************************************************
 *
 * [Function Name]: JARVIS_initKernel
 *
 * [Description]:   Responsible for Initializing SysTick Timer Hardware and
 *                  Initializing the Round-Robin Scheduler.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void JARVIS_initKernel(void)
{
    uint8_t Idx;

    Idx = nextThreadIndex(Threads);                         /* Call nextThreadIndex to know which Thread will initially run */

    currPtr = &Threads[Idx];

    Threads[Idx].status = RUNNING;                          /* Assign the running state to the initial thread */

    SysTick_init();                                         /* Configure SysTick Timer to Round-Robin Quanta Value (in milliseconds) */

    Scheduler_init();                                       /* Assembly Function @ JarvisOS_port.asm */
}


/******************************************************************************
 *
 * [Function Name]: checkSuspendedState
 *
 * [Description]:   Releases suspended threads if their finished their suspension
 *                  time. This function is triggered every QUANTA.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void checkSuspendedState (void)
{
    uint8_t Idx;

    for (Idx = 0 ; Idx <NUM_OF_THREADS ; Idx++)
    {
        if (Threads[Idx].status == SUSPENDED && Threads[Idx].delayTime == Jarvis_Ticks)
            Threads[Idx].status = READY;
    }
    Jarvis_Ticks++;
    return;
}

/******************************************************************************
 *
 * [Function Name]: LoadNextThread
 *
 * [Description]:   Assembly subroutine called from JarvisOS_port.asm responsible
 *                  for loading the next appropriate thread into ARM processor.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void LoadNextThread(void)
{
    uint8_t Idx;
    checkSuspendedState ();                                 /* Release suspended tasks first before choosing the next thread to run */

    Idx = nextThreadIndex(Threads);

    currPtr = &Threads[Idx];

    if(Threads[NUM_OF_THREADS].status == RUNNING)           /* If the idleThread was running before this thread, return it to ready state */
        Threads[NUM_OF_THREADS].status = READY;

    Threads[Idx].status = RUNNING;                          /* Assign the next thread to run to the running state */

    return;
}

/******************************************************************************
 *
 * [Function Name]:     JARVIS_initStack
 *
 * [Description]:       Responsible for creating the stack of each thread.
 *                      & assigning stack pointer to the top of the exception stack area
 *
 * [Arguments]:         uint32_t Idx
 * [Return]:            void
 *
 *****************************************************************************/
void JARVIS_initStack (uint8_t Idx) {

    Threads[Idx].stackPtr = &TCB_STACK[Idx][STACK_SIZE-16]; /* Make the Thread stack pointer points to the Stack Frame section in the thread's stack */
    TCB_STACK[Idx][STACK_SIZE-1] = 0x1000000;               /* Assign Thread to execute in Thumb mode */
}


/******************************************************************************
 *
 * [Function Name]:     ThreadCreate
 *
 * [Description]:       API Function that's responsible for creating a new thread
 *
 * [Arguments]:         void(*Thread)(void), uint8_t a_priority
 * [Return]:            uint8_t
 *
 *****************************************************************************/
uint8_t ThreadCreate(uint8_t *idPtr, void(*Thread)(void), uint8_t a_priority)
{
    __asm("CPSID:");                                        /* Disable Global Interrupt bit */

    static uint8_t Idx = 0;

    JARVIS_initStack(Idx);                                  /* Initialize Thread Stack */

    TCB_STACK[Idx][STACK_SIZE-2] = (int32_t)(Thread);       /* Thread PC <- Thread Address */

    Threads[Idx].priority = a_priority;                     /* Assign Thread Priority */

    Threads[Idx].status = READY;                            /* Thread is initialized in Ready state */

    strcpy(Threads[Idx].ThreadID,idPtr);                    /* Assign Thread ID */

    Idx++;

    if (Idx == NUM_OF_THREADS)                              /* Create IdleThread if we reached maximum number of allowed threads */
        Generate_stateIdle (Idx);

    __asm("CPSIE:");                                        /* Enable Global Interrupt bit */

    return 1;
}


/******************************************************************************
 *
 * [Function Name]:     Generate_stateIdle
 *
 * [Description]:       Subroutine responsible for creating a wait state task (Wait Task)
 *                      to run when all other threads are suspended.
 *
 * [Arguments]:         void
 * [Return]:            void
 *
 *****************************************************************************/
void Generate_stateIdle (uint8_t Idx)
{
    JARVIS_initStack(Idx);                                  /* Create stack for IdleThread */
    TCB_STACK[Idx][STACK_SIZE-2] = (int32_t)(stateIdle);    /* Make the PC initially point to IdleThread address */
    Threads[Idx].priority = 0;                              /* Assign in to Kernel's lowest priority */
    Threads[Idx].status = READY;                            /* Initialize it as ready */

    return;
}


/******************************************************************************
 *
 * [Function Name]:     stateIdle
 *
 * [Description]:       Waiting state when all tasks are suspended / blocked.
 *
 * [Arguments]:         void
 * [Return]:            void
 *
 *****************************************************************************/
void stateIdle (void)
{
    while (1) {}                                            /* Keep waiting until a physical thread is ready to run */
}


/******************************************************************************
 *
 * [Function Name]:     Thread_Suspend
 *
 * [Description]:       API Function responsible for terminating a specific thread
 *                      by triggering SysTick Handler to switch to a new thread.
 *
 * [Arguments]:         uint32_t port_DELAY
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Suspend (uint32_t port_DELAY)
{
    uint8_t Idx;

    if (port_DELAY == 0)                                    /* if port_DELAY time is zero, do nothing and return */
        return;

    for (Idx = 0; Idx<NUM_OF_THREADS ; Idx++)               /* Keep looping on Threads until it finds the Thread who called it */
    {
        if (Threads[Idx].status == RUNNING)
        {
            Threads[Idx].status = SUSPENDED;

            Threads[Idx].delayTime = Jarvis_Ticks + port_DELAY;
            break;
        }
        else
            continue;
    }

    ACCESS_REG(SysTick,INTCTRL) = 0x04000000;               /* Trigger SysTick_Handler found @ JarvisOS_port.asm */
}


/******************************************************************************
 *
 * [Function Name]:     Thread_Block
 *
 * [Description]:       API Function responsible for Blocking a thread from executing
 *                      till it's resumed
 *
 * [Arguments]:         uint8_t ThreadID[ThreadID_MAX_LENGTH]
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Block (uint8_t *idPtr)
{
    uint8_t Idx;
    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)            /* Keep looping on Threads until it finds the thread required to be blocked */
    {
        if(strcmp(idPtr,Threads[Idx].ThreadID) == 0)
        {
            Threads[Idx].status = BLOCKED;
            break;
        }
        else
            continue;
    }
    return;
}

/******************************************************************************
 *
 * [Function Name]:     Thread_Resume
 *
 * [Description]:       API Function responsible for continue a blocked thread and get
 *                      it back to ready state again.
 *
 * [Arguments]:         uint8_t ThreadID[ThreadID_MAX_LENGTH]
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Resume (uint8_t *idPtr)
{
    uint8_t Idx;
    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)
    {
        if(strcmp(idPtr,Threads[Idx].ThreadID) == 0)        /* Keep looping on Threads until it finds the thread required to be resumed */
        {
            Threads[Idx].status = READY;
            break;
        }
        else
            continue;
    }
    return;
}
